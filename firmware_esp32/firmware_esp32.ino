/**
 * ============================================================================
 * PROYECTO: HOGAR INTELIGENTE (SMART HOME) PORTÁTIL DIY CON ESP32
 * Institución: UNAN Robótica
 * Descripción: Firmware integral para maqueta con materiales reciclados.
 *              Incluye Servidor Web local, control de 4 zonas de iluminación,
 *              barrera de garaje con SG90, lector de acceso RFID-RC522,
 *              radar antirrobo ultrasónico HC-SR04 y sensor de clima DHT11.
 * ============================================================================
 */

#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <DHT.h>

// ----------------------------------------------------------------------------
// 1. ASIGNACIÓN DE PINES (GPIO) SEGÚN PLANO DEL FALSO FONDO
// ----------------------------------------------------------------------------
// Iluminación por habitaciones (LEDs difusos de 5mm con R 220Ω)
#define PIN_LED_SALA        16
#define PIN_LED_DORMITORIO  17
#define PIN_LED_COCINA      18
#define PIN_LED_GARAJE      19

// Garaje Inteligente
#define PIN_SERVO_GARAJE    13  // Salida PWM hacia SG90
#define PIN_RFID_SS          5  // SDA (Slave Select) del RC522
#define PIN_RFID_RST        22  // Reset del RC522
// Pines SPI por defecto en ESP32: SCK=18 (o remapeado a 14), MOSI=23, MISO=19 (o 12)

// Sistema Antirrobo
#define PIN_HC_TRIG          4  // Disparo ultrasonido HC-SR04
#define PIN_HC_ECHO         15  // Eco ultrasonido (Con divisor resistivo 1k/2k a 3.3V)
#define PIN_BUZZER           2  // Buzzer piezoeléctrico central

// Climatización Inteligente
#define PIN_DHT             27  // Línea DATA del DHT11
#define DHTTYPE          DHT11

// ----------------------------------------------------------------------------
// 2. OBJETOS Y VARIABLES GLOBALES
// ----------------------------------------------------------------------------
WebServer server(80);
Servo servoGaraje;
MFRC522 mfrc522(PIN_RFID_SS, PIN_RFID_RST);
DHT dht(PIN_DHT, DHTTYPE);

// Configuración de Red WiFi (Modo Punto de Acceso Autónomo)
const char* ap_ssid = "SmartHome-UNAN-Robotica";
const char* ap_pass = "unan123456";

// Estados del sistema
bool estadoLedSala       = false;
bool estadoLedDormitorio = false;
bool estadoLedCocina     = false;
bool estadoLedGaraje     = false;

bool barreraAbierta      = false;
int anguloActualServo    = 0;   // 0° = Cerrado horizontal, 90° = Abierto vertical
int anguloObjetivoServo  = 0;

bool sistemaAlarmaArmado = true;
bool alarmaDisparada     = false;
float distanciaUltrasonico = 999.0;
unsigned long ultimoEscaneoHC = 0;
const int UMBRAL_DISTANCIA_ALERTA = 18; // cm para detección en fachada

float temperaturaActual  = 25.0;
float humedadActual      = 60.0;
unsigned long ultimaLecturaDHT = 0;

String ultimoUID_RFID    = "Ninguna";
unsigned long tiempoAperturaGaraje = 0;
const unsigned long TIEMPO_GARAJE_AUTO_CLOSE = 5000; // Cierra tras 5 segundos

// Lista blanca de tarjetas RFID autorizadas (ejemplo: modificar con el UID de tu tarjeta)
const String UID_AUTORIZADO = "A1 B2 C3 D4";

