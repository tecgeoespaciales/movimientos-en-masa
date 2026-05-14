# 📚 Documentación técnica y manuales - nodo EMA

Este directorio centraliza la base de conocimiento del sistema de monitoreo de movimientos en masa. Aquí se encuentran los manuales de operación, las guías de referencia y los registros de control desarrollados para asegurar la correcta implementación y mantenimiento del nodo en campo.

## 📄 Recursos disponibles en esta carpeta

### 1. Manual de operación principal
* **Archivo:** `Manual_operacion_EMA.docx` (Anteriormente *PlantillaManualdeoperacin_1.docx*)
* **Contenido:** Guía exhaustiva que cubre desde los objetivos del proyecto hasta los protocolos de instalación. Es el documento maestro para entender el funcionamiento del hardware y software.

### 2. Guía de calibración y resolución
De acuerdo con las pruebas de ingeniería documentadas, el sistema utiliza los siguientes parámetros para la conversión de datos mecánicos a digitales:
* **Resolución:** 0.801 cm por cada pulso del sensor H206.
* **Equivalencia:** 125 pulsos corresponden a 100 cm (1 metro) de desplazamiento.
* **Componente crítico:** Carrete calibrador de 5.10 cm de diámetro.

### 3. Control de versiones y cambios
* **Archivo:** `Control_de_Cambios.png` (Referencia visual)
* **Uso:** Registro histórico de las actualizaciones realizadas en el firmware, mejoras en el diseño de las piezas 3D y ajustes en los protocolos de comunicación MQTT.

## 🛠️ Referencias técnicas complementarias
Para profundizar en los submódulos del nodo, se recomienda consultar las siguientes referencias mencionadas en el manual:
* **Hardware:** LilyGO T-A7670 Series hardware manual.
* **Comandos AT:** SIMCom A7670 Series command manual (Envío de tramas CSV).
* **Sensor:** Datasheet genérico H206.

## 📊 Gestión y visualización
La documentación describe el flujo de datos hacia la interfaz de **Node-RED**, detallando la configuración de los dashboards para la visualización de desplazamientos y niveles de batería reportados por el nodo.

---
**Autor:** Jhoiner Andretty Silva Montaño  
**Entidad:** Servicio Geológico Colombiano  
**Año:** 2026