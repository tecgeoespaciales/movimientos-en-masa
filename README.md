# Estación de Monitoreo Automática (EMA) - Movimientos en Masa 🛰️⛰️

> Espacio de trabajo para la rama **profesional** del dispositivo **movimientos-en-masa**, desarrollado bajo los estándares técnicos del **Servicio Geológico Colombiano (SGC)**.

## 📂 Módulos del Proyecto
Para una navegación detallada por los componentes del sistema, acceda a los siguientes módulos:

- 📑 [docs-movimientos-en-masa](./docs-movimientos-en-masa/README.md) - Manuales técnicos, de operación y seguridad.
- 🔌 [hardware-movimientos-en-masa](./hardware-movimientos-en-masa/README.md) - Esquemas electrónicos, PinOut y listado de componentes.
- 💻 [firmware-movimientos-en-masa](./firmware-movimientos-en-masa/README.md) - Código fuente para ESP32 con cifrado AES y protocolos SSL.
- ⚙️ [mechanical-movimientos-en-masa](./mechanical-movimientos-en-masa/README.md) - Archivos STL para impresión 3D en resina ABS.

---

## 📝 Descripción General
La **EMA (Estación de Monitoreo Automática)** es un sistema IoT de alta precisión diseñado para la detección temprana de deslizamientos. Utiliza un sensor óptico acoplado a un mecanismo de carrete y nylon para medir desplazamientos del terreno, transmitiendo datos en tiempo real mediante tecnología LTE Cat-1.

## 🛠️ Especificaciones Técnicas
* **Core:** ESP32-WROVER-B (LilyGO T-A7670G).
* **Comunicación:** Módem SIMCom A7670G (Protocolo MQTT).
* **Sensor:** Encoder de herradura infrarrojo H206 (125 pulsos por metro).
* **Sincronización:** RTC DS3231 para estampado de tiempo preciso.
* **Energía:** Panel solar 15W + Batería Litio 18650.
* **Materiales:** Carrete y guías impresos en **Resina ABS** de alta resistencia.

## 📐 Metodología de Medición
La distancia de desplazamiento ($D$) se obtiene mediante el conteo de pulsos ($P$):
$$D = P \times 0.8000 \, \text{cm}$$

---

## 🔐 Seguridad y Telemetría
El sistema garantiza la integridad de los datos para el SGC mediante:
1. **Capa de Transporte:** Conexión cifrada **SSL/TLS (Puerto 8883)** hacia EMQX Cloud.
2. **Capa de Aplicación:** Encriptación **AES-128** de la carga útil (Payload) antes de la transmisión.

---

## 🚀 Guía de Operación Paso a Paso

### 1. Preparación
* Instalar **Nano SIM** (Claro) y **MicroSD** (FAT32).
* Cargar batería 18650 (voltaje > 3.7V).

### 2. Calibración e Instalación
* Fijar la caja hermética 15x15 en el sitio de monitoreo.
* Enrollar el nylon en el carrete y alinearlo con el sensor H206.
* Anclar el extremo del nylon al punto de referencia en el terreno.

### 3. Puesta en Marcha
* Encender el interruptor. El sistema iniciará automáticamente el registro en red y la conexión MQTT.
* Verificar en el Dashboard (Node-RED) la recepción del primer paquete.

### 4. Descarga de Datos (Modo Local)
* En zonas sin cobertura, conectarse al Wi-Fi `EMA_SGC_CONFIG`.
* Acceder a `192.168.4.1` desde un navegador para descargar el archivo `LOGS.CSV`.

---

## ⚠️ Mantenimiento
* Limpiar el panel solar mensualmente.
* Revisar la tensión del nylon trenzado cada 3 meses.

---
**Autor:** Jhoiner Andretty Silva Montaño  
**Entidad:** Servicio Geológico Colombiano  
**Año:** 2026
