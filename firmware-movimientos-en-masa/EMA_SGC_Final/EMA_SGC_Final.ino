#include <Arduino.h>
#include <esp_task_wdt.h>  
#include "driver/gpio.h" 
#include "driver/rtc_io.h" 
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "LittleFS.h"
#include <Wire.h>         
#include <RTClib.h>       

// --- LIBRERÍAS PARA EL PORTAL WI-FI ---
#include <WiFi.h>
#include <WebServer.h>

// --- CONFIGURACIÓN DE SEGURIDAD (Archivo externo) ---
#include "config_env.h"

// --- 1. CONFIGURACIÓN DEL MÓDEM A7670G ---
#define TINY_GSM_MODEM_SIM7600  
#define MODEM_TX             26 
#define MODEM_RX             27
#define PIN_MODEM_POWER      12 // Llave maestra de energía (SD y LDO del Módem)
#define PIN_MODEM_PWRKEY      4 // Botón interno (Para apagar/encender celular)

// --- 2. CONFIGURACIÓN DE SENSORES Y PINES ---
#define PIN_ENCODER          GPIO_NUM_32 
#define PIN_BOTON_WIFI       GPIO_NUM_0   // BOTÓN DE EXTRACCIÓN EN EL PIN 0 (BOOT)
#define BOTON_WIFI_MASK      (1ULL << 0)  // MÁSCARA PARA EL PIN 0
#define PIN_BATTERY          35 

// --- PINES I2C PARA EL RTC ---
#define I2C_SDA              21
#define I2C_SCL              22

// --- PINES SPI PERSONALIZADOS PARA LILYGO T-A7670 (MICRO SD) ---
#define PIN_SD_MISO           2
#define PIN_SD_MOSI          15
#define PIN_SD_SCLK          14
#define PIN_SD_CS            13 

#include <TinyGsmClient.h>
#include <PubSubClient.h>

// --- 3. CREDENCIALES DE RED Y MQTT (Desde config_env.h) ---
const char apn[]      = SECRET_APN; 
const char gprsUser[] = SECRET_GPRS_USER;
const char gprsPass[] = SECRET_GPRS_PASS;

const char* mqtt_server = SECRET_MQTT_SERVER;
const int   mqtt_port   = SECRET_MQTT_PORT;
const char* mqtt_user   = SECRET_MQTT_USER;
const char* mqtt_pass   = SECRET_MQTT_PASS;
const char* mqtt_topic  = SECRET_MQTT_TOPIC; 

// --- 4. CONFIGURACIÓN DE TIEMPOS Y WATCHDOG ---
#define uS_TO_S_FACTOR 1000000ULL 
#define TIEMPO_DORMIR_SEG 3600  // Tiempo de envío de datos      
#define WDT_TIMEOUT 120             

// --- 5. MEMORIA RTC (Sobrevive al Deep Sleep) ---
RTC_DATA_ATTR int contador_pulsos = 0;
RTC_DATA_ATTR int dia_anterior = -1;  
RTC_DATA_ATTR unsigned long registro_numero = 0;
RTC_DATA_ATTR bool modem_encendido = true; 
RTC_DATA_ATTR bool esperando_bloqueo = true; 

RTC_DS3231 rtc; 

// Variables de cálculo para el Encoder
const int ranurasDisco = 20;
const float diametroBase = 5.10; 
float circunferencia = 3.14159 * diametroBase;
const float factorRealCm = 0.8000; 

HardwareSerial SerialAT(1);
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

WebServer server(80);
bool modo_wifi_activo = false;
TaskHandle_t TareaSensorHandle = NULL;

// =========================================================================
//                  TAREA DEL NÚCLEO 1: LECTURA DEL SENSOR
// =========================================================================
void TareaLecturaSensor(void * pvParameters) {
    for(;;) {
        if (esperando_bloqueo == true && digitalRead(PIN_ENCODER) == 0) {
            esperando_bloqueo = false; 
        } 
        else if (esperando_bloqueo == false && digitalRead(PIN_ENCODER) == 1) {
            contador_pulsos++;
            esperando_bloqueo = true; 
            Serial.print("PULSO DETECTADO (N1): "); Serial.println(contador_pulsos);
        }
        vTaskDelay(1 / portTICK_PERIOD_MS); 
    }
}

// =========================================================================
//                  ZONA DEL SERVIDOR WEB (WI-FI)
// =========================================================================

