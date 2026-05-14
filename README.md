# Estación de monitoreo automática (EMA) - Movimientos en Masa 🛰️⛰️

> Espacio de trabajo para la rama **profesional** del dispositivo **movimientos-en-masa**, desarrollado bajo los estándares técnicos del **Servicio Geológico Colombiano (SGC)**.

## 📂 Módulos del proyecto
Para una navegación detallada por los componentes del sistema, acceda a los siguientes módulos:

📑 **[docs-movimientos-en-masa](./Docs/)** – Manuales técnicos, de operación y seguridad.
* 🔌 **[hardware-movimientos-en-masa](./Hardware/)** – Esquemas electrónicos, PinOut y listado de componentes.
* 💻 **[firmware-movimientos-en-masa](./Firmware_ESP32/)** – Código fuente para ESP32 con protocolos de comunicación segura.
* ⚙️ **[mechanical-movimientos-en-masa](./Mechanical/)** – Archivos STL para impresión 3D en resina y ABS.
* 🎬 **[video-tutoriales-movimientos-en-masa](./Videos/)** – Guías visuales paso a paso para la operación y extracción de datos en campo.
---

## 📝 Descripción general
La **EMA (Estación de Monitoreo Automática)** es un sistema IoT de alta precisión diseñado para la detección temprana de deslizamientos. Utiliza un sensor óptico acoplado a un mecanismo de carrete y nylon para medir desplazamientos del terreno, transmitiendo datos en tiempo real mediante tecnología LTE.

## 🛠️ Especificaciones técnicas
* **Microcontrolador:** ESP32-WROVER-B (LilyGO T-A7670G).
* **Comunicación:** Módem SIMCom A7670G (Protocolo MQTT).
* **Sensor:** Encoder de herradura infrarrojo H206 (125 pulsos por metro).
* **Estampa:** RTC DS3231 para estampado de tiempo preciso.
* **Energía:** Panel solar 15W + batería Litio 18650.
* **Materiales:** Carrete y guías impresos en **resina ABS** de alta resistencia.

## 📐 Metodología de medición
La distancia de desplazamiento ($D$) se obtiene mediante el conteo de pulsos ($P$):
$$D = P \times 0.8000 \, \text{cm}$$

---

## 🔐 Seguridad y telemetría
El sistema garantiza la integridad de los datos para el SGC mediante:
1. **Capa de transporte:** Conexión cifrada **TLS (Puerto 1883)** hacia MQTT.

---

## 🚀 Guía de operación paso a paso

### 1. Preparación
* Instalar **Nano SIM** (Claro u operador de preferencia) y **MicroSD** (FAT32).
* Cargar batería 18650 (voltaje > 3.7V).

### 2. Calibración e instalación
* Fijar la caja hermética 15x15 en el sitio de monitoreo.
* Enrollar el nylon en el carrete y alinearlo con el sensor H206.
* Anclar el extremo del nylon al punto de referencia en el terreno.

### 3. Puesta en marcha
* Encender el interruptor. El sistema iniciará automáticamente el registro en red y la conexión MQTT.
* Verificar en el Dashboard (Node-RED) la recepción del primer paquete.

### 4. Descarga de datos (Modo local)
* En zonas sin cobertura, conectarse al Wi-Fi `EMA_SGC_CONFIG`.
* Acceder a `192.168.4.1` desde un navegador para descargar el archivo `LOGS.CSV`.

---

## ⚠️ Mantenimiento
* Limpiar el panel solar cada 3 meses, o antes de ser necesario.
* Revisar la tensión del nylon trenzado cada 3 meses , o antes de ser necesario.

---
**Autor:** Jhoiner Andretty Silva Montaño  
**Entidad:** Servicio Geológico Colombiano  
**Año:** 2026
