# 💾 Firmware EMA - Sistema de Monitoreo de Movimientos en Masa

Este directorio contiene el desarrollo lógico integral para el microcontrolador **ESP32 (LilyGO T-A7670G)**. El firmware ha sido diseñado para garantizar la captura precisa de desplazamientos superficiales y su transmisión telemática bajo condiciones ambientales críticas.

## ❓ ¿Para qué sirve esta carpeta?
Esta carpeta es el **núcleo del sistema**. El firmware aquí alojado actúa como el "cerebro" del nodo EMA, encargándose de:
* **Interpretar el movimiento:** Traduce las señales físicas del sensor H206 en datos métricos de desplazamiento.
* **Gestión inteligente:** Controla cuándo el equipo debe "dormir" para ahorrar batería y cuándo debe "despertar" para reportar una alerta.
* **Puente de comunicación:** Transforma los datos recolectados en tramas MQTT para enviarlas vía satelital/celular al centro de datos del SGC.
* **Seguridad de datos:** Administra el respaldo local (MicroSD/LittleFS) para que nunca se pierda información por falta de señal.

## 🏗️ Arquitectura del software (FreeRTOS)
Para garantizar que no se pierda ningún pulso del encoder durante las tareas de comunicación, el firmware utiliza una arquitectura de doble núcleo:
* **Core 1 (Prioridad Crítica):** Ejecuta la tarea de vigilancia del sensor óptico H206. Detecta flancos de subida y bajada en el Pin 32 en tiempo real.
* **Core 0 (Gestión de Red):** Administra la pila TCP/IP, los comandos AT del módem A7670G y la sesión MQTT.

## ⚡ Gestión de energía y eficiencia
El sistema está optimizado para funcionar con un panel solar de 15W y una batería de respaldo:
* **Modo Deep Sleep:** El ESP32 entra en sueño profundo, reduciendo el consumo a microamperios.
* **Wake-up (EXT0):** El sistema despierta instantáneamente ante cualquier rotación del disco encoder.
* **Wake-up (Timer):** Reporte de estado de batería y señal cada hora.
* **Hardware:** Se implementa una resistencia de pull-up de 100kΩ para minimizar fugas de corriente en reposo.

## 🔐 Configuración de seguridad (Paso a Paso)
El firmware utiliza el archivo `config_env.h` para gestionar credenciales sensibles sin exponerlas en el historial de Git. Siga estas instrucciones para compilar:

1. En esta carpeta, cree un archivo nuevo llamado exactamente `config_env.h`.
2. Copie y pegue la siguiente estructura:

```cpp
#ifndef CONFIG_ENV_H
#define CONFIG_ENV_H

// --- CREDENCIALES GPRS (APN) ---
#define SECRET_APN        "internet.comcel.com.co" 
#define SECRET_GPRS_USER  ""
#define SECRET_GPRS_PASS  ""

// --- CONFIGURACIÓN BROKER MQTT ---
#define SECRET_MQTT_SERVER "00.000.000.0"   // IP Servidor a usar
#define SECRET_MQTT_PORT   1883
#define SECRET_MQTT_USER   "tu_usuario"
#define SECRET_MQTT_PASS   "tu_clave"
#define SECRET_MQTT_TOPIC  "movimientos_masa/final"

// --- ACCESO TÉCNICO LOCAL ---
#define SECRET_WIFI_AP_PASS "12345678" 

#endif


//-- AUTOR --
//**Autor:** Jhoiner Andretty Silva Montaño  
//**Entidad:** Servicio Geológico Colombiano  
//**Año:** 2026