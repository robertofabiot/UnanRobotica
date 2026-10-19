# Guía de Construcción DIY: Maqueta "Hogar Inteligente" Portátil
**Proyecto:** Prototipo 3D y Físico de Smart Home Educativa  
**Concepto:** Bricolaje (DIY), Materiales Reciclados y Máxima Portabilidad (Transportable en Mochila)  
**Institución / Contexto:** UNAN Robótica / Prototipado Rápido y Robótica Educativa  

---

## 1. Filosofía de Diseño y Portabilidad

El objetivo de esta maqueta es brindar una plataforma de pruebas y demostración de **Internet de las Cosas (IoT)** y **Domótica** que sea:
1. **Económica y Sostenible:** Construida con cartón corrugado recuperado (cajas de embalaje), botellas plásticas PET descartadas y palitos de madera (bajalenguas o palitos de helado).
2. **Altamente Portátil:** Sus dimensiones exteriores de **30 cm de largo x 22 cm de ancho x 10 cm de alto** permiten transportarla dentro de cualquier mochila estándar escolar o universitaria sin riesgo de que se desprendan cables o sensores.
3. **Mantenimiento Cero en Tránsito (Sistema de Doble Fondo):** Un zócalo técnico inferior de **2 cm de altura** aloja la protoboard, el microcontrolador ESP32/ESP8266 y el cableado DuPont. Los sensores se conectan a través de ojales pasacables directos hacia el falso fondo, eliminando cables sueltos en el área habitable.
4. **Visualización Abierta (Open-Ceiling):** Formato arquitectónico sin techo que permite al docente, evaluador o público observar el funcionamiento simultáneo de la domótica interior (luces LED, climatización, alarma) y exterior (garaje inteligente y radar ultrasónico).

---

## 2. Lista de Materiales y Herramientas

### 2.1 Materiales Reciclados y Estructurales
| Elemento | Cantidad Sugerida | Origen / Función |
| :--- | :--- | :--- |
| **Cartón corrugado simple (Onda B o C, e ≈ 3.5 mm)** | 2 a 3 cajas medianas limpias | Base inferior, piso habitable, paredes perimetrales e interiores, escuadras de refuerzo |
| **Palitos de helado / bajalenguas (madera)** | 20 a 25 unidades | Marcos de puertas y ventanas, barrera levadiza del garaje, guías de encastre |
| **Plástico PET transparente (Botellas de gaseosa/agua)** | 1 botella lisa de 1.5L - 2L | Cristal de ventanas en Dormitorio y Cocina |
| **Silicona caliente (barras) o pegamento blanco de carpintero** | 4 a 6 barras | Fijación rápida y adhesión de refuerzos mecánicos |
| **Cinta de carrocero / papel kraft engomado** | 1 rollo | Opcional para sellar cantos expuestos |

### 2.2 Componentes Electrónicos
| Componente | Cantidad | Ubicación en la Maqueta | Función Domótica |
| :--- | :--- | :--- | :--- |
| **Placa de Desarrollo ESP32 (30 pines) o ESP8266 NodeMCU** | 1 | Doble fondo técnico | Cerebro WiFi, servidor web y control IoT |
| **Mini Protoboard (400 puntos) o Shield de expansión** | 1 | Doble fondo técnico | Distribución de líneas 5V, 3.3V y GND |
| **Micro Servomotor SG90 (9g)** | 1 | Entrada del Garaje (esquina frontal derecha) | Accionamiento mecánico de la barrera levadiza |
| **Módulo Lector RFID RC522 (13.56 MHz) + Tarjeta/Llavero** | 1 | Muro exterior lateral derecho del Garaje | Control de acceso seguro vehicular |
| **Sensor de Distancia Ultrasónico HC-SR04** | 1 | Fachada frontal de la Sala (ojos hacia afuera) | Radar detector de intrusos / sistema antirrobo |
| **Buzzer Piezoeléctrico (Activo o Pasivo, 5V)** | 1 | Pared central interna divisoria | Alarma sonora antirrobo y confirmación RFID |
| **Sensor de Temperatura y Humedad DHT11** | 1 | Pared interna de la Sala (tras rejilla de cartón) | Climatización inteligente y monitoreo ambiental |
| **LEDs difusos de 5mm (Blanco cálido, Amarillo, Azul, Verde)** | 4 | 1 en cada habitación (en soporte superior de pared) | Iluminación independiente por zona vía Web |
| **Resistencias de 220 Ω (para LEDs) y 10 kΩ (para pull-up)** | 4x 220Ω, 1x 10kΩ | Protoboard en doble fondo | Protección de LEDs y estabilización de señales |
| **Cables DuPont (Macho-Hembra y Macho-Macho)** | ~25 a 30 | Pasacables hacia el doble fondo | Enlace seguro entre sensores y ESP32 |
| **Power Bank USB 5V o Portapilas (4x AA con regulador)** | 1 | Doble fondo o conexión posterior USB | Alimentación autónoma portátil |