// ----------------------------------------------------------------------------
// 3. PÁGINA WEB EMBEBIDA (HTML + CSS + JS RESPONSIVO)
// ----------------------------------------------------------------------------
const char PAGE_MAIN[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Home DIY - UNAN Robótica</title>
  <style>
    :root {
      --bg: #0f172a; --card: #1e293b; --accent: #38bdf8;
      --text: #f8fafc; --text-muted: #94a3b8; --border: #334155;
      --active: #10b981; --danger: #ef4444;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; font-family: system-ui, sans-serif; }
    body { background: var(--bg); color: var(--text); padding: 16px; display: flex; justify-content: center; }
    .container { max-width: 480px; width: 100%; }
    header { text-align: center; margin-bottom: 20px; }
    header h1 { font-size: 1.5rem; color: var(--accent); }
    header p { font-size: 0.85rem; color: var(--text-muted); }
    .grid { display: grid; grid-template-columns: 1fr 1fr; gap: 12px; margin-bottom: 16px; }
    .card { background: var(--card); border: 1px solid var(--border); border-radius: 12px; padding: 14px; }
    .card.full { grid-column: span 2; }
    h2 { font-size: 0.95rem; margin-bottom: 10px; display: flex; justify-content: space-between; align-items: center; }
    .btn {
      width: 100%; padding: 10px; border: none; border-radius: 8px;
      font-size: 0.9rem; font-weight: 600; cursor: pointer;
      background: #334155; color: white; transition: 0.2s;
    }
    .btn.on { background: var(--active); }
    .btn.alert { background: var(--danger); animation: pulse 1s infinite alternate; }
    @keyframes pulse { from { opacity: 0.8; } to { opacity: 1; } }
    .stat-val { font-size: 1.5rem; font-weight: bold; color: var(--accent); }
    .badge { font-size: 0.75rem; padding: 2px 8px; border-radius: 10px; background: #334155; }
    .badge.green { background: #065f46; color: #34d399; }
    .badge.red { background: #7f1d1d; color: #f87171; }
  </style>
</head>
<body>
  <div class="container">
    <header>
      <h1>🏠 Smart Home UNAN</h1>
      <p>Prototipo Portátil DIY con Doble Fondo</p>
    </header>

    <!-- Iluminación -->
    <div class="grid">
      <div class="card">
        <h2>Sala <span id="tag-sala" class="badge">OFF</span></h2>
        <button id="btn-sala" class="btn" onclick="toggleLed('sala')">💡 Luz Sala</button>
      </div>
      <div class="card">
        <h2>Dormitorio <span id="tag-dorm" class="badge">OFF</span></h2>
        <button id="btn-dorm" class="btn" onclick="toggleLed('dormitorio')">💡 Luz Dorm</button>
      </div>
      <div class="card">
        <h2>Cocina <span id="tag-coc" class="badge">OFF</span></h2>
        <button id="btn-coc" class="btn" onclick="toggleLed('cocina')">💡 Luz Cocina</button>
      </div>
      <div class="card">
        <h2>Garaje <span id="tag-gar" class="badge">OFF</span></h2>
        <button id="btn-gar" class="btn" onclick="toggleLed('garaje')">💡 Luz Garaje</button>
      </div>
    </div>

    <!-- Garaje y Servomotor -->
    <div class="card full" style="margin-bottom: 12px;">
      <h2>🚗 Garaje & Barrera SG90 <span id="tag-barrera" class="badge">Cerrada</span></h2>
      <button id="btn-barrera" class="btn" onclick="toggleGaraje()">Abrir / Cerrar Portón</button>
      <p style="font-size: 0.8rem; color: var(--text-muted); margin-top: 8px;">
        Último RFID: <strong id="rfid-uid">Ninguna tarjeta</strong>
      </p>
    </div>

    <!-- Sistema Antirrobo -->
    <div class="card full" style="margin-bottom: 12px;">
      <h2>🚨 Sistema Antirrobo (HC-SR04) <span id="tag-alarma" class="badge green">ARMADO</span></h2>
      <p style="font-size: 0.85rem; margin-bottom: 8px;">Distancia Fachada: <span class="stat-val" id="dist-val">--</span> cm</p>
      <button id="btn-alarma" class="btn" onclick="silenciarAlarma()">Silenciar / Reset Alarma</button>
    </div>

    <!-- Climatización -->
    <div class="grid">
      <div class="card">
        <h2>🌡️ Temperatura</h2>
        <div class="stat-val" id="temp-val">-- °C</div>
        <p style="font-size: 0.75rem; color: var(--text-muted); margin-top: 4px;">Sensor DHT11 en Sala</p>
      </div>
      <div class="card">
        <h2>💧 Humedad</h2>
        <div class="stat-val" id="hum-val">-- %</div>
        <p style="font-size: 0.75rem; color: var(--text-muted); margin-top: 4px;">Rejilla de convección</p>
      </div>
    </div>
  </div>

  <script>
    function updateUI(data) {
      // Luces
      setBtn('sala', data.ledSala);
      setBtn('dorm', data.ledDorm);
      setBtn('coc', data.ledCoc);
      setBtn('gar', data.ledGar);
      // Garaje
      const bBtn = document.getElementById('btn-barrera');
      const bTag = document.getElementById('tag-barrera');
      if (data.barrera) {
        bBtn.classList.add('on'); bBtn.innerText = 'Cerrar Barrera';
        bTag.innerText = 'ABIERTO'; bTag.className = 'badge green';
      } else {
        bBtn.classList.remove('on'); bBtn.innerText = 'Abrir Barrera';
        bTag.innerText = 'CERRADO'; bTag.className = 'badge';
      }
      document.getElementById('rfid-uid').innerText = data.rfid;
      // Alarma
      document.getElementById('dist-val').innerText = data.distancia.toFixed(1);
      const aTag = document.getElementById('tag-alarma');
      const aBtn = document.getElementById('btn-alarma');
      if (data.alerta) {
        aTag.innerText = '¡INTRUSO DETECTADO!'; aTag.className = 'badge red';
        aBtn.classList.add('alert'); aBtn.innerText = '¡APAGAR ALARMA!';
      } else {
        aTag.innerText = 'VIGILANDO'; aTag.className = 'badge green';
        aBtn.classList.remove('alert'); aBtn.innerText = 'Sistema Normal';
      }
      // Clima
      document.getElementById('temp-val').innerText = data.temp.toFixed(1) + ' °C';
      document.getElementById('hum-val').innerText = data.hum.toFixed(0) + ' %';
    }

    function setBtn(id, st) {
      const b = document.getElementById('btn-' + id);
      const t = document.getElementById('tag-' + id);
      if (st) {
        b.classList.add('on'); t.innerText = 'ON'; t.className = 'badge green';
      } else {
        b.classList.remove('on'); t.innerText = 'OFF'; t.className = 'badge';
      }
    }

    function toggleLed(r) { fetch('/api/led?room=' + r + '&state=toggle').then(r => r.json()).then(updateUI); }
    function toggleGaraje() { fetch('/api/garage?action=toggle').then(r => r.json()).then(updateUI); }
    function silenciarAlarma() { fetch('/api/alarm?action=reset').then(r => r.json()).then(updateUI); }

    setInterval(() => {
      fetch('/api/status').then(r => r.json()).then(updateUI).catch(() => {});
    }, 1200);
  </script>
</body>
</html>
)rawliteral";

// ----------------------------------------------------------------------------
// 4. FUNCIONES DE SERVICIO REST API
// ----------------------------------------------------------------------------
String getStatusJSON() {
  String json = "{";
  json += "\"ledSala\":" + String(estadoLedSala ? "true" : "false") + ",";
  json += "\"ledDorm\":" + String(estadoLedDormitorio ? "true" : "false") + ",";
  json += "\"ledCoc\":" + String(estadoLedCocina ? "true" : "false") + ",";
  json += "\"ledGar\":" + String(estadoLedGaraje ? "true" : "false") + ",";
  json += "\"barrera\":" + String(barreraAbierta ? "true" : "false") + ",";
  json += "\"rfid\":\"" + ultimoUID_RFID + "\",";
  json += "\"distancia\":" + String(distanciaUltrasonico, 1) + ",";
  json += "\"alerta\":" + String(alarmaDisparada ? "true" : "false") + ",";
  json += "\"temp\":" + String(temperaturaActual, 1) + ",";
  json += "\"hum\":" + String(humedadActual, 1);
  json += "}";
  return json;
}

void handleRoot() {
  server.send(200, "text/html", PAGE_MAIN);
}

void handleStatus() {
  server.send(200, "application/json", getStatusJSON());
}

void handleLed() {
  if (server.hasArg("room")) {
    String room = server.arg("room");
    String state = server.hasArg("state") ? server.arg("state") : "toggle";

    if (room == "sala") {
      estadoLedSala = (state == "toggle") ? !estadoLedSala : (state == "1" || state == "true");
      digitalWrite(PIN_LED_SALA, estadoLedSala ? HIGH : LOW);
    } else if (room == "dormitorio") {
      estadoLedDormitorio = (state == "toggle") ? !estadoLedDormitorio : (state == "1" || state == "true");
      digitalWrite(PIN_LED_DORMITORIO, estadoLedDormitorio ? HIGH : LOW);
    } else if (room == "cocina") {
      estadoLedCocina = (state == "toggle") ? !estadoLedCocina : (state == "1" || state == "true");
      digitalWrite(PIN_LED_COCINA, estadoLedCocina ? HIGH : LOW);
    } else if (room == "garaje") {
      estadoLedGaraje = (state == "toggle") ? !estadoLedGaraje : (state == "1" || state == "true");
      digitalWrite(PIN_LED_GARAJE, estadoLedGaraje ? HIGH : LOW);
    }
  }
  server.send(200, "application/json", getStatusJSON());
}

void handleGarage() {
  if (server.hasArg("action")) {
    String act = server.arg("action");
    if (act == "toggle") {
      barreraAbierta = !barreraAbierta;
    } else if (act == "open") {
      barreraAbierta = true;
    } else if (act == "close") {
      barreraAbierta = false;
    }
    anguloObjetivoServo = barreraAbierta ? 90 : 0;
  }
  server.send(200, "application/json", getStatusJSON());
}

void handleAlarm() {
  if (server.hasArg("action") && server.arg("action") == "reset") {
    alarmaDisparada = false;
    digitalWrite(PIN_BUZZER, LOW);
  }
  server.send(200, "application/json", getStatusJSON());
}

// ----------------------------------------------------------------------------
// 5. RUTINAS DE CONTROL DE HARDWARE
// ----------------------------------------------------------------------------
void actualizarServoSuave() {
  // Movimiento amortiguado del servo SG90 para evitar sacudidas y picos de consumo
  if (anguloActualServo < anguloObjetivoServo) {
    anguloActualServo += 3;
    if (anguloActualServo > anguloObjetivoServo) anguloActualServo = anguloObjetivoServo;
    servoGaraje.write(anguloActualServo);
  } else if (anguloActualServo > anguloObjetivoServo) {
    anguloActualServo -= 3;
    if (anguloActualServo < anguloObjetivoServo) anguloActualServo = anguloObjetivoServo;
    servoGaraje.write(anguloActualServo);
  }
}

void escanearUltrasonico() {
  // Disparo de pulso limpio de 10 microsegundos
  digitalWrite(PIN_HC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_HC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_HC_TRIG, LOW);

  long duracion = pulseIn(PIN_HC_ECHO, HIGH, 25000); // Timeout 25ms (~4 metros)
  if (duracion > 0) {
    distanciaUltrasonico = duracion * 0.0343 / 2.0;
  } else {
    distanciaUltrasonico = 999.0;
  }

  // Detección de intruso en fachada frontal
  if (distanciaUltrasonico > 2.0 && distanciaUltrasonico < UMBRAL_DISTANCIA_ALERTA && sistemaAlarmaArmado) {
    if (!alarmaDisparada) {
      alarmaDisparada = true;
      Serial.println(F("[SEGURIDAD] ¡Intruso detectado frente a la vivienda!"));
    }
  }
}

void escanearRFID() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) uid += " ";
  }
  uid.toUpperCase();
  ultimoUID_RFID = uid;
  Serial.print(F("[RFID] Tarjeta detectada: "));
  Serial.println(uid);

  // Beep de confirmación acústica en buzzer
  tone(PIN_BUZZER, 2400, 150);

  // Abrir portón automáticamente por 5 segundos
  barreraAbierta = true;
  anguloObjetivoServo = 90;
  tiempoAperturaGaraje = millis();

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void leerDHT11() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t) && !isnan(h)) {
    temperaturaActual = t;
    humedadActual = h;
  }
}

