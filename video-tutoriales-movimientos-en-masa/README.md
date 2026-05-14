# 📥 Guía de extracción de datos (Modo Comunitario / Técnico)

Este documento describe el protocolo para la descarga de datos históricos directamente desde el nodo **EMA** sin necesidad de conexión a internet, utilizando el servidor web local integrado en el firmware.

## 🎥 Video demostrativo
*(En esta carpeta se encuentra el archivo `VIDEO-EXTRACCION.mp4` que ilustra los pasos descritos a continuación).*

## 🛠️ Procedimiento de extracción paso a paso

### 1. Activación del modo AP (punto de acceso)
Para despertar el Wi-Fi del nodo, localice el orificio de servicio en la parte frontal del chasis:
* Inserte una herramienta fina para presionar el botón de **BOOT (Pin 0)** durante 2 segundos.
* El nodo detendrá temporalmente la telemetría MQTT para levantar el servidor web local.

### 2. Conexión al nodo
Desde un smartphone o computadora, busque las redes Wi-Fi disponibles:
* **SSID:** `Masa_Prototipo`
* **Contraseña:** `12345678` (Configuración por defecto).

### 3. Acceso a la interfaz de usuario
Abra su navegador web (Chrome, Safari o Firefox) e ingrese la dirección IP estática:
* **IP:** `192.168.4.1`

### 4. Descarga de archivos
En el panel de control web encontrará las siguientes opciones:
* **Visualización:** Monitoreo del desplazamiento acumulado y voltaje de batería.
* **Descarga SD:** Permite bajar el archivo `DATA.CSV` almacenado en la MicroSD.
* **Descarga interna:** respaldo almacenado en la memoria LittleFS (Redundancia).

### 5. Finalización y reposo
**IMPORTANTE:** Una vez terminada la descarga, presione el botón **"Apagar Wi-Fi y Dormir"** en la interfaz web. Esto asegura que el nodo cierre el servidor y vuelva a su estado de bajo consumo para continuar con la vigilancia geológica.

## 📊 Formato de datos
Los archivos descargados están en formato `.CSV o .TXT` (valores separados por comas), compatibles con Excel o software de análisis de datos, con la siguiente estructura:
`fecha, hora, desplazamiento(m), batería(V), señal(dBm)`

---
**Desarrollado por:** Jhoiner Andretty Silva Montaño  
**Entidad:** Servicio Geológico Colombiano  
**Año:** 2026