### 2.3 Herramientas de Taller
- Cúter / bisturí de precisión con hoja afilada.
- Regla metálica de 30 o 50 cm.
- Escuadra de carpintero o técnica a 90°.
- Tijeras fuertes para plástico PET.
- Pistola de silicona termofusible.
- Sacabocados o broca manual de 16 mm (para los ojos del HC-SR04) y 5 mm (para los LEDs).
- Lápiz grafito y marcador indeleble fino.

---

## 3. Planos de Despiece y Dimensiones Exactas

```
                     VISTA EN PLANTA (30 cm x 22 cm)
  -15 cm                                                          +15 cm
    +------------------------------+--------------------------------+  -11 cm
    |                              |                                |
    |          DORMITORIO          |            COCINA              |
    |         (15 cm x 11 cm)      |        (15 cm x 11 cm)         |
    |                              |                                |
    | [Ventana PET Izq]            |               [Ventana PET]    |
    |                              |                                |
    +-----------------[Puerta]-----+---------[Buzzer Central]-------+   0 cm
    |                              |                                |
    |             SALA             |            GARAJE              |
    |         (18 cm x 11 cm)      |        (12 cm x 11 cm)         |
    |                              |                                |
    | [DHT11 + Rejilla]            |                 [RFID RC522]   |
    |                              |              (Muro Lateral Ext)|
    | [HC-SR04 Fachada]  [Puerta]  |    [Barrera Madera + SG90]     |
    +------------------------------+--------------------------------+  +11 cm
    <------- Fachada Frontal (30 cm de ancho total) --------------->
```

### 3.1 Lista de Cortes de Cartón Corrugado

1. **Placa Base Inferior (Suelo Exterior de Apoyo):**
   - 1 pieza de **30.0 cm x 22.0 cm** (Onda longitudinal para resistir flexión).
2. **Placa de Piso Habitable (Suelo Intermedio - Techo del Doble Fondo):**
   - 1 pieza de **29.3 cm x 21.3 cm** (Encaja dentro del perímetro de los zócalos de 2 cm).
   - Incluye 6 orificios pasacables circulares de 8 mm de diámetro ubicados debajo de cada sensor.
3. **Zócalos Laterales del Doble Fondo (Altura = 2.0 cm):**
   - 2 piezas de **30.0 cm x 2.0 cm** (Frontal y Posterior inferior).
   - 2 piezas de **21.3 cm x 2.0 cm** (Laterales izquierdo y derecho inferior).
   - *Ventana técnica:* La pieza posterior lleva un calado de 1.5 cm x 0.8 cm para dar paso al conector microUSB/USB-C del ESP32.