void configurarServidorWeb() {
  server.on("/", HTTP_GET, []() {
    String uuid_esp32 = WiFi.softAPmacAddress();
    int bat = obtenerPorcentajeBateria();
    
    SPI.begin(PIN_SD_SCLK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
    bool sd_ok = SD.begin(PIN_SD_CS, SPI);
    if(sd_ok) SD.end();

    float distanciaM = (contador_pulsos * factorRealCm) / 100.0;

    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Monitoreo de Masa</title>";
    html += "<style>body{font-family:sans-serif; background:#eef2f3; padding:20px;} .panel{background:white; border-radius:10px; padding:20px; margin-bottom:20px; box-shadow:0 2px 5px rgba(0,0,0,0.1);} ";
    html += "h1{text-align:center; color:#2c3e50;} .uuid{text-align:center; color:#7f8c8d; font-family:monospace; margin-bottom:20px;} ";
    html += "a{display:block; background:#2196F3; color:white; padding:15px; text-align:center; text-decoration:none; border-radius:8px; margin:10px 0; font-weight:bold;} ";
    html += ".btn-exit{background:#e74c3c;}</style></head><body>";
    
    html += "<h1>Nodo Movimientos en Masa</h1>";
    html += "<p class='uuid'>S/N: " + uuid_esp32 + "</p>";

    html += "<div class='panel'><h2>Estado en Vivo</h2>";
    html += "<p style='background-color:#e8f4f8; padding:10px; border-radius:5px;'><strong>📊 Desplazamiento:</strong> " + String(distanciaM, 3) + " metros</p>";
    html += "<p style='font-size:12px; color:#7f8c8d;'>* Recarga la pagina para actualizar</p>";
    html += "<hr>";
    html += "<p><strong>Bateria:</strong> " + String(bat) + "%</p>";
    html += "<p><strong>SD Card:</strong> " + String(sd_ok ? "OK" : "ERROR") + "</p>";
    html += "</div>";

    html += "<div class='panel'><h2>Descargar Datos</h2>";
    html += "<a href='/descargar_sd'>1. Respaldo MicroSD (.csv)</a>";
    html += "<a href='/descargar_interno'>2. Respaldo Memoria Interna (.csv)</a>";
    html += "</div>";

    html += "<a href='/salir' class='btn-exit'>Apagar Wi-Fi y Dormir</a>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/descargar_sd", HTTP_GET, []() {
    SPI.begin(PIN_SD_SCLK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
    if (!SD.begin(PIN_SD_CS, SPI)) { server.send(500, "text/plain", "Error SD"); return; }
    File file = SD.open("/backup_masa.csv", FILE_READ);
    server.streamFile(file, "application/octet-stream");
    file.close(); SD.end();
  });

  server.on("/descargar_interno", HTTP_GET, []() {
    if (!LittleFS.begin(true)) { server.send(500, "text/plain", "Error FS"); return; }
    File file = LittleFS.open("/backup_interno.csv", FILE_READ);
    server.streamFile(file, "application/octet-stream");
    file.close(); LittleFS.end();
  });

  server.on("/salir", HTTP_GET, []() { server.send(200, "text/plain", "Cerrando..."); delay(1000); modo_wifi_activo = false; });
}

void iniciarPortalMantenimiento() {
  WiFi.disconnect(true); 
  WiFi.mode(WIFI_OFF); 
  delay(100);
  
  WiFi.persistent(false); 
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Masa_Prototipo", SECRET_WIFI_AP_PASS);
  
  configurarServidorWeb();
  server.begin();
  modo_wifi_activo = true;
  
  Serial.println(">>> PORTAL WI-FI ACTIVO: Masa_Prototipo <<<");
  
  while (modo_wifi_activo) { 
    esp_task_wdt_reset(); 
    server.handleClient(); 
    delay(10); 
  }

  Serial.println("Sincronizando mediciones con MQTT antes de finalizar...");
  transmitirDatos(); 

  WiFi.softAPdisconnect(true); 
  WiFi.mode(WIFI_OFF);
  Serial.println("Portal cerrado. Volviendo a modo ahorro.");
}

// =========================================================================
//                  ZONA DE FUNCIONES MÓDEM, BATERÍA Y SD
// =========================================================================

int obtenerPorcentajeBateria() {
  delay(100); 
  pinMode(PIN_BATTERY, INPUT);
  long suma_adc = 0;
  for(int i = 0; i < 10; i++) {
    suma_adc += analogRead(PIN_BATTERY);
    delay(10);
  }
  float adc_promedio = suma_adc / 10.0;
  float voltaje_bateria = (adc_promedio / 4095.0) * 3.3 * 2.0 * 1.097;
  int porcentaje = map(voltaje_bateria * 100, 320, 420, 0, 100);
  if (porcentaje > 100) porcentaje = 100;
  if (porcentaje < 0) porcentaje = 0;
  return porcentaje;
}

void sincronizarRTCConRed() {
  int año, mes, dia, hora, min, seg;
  float zona;
  if (modem.getNetworkTime(&año, &mes, &dia, &hora, &min, &seg, &zona)) {
    rtc.adjust(DateTime(año, mes, dia, hora, min, seg));
    Serial.println("🕒 RTC Sincronizado correctamente.");
  }
}

void obtenerFechaHora(char* fecha, char* hora) {
  DateTime now = rtc.now();
  sprintf(fecha, "%02d/%02d/%02d", now.day(), now.month(), now.year() % 100);
  sprintf(hora, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
}

void transmitirDatos() {
  Serial.println("\n--- INICIANDO PROTOCOLO DE TRANSMISIÓN ---");
  int pct_bateria = obtenerPorcentajeBateria();
  Serial.print("Nivel de Batería: "); Serial.print(pct_bateria); Serial.println("%");
  
  if (pct_bateria < 15 && modem_encendido) {
    Serial.println("⚠️ ALERTA: Batería crítica (<15%). Apagando módem celular.");
    pinMode(PIN_MODEM_PWRKEY, OUTPUT);
    digitalWrite(PIN_MODEM_PWRKEY, HIGH);
    delay(2500); 
    digitalWrite(PIN_MODEM_PWRKEY, LOW);
    modem_encendido = false;
  } 
  else if (pct_bateria >= 20 && !modem_encendido) {
    Serial.println("☀️ RECUPERACIÓN: Batería estable (>=20%). Encendiendo módem.");
    pinMode(PIN_MODEM_PWRKEY, OUTPUT);
    digitalWrite(PIN_MODEM_PWRKEY, HIGH);
    delay(1500); 
    digitalWrite(PIN_MODEM_PWRKEY, LOW);
    delay(5000);
    esp_task_wdt_reset(); 
    delay(5000);
    modem_encendido = true;
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    modem.init();
    modem.sendAT("+CTZU=1");
    modem.waitResponse(5000L);
  }

  char bat_str[4];
  sprintf(bat_str, "%03d", pct_bateria);
  char fecha[12], hora[12];
  
  if (modem_encendido) {
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    delay(500);
    for (int i = 0; i < 3; i++) { SerialAT.println("AT"); delay(150); }
    if (modem.waitForNetwork(60000L)) {
      sincronizarRTCConRed(); 
    }
    esp_task_wdt_reset(); 
  }

  obtenerFechaHora(fecha, hora);
  float distanciaM = (contador_pulsos * factorRealCm) / 100.0;
  char payload_char[150];
  sprintf(payload_char, "{\"f\":\"%s\",\"h\":\"%s\",\"d\":%.3f,\"b\":\"%s\"}", fecha, hora, distanciaM, bat_str);
  String payload = String(payload_char);

  // 1. Guardar en SD
  SPI.begin(PIN_SD_SCLK, PIN_SD_MISO, PIN_SD_MOSI, PIN_SD_CS);
  if (SD.begin(PIN_SD_CS, SPI)) {
    File f = SD.open("/backup_masa.csv", FILE_APPEND);
    if(f) { 
      f.println("Registro: " + String(registro_numero) + " | " + payload); 
      f.close(); 
      Serial.println("[OK] Guardado en SD.");
    }
    SD.end();
  } else {
    Serial.println("[ERROR] No se pudo escribir en la SD.");
  }

  // 2. Guardar en LittleFS (Memoria Interna)
  if (LittleFS.begin(true)) {
    File f = LittleFS.open("/backup_interno.csv", FILE_APPEND);
    if(f) { 
      f.println("Registro: " + String(registro_numero) + " | " + payload); 
      f.close(); 
      Serial.println("[OK] Guardado en Memoria Interna.");
    }
    LittleFS.end();
  }

  // 3. Enviar por MQTT
  if (modem_encendido) {
    if (!modem.isGprsConnected()) {
      Serial.println("Conectando GPRS...");
      if (modem.gprsConnect(apn, gprsUser, gprsPass)) {
        mqtt.setServer(mqtt_server, mqtt_port);
        if (mqtt.connect("NodoMasa", mqtt_user, mqtt_pass)) {
          mqtt.publish(mqtt_topic, payload.c_str());
          Serial.println("[OK] MQTT Publicado.");
          delay(1000);
          mqtt.disconnect();
        }
      }
    }
  }
  
  registro_numero++;
}

// =========================================================================
//                          BUCLE PRINCIPAL (SETUP)
// =========================================================================

void setup() {
  esp_task_wdt_config_t wdt_config;
  wdt_config.timeout_ms = WDT_TIMEOUT * 1000; 
  wdt_config.idle_core_mask = 3; 
  wdt_config.trigger_panic = true; 
  esp_task_wdt_reconfigure(&wdt_config); 
  esp_task_wdt_add(NULL); 

  pinMode(PIN_MODEM_PWRKEY, OUTPUT);
  digitalWrite(PIN_MODEM_PWRKEY, LOW); 
  gpio_hold_dis((gpio_num_t)PIN_MODEM_PWRKEY); 

  pinMode(MODEM_TX, OUTPUT);
  digitalWrite(MODEM_TX, HIGH); 
  gpio_hold_dis((gpio_num_t)MODEM_TX); 

  Serial.begin(115200);
  delay(100);
  Wire.begin(I2C_SDA, I2C_SCL);
  if (!rtc.begin()) Serial.println("❌ RTC No Encontrado");

  xTaskCreatePinnedToCore(TareaLecturaSensor, "TaskSensor", 4096, NULL, 1, &TareaSensorHandle, 1);
  
  pinMode(0, INPUT_PULLUP);
  if (digitalRead(0) == LOW || esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1) {
      Serial.println("!!! ENTRANDO A PORTAL POR PRESIÓN DE BOTÓN !!!");
      iniciarPortalMantenimiento();
  }

  esp_sleep_wakeup_cause_t causa_despertar = esp_sleep_get_wakeup_cause();

  switch (causa_despertar) {
    
    case ESP_SLEEP_WAKEUP_EXT1:
      break;

      case ESP_SLEEP_WAKEUP_EXT0:
      Serial.println(">>> MOVIMIENTO DETECTADO <<<");
      {
        unsigned long t_ultimo_pulso = millis();
        unsigned long t_inicio_jalon = millis(); 
        int conteo_anterior = contador_pulsos;

        Serial.println("Esperando más movimientos antes de transmitir...");

        while ((millis() - t_ultimo_pulso < 5000) && (millis() - t_inicio_jalon < 30000)) {
            esp_task_wdt_reset(); 
            if (contador_pulsos > conteo_anterior) {
                conteo_anterior = contador_pulsos;
                t_ultimo_pulso = millis(); 
                Serial.print("PULSO ACUMULADO: "); Serial.println(contador_pulsos);
            }
            delay(10); 
        }

        if (millis() - t_inicio_jalon >= 30000) {
            Serial.println("Límite de 30s alcanzado. Transmitiendo medida parcial...");
        } else {
            Serial.println("Calma detectada por 5 segundos. Procediendo a transmitir...");
        }
      }
      transmitirDatos(); 
      break;

    case ESP_SLEEP_WAKEUP_TIMER:
      Serial.println("Ciclo completado. Procesando datos...");
      transmitirDatos();
      break;

    default:
      Serial.println("\n--- ARRANQUE EN FRÍO ---");
      contador_pulsos = 0; dia_anterior = -1; registro_numero = 0; modem_encendido = true; 
      esperando_bloqueo = true; 
      Serial.println("Iniciando llave maestra (Pin 12 -> HIGH)...");
      gpio_hold_dis((gpio_num_t)PIN_MODEM_POWER); 
      pinMode(PIN_MODEM_POWER, OUTPUT);
      digitalWrite(PIN_MODEM_POWER, HIGH); 
      gpio_hold_en((gpio_num_t)PIN_MODEM_POWER);
      gpio_deep_sleep_hold_en(); 
      delay(10000);
      esp_task_wdt_reset(); 
      SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
      modem.init();
      modem.sendAT("+CTZU=1");
      modem.waitResponse(5000L);
      transmitirDatos();
      break;
  }
  
  if (TareaSensorHandle != NULL) vTaskDelete(TareaSensorHandle);

  pinMode(32, INPUT_PULLUP);
  rtc_gpio_pullup_en(PIN_ENCODER);
  rtc_gpio_pulldown_dis(PIN_ENCODER);

  if (esperando_bloqueo == true) {
      esp_sleep_enable_ext0_wakeup(PIN_ENCODER, 0);
  } else {
      esp_sleep_enable_ext0_wakeup(PIN_ENCODER, 1);
  }

  rtc_gpio_init(GPIO_NUM_0);
  rtc_gpio_set_direction(GPIO_NUM_0, RTC_GPIO_MODE_INPUT_ONLY);
  rtc_gpio_pullup_en(GPIO_NUM_0);
  rtc_gpio_pulldown_dis(GPIO_NUM_0);
  esp_sleep_enable_ext1_wakeup(BOTON_WIFI_MASK, ESP_EXT1_WAKEUP_ALL_LOW);
  esp_sleep_enable_timer_wakeup(TIEMPO_DORMIR_SEG * uS_TO_S_FACTOR);

  Serial.println("Entrando en Deep Sleep..."); Serial.flush(); 
  SerialAT.end();
  gpio_hold_en((gpio_num_t)PIN_MODEM_PWRKEY); 
  pinMode(MODEM_TX, OUTPUT);
  digitalWrite(MODEM_TX, HIGH); 
  gpio_hold_en((gpio_num_t)MODEM_TX); 
  esp_deep_sleep_start();
}

void loop() {}