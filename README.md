# 🌍 Proyecto EMA - Estación de Monitoreo de Movimientos en Masa

Este proyecto, desarrollado por **Jhoiner Silva** para el **Servicio Geológico Colombiano (SGC)**, consiste en un sistema de instrumentación geotécnica diseñado para la detección temprana de desplazamientos de terreno. El nodo utiliza tecnología IoT para transformar variables físicas en datos telemáticos, permitiendo la gestión de riesgos en tiempo real.

## 🚀 propósito del proyecto

El sistema EMA tiene como objetivo proporcionar una solución robusta y de bajo costo para el monitoreo de laderas, utilizando un sensor óptico de herradura **H206** y un microcontrolador **ESP32 (LilyGO T-A7670G)** con conectividad LTE.

---

## 🛠️ Modalidades de uso

Para garantizar la versatilidad del sistema, el proyecto se divide en dos enfoques operativos:

### 1. Perfil profesional (Uso institucional SGC)
Este enfoque está diseñado para el despliegue oficial en estaciones de monitoreo críticas.
* **Telemetría avanzada:** Transmisión de tramas de datos mediante el protocolo **MQTT** hacia un Broker centralizado.
* **Respaldo redundante:** Almacenamiento local en MicroSD y memoria interna LittleFS en formato CSV.
* **Gestión de energía:** Optimización mediante *Deep Sleep* y carga solar para autonomía indefinida.
* **Sincronización:** Uso de RTC DS3231 para estampa de tiempo de alta precisión.

### 2. Perfil comunitario (Gestión social del riesgo)
Este enfoque busca empoderar a las comunidades locales mediante la tecnología.
* **Interfaz de usuario:** Activación de un **Portal Web Local** (Modo Extracción) mediante el botón de servicio (Pin 0).
* **Acceso directo:** Permite que líderes comunitarios descarguen datos directamente al celular vía Wi-Fi sin depender de internet o servidores externos.
* **Mantenimiento simplificado:** Diseñado para ser operado por personal no técnico mediante guías de interpretación visual de los datos.

---

## 📂 Estructura del repositorio

* **[💾 Firmware](./Firmware/):** Código fuente en C++/Arduino. Es el "cerebro" que gestiona la lógica de medición y ahorro de energía.
* **[🔌 Hardware](./Hardware/):** Esquemas de conexión (PinOut) y especificaciones de componentes electrónicos como el sensor H206.
* **[⚙️ 3D_Parts](./3D_Parts/):** Archivos STL para impresión. Combina piezas en **ABS** para estructura y **Resina** para precisión mecánica.
* **[📚 Docs](./Docs/):** Manuales técnicos, formatos de registro de campo y control de cambios.

## 📊 Especificaciones técnicas
* **Resolución:** 0.801 cm por pulso (125 pulsos = 1 metro).
* **Conectividad:** GPRS/LTE y Wi-Fi (Modo AP).
* **Procesamiento:** Arquitectura Dual-Core con FreeRTOS.

---
**Autor:** Jhoiner Andretty Silva Montaño  
**Entidad:** Servicio Geológico Colombiano  
**Año:** 2026