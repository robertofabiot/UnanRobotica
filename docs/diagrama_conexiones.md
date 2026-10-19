# Diagrama de Conexiones Eléctricas y Cableado Técnico
**Proyecto:** Maqueta "Hogar Inteligente" (Smart Home) Portátil DIY  
**Microcontrolador Principal:** ESP32 DevKit V1 (30 pines) / Compatible con ESP8266  
**Ubicación de Componentes:** Doble Fondo de 2 cm (Protoboard + ESP32 + Regulación)  

---

## 1. Tabla Maestra de Asignación de Pines (Pinout ESP32)

| Componente | Tipo de Señal | Pin del Módulo | Pin ESP32 (GPIO) | Voltaje de Alimentación | Notas de Cableado y Protección |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **LED Sala** | Salida Digital | Ánodo (+) | **GPIO 16** | 3.3V (Vía R 220Ω) | Resistencia de 220Ω en serie; Cátodo (-) a GND |
| **LED Dormitorio**| Salida Digital | Ánodo (+) | **GPIO 17** | 3.3V (Vía R 220Ω) | Resistencia de 220Ω en serie; Cátodo (-) a GND |
| **LED Cocina** | Salida Digital | Ánodo (+) | **GPIO 18** | 3.3V (Vía R 220Ω) | Resistencia de 220Ω en serie; Cátodo (-) a GND |
| **LED Garaje** | Salida Digital | Ánodo (+) | **GPIO 19** | 3.3V (Vía R 220Ω) | Resistencia de 220Ω en serie; Cátodo (-) a GND |
| **Servomotor SG90**| Salida PWM | Señal (Naranja) | **GPIO 13** | **5V (VIN / VBUS)**| Alimentar desde riel de 5V para evitar caídas en el ESP32 |
| | Alimentación | VCC (Rojo) | **5V (VIN)** | 5V | Se sugiere capacitor de 100µF entre 5V y GND |
| | Masa | GND (Marrón) | **GND** | 0V | Tierra común |
| **Lector RFID RC522**| Bus SPI | SDA (SS) | **GPIO 5** | **3.3V (¡CRÍTICO!)**| ¡Nunca conectar a 5V! El chip RC522 se quema con 5V |
| | Bus SPI | SCK | **GPIO 18 / 14**| 3.3V | Reloj SPI (GPIO 14 en configuración secundaria o 18 en bus nativo) |
| | Bus SPI | MOSI | **GPIO 23** | 3.3V | Master Out Slave In |
| | Bus SPI | MISO | **GPIO 19 / 12**| 3.3V | Master In Slave Out |
| | Reset | RST | **GPIO 22** | 3.3V | Reset del chip MFRC522 |
| | Alimentación | 3.3V | **3V3** | 3.3V | Riel de 3.3V del ESP32 |
| | Masa | GND | **GND** | 0V | Tierra común |
| **HC-SR04 Ultrasónico**| Salida Pulso | TRIG | **GPIO 4** | 5V | Disparo de pulso de 10µs |
| | Entrada Pulso | ECHO | **GPIO 15** | 5V a 3.3V | **Divisor resistivo:** 1kΩ y 2kΩ (protege pin de 3.3V) |
| | Alimentación | VCC | **5V (VIN)** | 5V | Transductores de 40kHz requieren 5V |
| | Masa | GND | **GND** | 0V | Tierra común |
| **Sensor DHT11** | Entrada 1-Wire | DATA | **GPIO 27** | 3.3V o 5V | Requiere resistencia pull-up de 10kΩ entre DATA y VCC |
| | Alimentación | VCC | **3V3** | 3.3V | Recomendado 3.3V para lectura directa en ADC/GPIO |
| | Masa | GND | **GND** | 0V | Tierra común |
| **Buzzer Alarma** | Salida Digital/PWM| Positivo (+) | **GPIO 2** | 3.3V o 5V | Buzzer activo o pasivo accionado por tono PWM |
| | Masa | Negativo (-) | **GND** | 0V | Tierra común |

> [!IMPORTANT]
> **Protección del Pin ECHO del HC-SR04:**
> El módulo ultrasónico HC-SR04 emite un pulso en el pin `ECHO` a 5V. Dado que los pines del ESP32 operan a 3.3V, se debe colocar un divisor de tensión simple en la protoboard:
> - Resistencia $R_1 = 1\text{ k}\Omega$ conectada entre el pin `ECHO` del sensor y el GPIO 15 del ESP32.
> - Resistencia $R_2 = 2\text{ k}\Omega$ (o $2.2\text{ k}\Omega$) conectada entre el GPIO 15 y GND.
> Esto convierte el pulso de 5V en un pulso seguro de aprox. 3.33V.

---

## 2. Esquema Arquitectónico del Doble Fondo (2 cm de Espacio Libre)

```
+-------------------------------------------------------------------------------+
|                           PARED POSTERIOR (30 cm)                             |
|                                [Puerto USB 5V]                                |
|                                                                               |
|      +------------------------+          +-----------------------------+      |
|      |    MINI PROTOBOARD     |          |       ESP32 DEVKIT V1       |      |
|      |  - Riel +5V (Rojo)     |          |  [EN]                 [BOOT]|      |
|      |  - Riel GND (Azul)     | <======> |  D23 D22 TX RX D21 D19 D18  |      |
|      |  - Divisor R 1k/2k     | (DuPont) |  D5  D4  D2  D15 D13 D12    |      |
|      |  - 4x R 220Ω para LEDs |          |  VIN GND 3V3 GPIO16 GPIO17  |      |
|      |  - Cap 100µF estabiliz |          +-----------------------------+      |
|      +------------------------+                                               |
|                                                                               |
| (Ojal 1: Sala)   (Ojal 2: HC-SR04)   (Ojal 3: DHT11)   (Ojal 4: Dormitorio)   |
| (Ojal 5: Cocina) (Ojal 6: Buzzer)    (Ojal 7: Garaje)  (Ojal 8: RFID RC522)   |
|                                                                               |
|               Cables asegurados con clips de cartón y cinta                   |
+-------------------------------------------------------------------------------+
                             FACHADA FRONTAL (30 cm)
```

---

## 3. Presupuesto de Consumo Eléctrico y Autonomía Portátil

| Modo de Operación | Consumo Estimado | Fuente de Alimentación Recomendada | Autonomía |
| :--- | :--- | :--- | :--- |
| **Reposo (WiFi Activo, 4 LEDs apagados, Servo quieto)** | ~95 mA @ 5V (0.47 W) | Power Bank USB estándar (10,000 mAh) | **~70 a 80 horas continuas** |
| **Normal (2 LEDs encendidos, lecturas DHT11 + HC-SR04)** | ~135 mA @ 5V (0.67 W) | Power Bank USB estándar (10,000 mAh) | **~50 a 60 horas continuas** |
| **Pico de Operación (4 LEDs ON, Buzzer sonando, Servo girando)**| ~380 mA @ 5V (1.9 W) | Power Bank USB o cargador 5V 1A | Soporta picos sin reinicios |

> [!TIP]
> **Consejo Antirreinicios (Brown-out):**
> Los micro servomotores SG90 presentan picos de corriente inductiva de hasta 350mA al arrancar. Para evitar que el microcontrolador ESP32 se reinicie repentinamente por caída de tensión (*brownout detector*), es una buena práctica conectar un capacitor electrolítico de **100µF a 470µF a 10V** en paralelo con las líneas de alimentación de 5V y GND en la protoboard del doble fondo.
