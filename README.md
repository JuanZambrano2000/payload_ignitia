## README.md

**Proyecto: Medición de Radiación durante Vuelo**

**Grupo:** IGNITIA ITESM GDL

**Objetivo:**

Este proyecto tiene como objetivo desarrollar un payload para un Cohete para medir la radiación durante un vuelo. La información recopilada se almacenará en una tarjeta SD para su posterior análisis e investigación.

**Componentes:**

* **Hardware:**
    * Arduino Leonardo
    * Contador Geiger RadiationD-v1.1 (CAJOE)
    * Tarjeta SD
    * Sensores adicionales (opcional)
* **Software:**
    * Código Arduino para la adquisición de datos y guardado en tarjeta

**Funcionamiento:**

1. El Contador Geiger detecta la radiación y envía señales al Arduino.
2. El Arduino procesa las señales y calcula la tasa de conteo por minuto.
3. La tasa de conteo se guarda en la tarjeta SD.
4. Después del vuelo, la información se recupera de la tarjeta SD y se analiza.

**Aplicaciones:**

* **Investigación científica:** La información recopilada puede ser utilizada para estudiar la distribución de la radiación en la atmósfera, los efectos de la radiación en los seres vivos, etc.
* **Educación:** El proyecto puede ser utilizado como herramienta educativa para enseñar sobre la radiación y sus efectos.

**Contribuciones:**

Este proyecto es una colaboración entre los miembros del grupo IGNITIA ITESM GDL. Cada miembro ha contribuido al proyecto de diferentes maneras:

* **Desarrollo del software:**
    * **Juan Pablo Zambrano Barajas**
    * **Stefano Herrejon Antuñano**

**Documentación:**

Este README.md proporciona una descripción general del proyecto. Para obtener más información, consulte la siguiente documentación:

* **Sensor de radiación:** ![Imagen del sensor RadiationD-v1.1 (CAJOE)](RadiationD-v1.1 (CAJOE).jpg)
* **Código Arduino:** [Enlace al código]
* **Script para el análisis de datos:** [Enlace al script]
