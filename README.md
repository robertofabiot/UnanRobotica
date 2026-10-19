# Maqueta de "Hogar Inteligente" (Smart Home) Portátil DIY
### Proyecto de Bricolaje Sostenible con Materiales Reciclados & Doble Fondo Técnico
**UNAN Robótica &bull; Educación en IoT y Prototipado Rápido**

---

## 🌟 Resumen del Proyecto

Este proyecto presenta el **prototipo 3D interactivo y físico** de una vivienda unifamiliar domótica a escala, concebida bajo la filosofía **DIY (Do It Yourself)** con materiales 100% reciclados y accesibles. Su diseño está optimizado para **alta portabilidad**, permitiendo que estudiantes e investigadores puedan trasladarla dentro de una mochila escolar estándar sin riesgo de que se desprendan cables o se descalibren los sensores.

---

## 📐 Especificaciones de Formato y Estructura

- **Dimensiones Exteriores:** Formato rectangular compacto estilo "caja de zapatos" de una sola planta:
  - **Largo:** 30.0 cm
  - **Profundidad:** 22.0 cm
  - **Altura Total:** 10.0 cm
- **Formato Arquitectónico:** Vista de **planta abierta (Open-Ceiling / corte arquitectónico)** sin techo, lo cual permite observar desde cualquier ángulo el funcionamiento interior de los actuadores, la iluminación difusa y los sensores.
- **Materiales Constructivos Principales:**
  - **Cartón Corrugado Reciclado (Onda B o C, e ≈ 3.5 mm):** Base estructural, piso habitable intermedio, tabiques perimetrales/interiores y escuadras triangulares de refuerzo a 45°.
  - **Palitos de Madera de Helado / Bajalenguas:** Marcos de puertas y ventanas, dintel reforzado del garaje y pluma de la barrera levadiza vehicular.
  - **Plástico PET Reciclado (Botellas de 1.5L/2L):** Cristales transparentes, ligeros e irrompibles para las ventanas de Dormitorio y Cocina.
- **Sistema de "Doble Fondo" Técnico (Altura = 2.0 cm):**
  - Compartimento inferior cerrado y protegido entre $Y = 0\text{ cm}$ y $Y = 2\text{ cm}$.
  - Aloja la **Mini Protoboard (400 puntos)**, el **microcontrolador ESP32 DevKit V1**, la resistencia y el mazo de cables DuPont.
  - Los sensores superiores se conectan mediante **ojales pasacables directos** hacia el falso fondo.
  - Entrada posterior para conector USB de alimentación mediante Power Bank de 5V.

---

## 🏠 Distribución de Habitaciones (4 Zonas a 90°)

1. **Sala / Entrada Principal (Frontal-Izquierda: 18 cm x 11 cm):**
   - Fachada con puerta principal enmarcada en madera y vanos cilíndricos para el sensor ultrasónico HC-SR04.
   - Soporte superior biselado a 45° con **LED difuso de 5mm (Blanco Cálido)**.
   - Sensor de clima **DHT11** montado en el tabique lateral tras una **rejilla de ventilación calada** en el cartón, aislado del calor del LED.
2. **Garaje Inteligente (Frontal-Derecha: 12 cm x 11 cm):**
   - Vano abierto con **servomotor SG90** acoplado a una **barrera levadiza** de palito de madera con franjas reflectantes.
   - En el muro exterior lateral derecho: soporte plano para el lector de tarjetas **RFID-RC522 (13.56 MHz)**.
   - Soporte superior con **LED difuso de 5mm (Blanco Neutro)**.
3. **Dormitorio (Posterior-Izquierda: 15 cm x 11 cm):**
   - Ventana exterior lateral con cristal de **plástico PET reciclado** y cruceta de madera.
   - Soporte superior con **LED difuso de 5mm (Ámbar Relajante)**.
4. **Cocina (Posterior-Derecha: 15 cm x 11 cm):**
   - Ventana posterior con cristal PET.
   - Soporte superior con **LED difuso de 5mm (Blanco Frío)**.
   - En la pared divisoria central: montaje del **Buzzer Piezoeléctrico**, garantizando una propagación acústica omnidireccional.

---

## 🚀 Cómo Visualizar el Prototipo 3D Interactivo (`index.html`)

