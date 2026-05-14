# hardware-movimientos-en-masa
# 🔌 Hardware y electrónica - Nodo EMA

Este directorio contiene la especificación técnica de los componentes físicos y el esquema de conexiones desarrollado para el **Servicio Geológico Colombiano**. El diseño está optimizado para la detección de movimientos en masa mediante un sistema híbrido de alta precisión y bajo consumo.

## 📍 Análisis de componentes y conexiones (PinOut)

Cada terminal y periférico visualizado en el esquema de conexiones cumple una función vital para la autonomía y precisión del nodo:

### 1. Sensor óptico de herradura H206
* **Ubicación:** Conectado al **GPIO 32**.
* **Función:** Es el sensor principal de medición. Detecta la interrupción del haz infrarrojo al paso de las ranuras del disco encoder.
* **Detalle Técnico:** Configurado con una resistencia de **Pull-Up de 100kΩ** para estabilizar la señal y minimizar el consumo en *Deep Sleep*.

### 2. Unidad de procesamiento (LilyGO T-A7670G)
* **ESP32-WROVER-E:** Gestiona la lógica de control, el almacenamiento y la pila de protocolos.
* **Módem A7670G (Pines IO26/IO27):** Interfaz UART para el envío de telemetría GPRS/MQTT hacia el servidor central.
* **Control de Encendido (IO12/IO4):** Pines dedicados a la gestión de energía del módem, permitiendo apagarlo completamente cuando no hay transmisión.

### 3. Sincronización y tiempo (DS3231)
* **Interfaz:** Bus I2C (**SDA: IO21 / SCL: IO22**).
* **Función:** Garantiza que cada registro tenga un *timestamp* exacto, permitiendo la reconstrucción cronológica de los eventos geológicos.

### 4. Gestión energética solar
* **Panel solar:** 15W Modelo CL1615.
* **Lectura de Batería (IO35):** Entrada analógica (ADC) que permite al firmware monitorear el nivel de carga y reportarlo en la trama de datos para mantenimiento preventivo.

### 5. Almacenamiento redundante (MicroSD)
* **Interfaz:** Bus SPI (**CS: IO13**).
* **Función:** Respaldo físico de archivos CSV. Asegura que los datos no se pierdan si falla la cobertura celular.

## ⚙️ Especificaciones Mecánicas Integradas
Para asegurar la fidelidad del dato del sensor H206, el hardware se apoya en:
* **Eje:** Varilla lisa de acero de 8mm.
* **Soportes:** Rodamientos **KFL08** de alta eficiencia para un giro sin fricción.
* **Tracción:** Nylon trenzado de 8 hilos (elongación nula).

## 🛠️ Guía de Ensamble y Mantenimiento
1.  **Alineación Crítica:** El disco encoder debe estar perfectamente centrado en la herradura del sensor H206 para evitar rozamientos.
2.  **Protección Ambiental:** El hardware debe alojarse en la **Caja de paso 15x15 hermética** para proteger los circuitos de la humedad y el polvo.
3.  **Botón de Servicio (IO0):** Se utiliza para activar el portal Wi-Fi local sin necesidad de abrir el equipo en campo.

---
**Autor:** Jhoiner Andretty Silva Montaño  
**Entidad:** Servicio Geológico Colombiano  
**Año:** 2026