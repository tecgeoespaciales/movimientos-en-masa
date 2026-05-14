# ⚙️ Especificaciones de fabricación mecánica (híbrido ABS - resina)

Este directorio contiene los archivos de diseño industrial para el ensamble del nodo de monitoreo **EMA - Movimientos en masa**. El sistema utiliza una combinación estratégica de materiales para garantizar precisión milimétrica y durabilidad estructural en campo.

## 📂 Inventario de piezas (.STL)

1.  **`CARRETE-CALIBRADOR.stl`**: Componente principal de medición. Su diámetro de 5.10 cm está optimizado para una resolución de 0.801 cm por pulso.
    * *Relación de campo:* 125 pulsos equivalen a 100 cm de desplazamiento lineal.
2.  **`ENCODER-8MM.stl`**: Acople de precisión diseñado para vincular el eje del encoder de 8mm con el sistema de rotación sin juegos mecánicos.
3.  **`CARRETE-NYLON.stl`**: Carrete de alta resistencia para el almacenamiento del hilo trenzado de 8 hilos.
4.  **`SOPORTE-H206.stl`**: Estructura de anclaje que garantiza el alineamiento perfecto entre el sensor óptico y el disco encoder.

## 🛠 Estándar de manufactura

Para asegurar el correcto funcionamiento del nodo bajo condiciones ambientales reales, se establecen los siguientes parámetros:

### 1. Componentes estructurales (ABS - FDM)
* **Material:** ABS (acrilonitrilo butadieno estireno).
* **Piezas:** Chasis, soportes y carrete de nylon.
* **Configuración:** Relleno al 60% (Giroide) y mínimo 4 capas de pared.
* **Propiedades:** Alta resistencia al impacto y estabilidad térmica hasta 60°C.

### 2. Componentes de Precisión (resina ABS - SLA/DLP)
* **Material:** Resina de ingeniería (Tough o ABS).
* **Piezas:** `CARRETE-CALIBRADOR` y `ENCODER-8MM`.
* **Propiedades:** Acabado superficial liso que minimiza la fricción y garantiza la fidelidad del conteo.

## 🔧 Notas de Ensamble y Mantenimiento

* **Alineación:** El sensor H206 debe estar centrado con las ranuras del disco para evitar errores de lectura por vibración o desajuste.
* **Fijación:** Se requiere tornillería M3 para el aseguramiento de los componentes al chasis.
* **Limpieza:** Mantener el disco encoder libre de polvo. Utilizar alcohol isopropílico únicamente para la limpieza del sensor óptico.

---
*Diseño y desarrollo técnico por: **Jhoiner Andretty Silva Montaño***