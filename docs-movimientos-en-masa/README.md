# 📚 Documentación Técnica y Manuales - Nodo EMA

Este directorio centraliza la base de conocimiento del Sistema de Monitoreo de Movimientos en Masa. Aquí se encuentran los manuales de operación, las guías de referencia y los registros de control desarrollados para asegurar la correcta implementación y mantenimiento del nodo en campo.

## 📄 Recursos Disponibles en esta Carpeta

### 1. Manual de Operación Principal
* **Archivo:** `Manual_Operacion_EMA.docx` (Anteriormente *PlantillaManualdeoperacin_1.docx*)
* **Contenido:** Guía exhaustiva que cubre desde los objetivos del proyecto hasta los protocolos de instalación. Es el documento maestro para entender el funcionamiento del hardware y software.

### 2. Guía de Calibración y Resolución
De acuerdo con las pruebas de ingeniería documentadas, el sistema utiliza los siguientes parámetros para la conversión de datos mecánicos a digitales:
* **Resolución:** 0.801 cm por cada pulso del sensor H206.
* **Equivalencia:** 125 pulsos corresponden a 100 cm (1 metro) de desplazamiento.
* **Componente Crítico:** Carrete calibrador de 5.10 cm de diámetro.

### 3. Control de Versiones y Cambios
* **Archivo:** `Control_de_Cambios.png` (Referencia visual)
* **Uso:** Registro histórico de las actualizaciones realizadas en el firmware, mejoras en el diseño de las piezas 3D y ajustes en los protocolos de comunicación MQTT.

## 🛠️ Referencias Técnicas Complementarias
Para profundizar en los submódulos del nodo, se recomienda consultar las siguientes referencias mencionadas en el manual:
* **Hardware:** LilyGO T-A7670 Series Hardware Manual.
* **Comandos AT:** SIMCom A7670 Series Command Manual (Envío de tramas CSV).
* **Sensor:** Datasheet Genérico H206.

## 📊 Gestión y Visualización
La documentación describe el flujo de datos hacia la interfaz de **Node-RED**, detallando la configuración de los dashboards para la visualización de desplazamientos y niveles de batería reportados por el nodo.

---
**Autor:** Jhoiner Andretty Silva Montaño  
**Entidad:** Servicio Geológico Colombiano  
**Año:** 2026