4. **Paredes Perimetrales Superiores (Altura = 8.0 cm):**
   - **Pared Frontal (Fachada):**
     - Tramo Sala: **18.0 cm x 8.0 cm**. Calados:
       - Puerta principal: 4.0 cm (ancho) x 6.5 cm (alto).
       - Perforaciones HC-SR04: 2 círculos de 16 mm de diámetro, separados a 2.6 cm de centro a centro, a 3.5 cm de altura sobre el suelo habitable.
     - Tramo Garaje: Vano libre de 12.0 cm con dintel de dintel superior de 1.5 cm de cartón reforzado con palito de madera.
   - **Pared Posterior:**
     - 1 pieza de **30.0 cm x 8.0 cm**. Calado para ventana de Cocina: 6.0 cm x 4.0 cm a 2.5 cm de altura.
   - **Pared Lateral Izquierda (Dormitorio/Sala exterior):**
     - 1 pieza de **22.0 cm x 8.0 cm**. Calado para ventana de Dormitorio: 6.0 cm x 4.0 cm.
   - **Pared Lateral Derecha (Garaje/Cocina exterior):**
     - 1 pieza de **22.0 cm x 8.0 cm**. Soporte empotrado para el lector RFID-RC522 en la zona frontal del garaje.
5. **Paredes Divisorias Interiores (Altura = 8.0 cm):**
   - **Pared Central Transversal (Eje Z = 0):**
     - Tramo izquierdo (Sala - Dormitorio): **15.0 cm x 8.0 cm** con vano de puerta de 3.5 cm x 6.5 cm.
     - Tramo derecho (Garaje - Cocina): **15.0 cm x 8.0 cm** con pestaña de encastre y orificio central para el buzzer piezoeléctrico.
   - **Pared Divisoria Longitudinal (Eje X = +3 cm en frente, X = 0 cm en fondo):**
     - Tabique Garaje - Sala: **11.0 cm x 8.0 cm**. En la cara interna de la Sala se cala la rejilla de ventilación para el sensor DHT11.
     - Tabique Dormitorio - Cocina: **11.0 cm x 8.0 cm**.
6. **Escuadras de Refuerzo Triangular (Gussets):**
   - 12 a 16 triángulos rectángulos de cartón de **2.5 cm x 2.5 cm** en los catetos, pegados en las esquinas internas y zócalos para evitar que las paredes se doblen al manipular la maqueta.

---

## 4. Ensamblaje Paso a Paso

### Fase 1: Fabricación del Módulo de Doble Fondo (2 cm)
1. Coloca la **Placa Base Inferior (30 x 22 cm)** sobre tu mesa de trabajo.
2. Pega con silicona caliente los 4 listones perimetrales de 2 cm de altura sobre los bordes de la base.
3. Agrega 2 pequeños tacos de cartón de 2 cm de alto en el centro para soportar el peso de los tabiques superiores sin que el piso habitable se hunda.
4. En el fondo del compartimento, fija con cinta doble cara la Mini Protoboard y el ESP32.
5. Traza los orificios pasacables en la **Placa de Piso Habitable (29.3 x 21.3 cm)** haciéndolos coincidir con las paredes donde subirán los cables:
   - Punto 1: Esquina Sala (LED Sala).
   - Punto 2: Fachada Sala (HC-SR04).
   - Punto 3: Tabique Sala (DHT11).
   - Punto 4: Esquina Dormitorio (LED Dormitorio).
   - Punto 5: Esquina Cocina (LED Cocina).
   - Punto 6: Tabique Central (Buzzer).
   - Punto 7: Esquina Garaje (LED Garaje y Servomotor SG90).
   - Punto 8: Muro lateral Garaje (RFID-RC522).
6. Deja este piso habitable desmontable o fijado con 4 tornillos pequeños para madera o velcro adhesivo; así podrás abrir el falso fondo en caso de querer cambiar el circuito.

### Fase 2: Carpintería DIY con Palitos de Madera y Plástico PET
1. **Ventanas de Dormitorio y Cocina:**
   - Corta dos rectángulos de plástico de botella PET limpia y desengrasada de **7.0 cm x 5.0 cm**.
   - Con palitos de helado, corta 4 tiras para formar el marco perimetral y 2 tiras delgadas para la cruceta central.
   - Pega el plástico PET por detrás del marco de madera usando una fina película de pegamento o silicona caliente líquida.
   - Pega el conjunto en los calados de las paredes exteriores de cartón.
2. **Marcos de Puertas:**
   - Enmarca los vanos de las puertas con tiras de palitos de helado cortados a 45 grados en las esquinas para brindar acabado profesional y resistencia estructural a los bordes de cartón.