// ----------------------------------------------------------------------------
// 6. SETUP PRINCIPAL
// ----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println(F("\n=== SMART HOME UNAN ROBOTICA - INICIANDO ==="));

  // Configuración de pines de iluminación
  pinMode(PIN_LED_SALA, OUTPUT);
  pinMode(PIN_LED_DORMITORIO, OUTPUT);
  pinMode(PIN_LED_COCINA, OUTPUT);
  pinMode(PIN_LED_GARAJE, OUTPUT);
  digitalWrite(PIN_LED_SALA, LOW);
  digitalWrite(PIN_LED_DORMITORIO, LOW);
  digitalWrite(PIN_LED_COCINA, LOW);
  digitalWrite(PIN_LED_GARAJE, LOW);

  // Configuración de Sistema Antirrobo
  pinMode(PIN_HC_TRIG, OUTPUT);
  pinMode(PIN_HC_ECHO, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  // Configuración de Servomotor Garaje SG90
  ESP32PWM::allocateTimer(0);
  servoGaraje.setPeriodHertz(50);
  servoGaraje.attach(PIN_SERVO_GARAJE, 500, 2400);
  servoGaraje.write(0);

  // Inicialización de Bus SPI y Lector RFID-RC522
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("[RFID] MFRC522 listo en muro lateral del garaje"));

  // Inicialización de Sensor Climático DHT11
  dht.begin();
  Serial.println(F("[CLIMA] DHT11 iniciado tras rejilla de cartón"));

  // Modo Punto de Acceso WiFi
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);
  IPAddress IP = WiFi.softAPIP();
  Serial.print(F("[WIFI] Punto de acceso creado: "));
  Serial.println(ap_ssid);
  Serial.print(F("[WIFI] Conéctate y abre tu navegador en: http://"));
  Serial.println(IP);

  // Enrutamiento del Servidor Web
  server.on("/", handleRoot);
  server.on("/api/status", handleStatus);
  server.on("/api/led", handleLed);
  server.on("/api/garage", handleGarage);
  server.on("/api/alarm", handleAlarm);
  server.begin();
  Serial.println(F("[WEB] Servidor HTTP escuchando peticiones en puerto 80"));

  // Beep doble de sistema listo
  tone(PIN_BUZZER, 2000, 100);
  delay(150);
  tone(PIN_BUZZER, 2600, 120);
}