El archivo [`index.html`](file:///home/ro/Projects/UnanRobotica/index.html) es una aplicación web 3D autocontenida que funciona **offline** o conectado a internet.

### Opción 1: Abrir directamente en el navegador
Haz doble clic sobre el archivo `index.html` o ábrelo desde cualquier navegador moderno (Chrome, Firefox, Edge, Safari).

### Opción 2: Ejecutar con un servidor local
```bash
# Con Python 3:
python3 -m http.server 8080

# O con Node.js:
npx serve .
```
Luego abre tu navegador en `http://localhost:8080`.

### Características de la Aplicación 3D:
- **OrbitControls:** Rotación 360°, paneo y zoom suave con amortiguación.
- **Control Domótico Web en Tiempo Real:**
  - Encendido/apagado independiente de los 4 LEDs por habitación con emisión de luz puntual física.
  - Accionamiento de la barrera levadiza del garaje con sonido mecánico sintetizado de servo SG90.
  - Simulación de tarjeta RFID (animación 3D de pase de tarjeta, validación con destello verde y apertura automática).
  - Radar ultrasónico en fachada con control de proximidad, ondas de sonar concéntricas y sirena antirobo con buzzer sintetizado.
  - Monitor térmico con sensor DHT11 y ventilación animada por convección.
  - **Botón "Abrir Doble Fondo":** Vista explotada animada que eleva la vivienda para inspeccionar el ESP32, la protoboard y el enrutamiento de cables.
- **Presets de Cámara:** Vista Isométrica, Vista en Planta (Top-down), Vista Frontal de Fachada, Vista Garaje/RFID y Vista Técnica de Doble Fondo.
- **Modos Visuales:** Realista PBR (kraft/madera/PET), Modo Materiales Reciclados (codificado por colores), Rayos X / Cableado y Cotas con Medidas en centímetros.
- **Tour Guiado Interactivo:** Tutorial paso a paso que explica cada decisión de diseño para estudiantes y evaluadores.
- **Modales Integrados:** Visualizador del Plano de Corte SVG y Lista BOM de Materiales.
- **Web Audio API:** Efectos de sonido sintetizados en tiempo real sin requerir archivos externos de audio.

---

## 📂 Estructura de Archivos del Repositorio

```
UnanRobotica/
│
├── index.html                    # Aplicación Web 3D interactiva principal
├── README.md                     # Documentación general del proyecto
│
├── js/                           # Bibliotecas JavaScript locales (Offline)
│   ├── three.min.js              # Three.js r128
│   ├── OrbitControls.js          # Control de cámara orbital
│   └── tween.umd.js              # Motor de interpolación de animaciones
│
├── docs/                         # Manuales y Planos Técnicos
│   ├── guia_construccion_diy.md  # Manual paso a paso de corte, ensamble y transporte
│   ├── diagrama_conexiones.md    # Pinout del ESP32, divisor resistivo y presupuesto eléctrico
│   └── planos_corte_despiece.svg # Vectorial con despiece a escala 1:1 de cartón y madera
│
└── firmware_esp32/               # Código Fuente para el Microcontrolador
    └── firmware_esp32.ino        # Firmware Arduino con WiFi, Servidor Web local,
                                  # control de servo SG90, RFID RC522, HC-SR04, DHT11 y LEDs
```

---

## ⚡ Conexión Eléctrica y Firmware ESP32

El firmware [`firmware_esp32/firmware_esp32.ino`](file:///home/ro/Projects/UnanRobotica/firmware_esp32/firmware_esp32.ino) convierte al ESP32 en un **Punto de Acceso WiFi autónomo** (`SmartHome-UNAN-Robotica`) con servidor web HTTP en el puerto 80.

### Resumen de Pines (GPIO):
- **LEDs Difusos 5mm:** Sala (GPIO 16), Dormitorio (GPIO 17), Cocina (GPIO 18), Garaje (GPIO 19).
- **Servomotor SG90:** GPIO 13 (PWM a 50 Hz).
- **Lector RFID-RC522:** SS/SDA (GPIO 5), RST (GPIO 22), SCK (GPIO 18 o 14), MOSI (GPIO 23), MISO (GPIO 19 o 12). Alimentación a 3.3V.
- **Sensor Ultrasónico HC-SR04:** TRIG (GPIO 4), ECHO (GPIO 15 a través de divisor 1kΩ / 2kΩ).
- **Buzzer Alarma:** GPIO 2.
- **Sensor DHT11:** GPIO 27 (DATA con pull-up de 10kΩ).

---

## 🎒 Recomendaciones de Portabilidad en Mochila

1. **Fijación de la Placa Habitable:** Sujeta el piso superior sobre los zócalos de 2 cm usando 4 tiras de velcro adhesivo de doble cara; esto mantiene el compartimento cerrado durante el viaje pero accesible para demostraciones.
2. **Capacitor de Desacoplo:** Coloca un capacitor de 100µF a 470µF en paralelo con las líneas de 5V y GND en la protoboard para absorber el consumo de arranque del servomotor SG90 cuando se alimenta desde un Power Bank USB.
3. **Escuadras en Esquinas:** Las 16 escuadras triangulares de cartón de 2.5 x 2.5 cm aseguran que la maqueta resista la compresión lateral al guardarla junto a cuadernos y laptops.