3. **Barrera Levadiza del Garaje:**
   - Selecciona un palito de helado recto de 11 cm de largo. Píntalo con franjas diagonales rojas y blancas (marcador) para simular una pluma de control vehicular.
   - Pega el extremo del palito al cuerno o aspa de plástico de un solo brazo del servomotor SG90 usando pegamento instantáneo (cianoacrilato) o silicona.

### Fase 3: Montaje e Integración de Sensores y Actuadores

#### A. Garaje (Servomotor SG90 + RFID-RC522)
- **Servomotor SG90:** Se fija con su soporte de plástico o una abrazadera de cartón y palito en el pilar interior izquierdo del portal del garaje, a 3 cm del suelo. El eje debe orientarse hacia el frente para que la barrera levante de 0° (horizontal, cerrado) a 90° (vertical, abierto).
- **Lector RFID-RC522:** Se fija en la cara exterior del muro lateral derecho del garaje. Al ser un módulo plano con antena impresa en PCB, se fabrica un pequeño marco de cartón de 2 mm que proteja sus pistas y deje expuesta la serigrafía para que el usuario acerque la tarjeta o llavero de forma intuitiva. Los 8 pines de conexión pasan hacia el interior mediante una ranura oculta y bajan al falso fondo.

#### B. Sistema Antirrobo (Sensor Ultrasónico HC-SR04 + Buzzer)
- **HC-SR04 en Fachada:** Los dos transductores cilíndricos de aluminio (emisor "T" y receptor "R") se insertan a presión en los dos orificios de 16 mm practicados en la fachada de la Sala. La placa azul del sensor queda oculta en el interior de la pared de la Sala y los cables bajan directo por el pasacables del suelo.
- **Buzzer Piezoeléctrico:** Se monta en el centro de la pared divisoria interna (entre Cocina y Garaje/Sala), a una altura de 4 cm. Su orificio resonador queda orientado hacia el volumen central de la vivienda, garantizando que el tono de alarma (frecuencia de 2 kHz a 3 kHz) se propague sin amortiguación acústica.

#### C. Climatización (Sensor DHT11 con Rejilla de Cartón)
- **Rejilla de Ventilación:** En el tabique interior de la Sala (alejado del LED de techo), practica 4 cortes paralelos horizontales de 2.5 cm de longitud separados 4 mm entre sí. Presiona ligeramente las tiras para formar persianas venecianas (louvers).
- **Montaje DHT11:** Coloca el módulo azul DHT11 justo detrás de esta rejilla. Esto permite la libre convección de aire de la habitación protegiendo el sensor de golpes accidentales.
- **Aislamiento Térmico:** Al ubicarlo a 3 cm sobre el suelo y separado del LED difuso, se evita que el calor emitido por la resistencia limitadora del LED falsee la lectura de temperatura ambiente.

#### D. Iluminación por Zonas (4 LEDs Difusos de 5mm)
- En cada una de las 4 habitaciones (Sala, Dormitorio, Cocina, Garaje), elabora un pequeño soporte en ángulo de 45° con un retazo de cartón en la esquina superior de la pared (a 7 cm de altura).
- Inserta el LED difuso de 5mm apuntando hacia el centro del suelo de la habitación para conseguir una iluminación uniforme y sin deslumbramiento.
- Conecta el terminal negativo (cátodo, pata corta) a una línea de tierra común con cable flexible que baja por el pasacables.

---

## 5. Pruebas de Resistencia Mecánica y Portabilidad

Para verificar que la maqueta cumple con el estándar de transporte en mochila:
1. **Prueba de Inclinación:** Inclina la maqueta a 90° y 180° (boca abajo). Ningún cable debe colgar ni asomar en las habitaciones habitables.
2. **Prueba de Vibración:** Sacude suavemente la maqueta para simular el paso caminando con la mochila. El servomotor, sensor ultrasónico y RFID deben permanecer firmes gracias a sus soportes de encastre y escuadras triangulares.
3. **Conexión de Alimentación:** Un único cable USB conectado a un Power Bank en el zócalo posterior enciende todo el sistema en menos de 3 segundos, listo para la demostración.