// ----------------------------------------------------------------------------
// 7. BUCLE PRINCIPAL (LOOP NO BLOQUEANTE)
// ----------------------------------------------------------------------------
void loop() {
  server.handleClient();
  actualizarServoSuave();
  escanearRFID();

  // Escaneo ultrasónico de fachada cada 120 ms
  if (millis() - ultimoEscaneoHC > 120) {
    ultimoEscaneoHC = millis();
    escanearUltrasonico();
  }

  // Lectura de temperatura y humedad cada 2.5 segundos
  if (millis() - ultimaLecturaDHT > 2500) {
    ultimaLecturaDHT = millis();
    leerDHT11();
  }

  // Cierre automático de barrera de garaje
  if (barreraAbierta && tiempoAperturaGaraje > 0 && (millis() - tiempoAperturaGaraje > TIEMPO_GARAJE_AUTO_CLOSE)) {
    barreraAbierta = false;
    anguloObjetivoServo = 0;
    tiempoAperturaGaraje = 0;
    Serial.println(F("[GARAJE] Cierre automático por temporizador completado"));
  }

  // Tono intermitente de alarma cuando se detecta un intruso
  if (alarmaDisparada) {
    static unsigned long tiempoBuzzer = 0;
    if (millis() - tiempoBuzzer > 250) {
      tiempoBuzzer = millis();
      static bool tonoAlto = false;
      tonoAlto = !tonoAlto;
      if (tonoAlto) {
        tone(PIN_BUZZER, 2800);
      } else {
        tone(PIN_BUZZER, 1800);
      }
    }
  }

  delay(5);
}
