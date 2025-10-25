/**
 * Controlador (DualSense + LCD) -> envia velocidades ao motor-node via ESP-NOW.
 */
#ifdef ROLE_CONTROLLER

#include <Arduino.h>
#include <ps5Controller.h>
#include "config.h"
#include "display.h"
#include "protocol.h"

// Estado
static bool connected = false;
static unsigned long lastSend = 0;
static unsigned long lastDisplay = 0;
static const unsigned long SEND_INTERVAL = CONTROLLER_SEND_INTERVAL;    // ms (config.h)
static const unsigned long DISPLAY_INTERVAL = CONTROLLER_DISPLAY_INTERVAL; // ms (config.h)

// Velocidades atuais
static int m1=0,m2=0,m3=0,m4=0;

// Funções de mapeamento (baseado em lógica anterior)
static int mapTrigger(int press) { return map(press, 0, 255, 0, MAX_SPEED_THROTTLE); }
static int mapStick(int v) {
  if (abs(v) < ANALOG_DEADZONE) return 0;
  // Map full range -128 to 127 to -MAX_SPEED_STEERING to MAX_SPEED_STEERING
  return map(v, -128, 127, -MAX_SPEED_STEERING, MAX_SPEED_STEERING);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== Controller Node Iniciando ===");
  initDisplay();
  showWaitingMessage();

  // Inicializar DualSense
  ps5.begin(PS5_MAC_ADDRESS);
  Serial.print("Aguardando DualSense: ");
  Serial.println(PS5_MAC_ADDRESS);

  // Inicializar protocolo (MAC do motor node via build flag)
  #ifdef MOTOR_NODE_MAC_STR
    if (!protocolInitController(MOTOR_NODE_MAC_STR)) {
      Serial.println("Falha protocolo, reiniciando...");
      delay(2000);
      ESP.restart();
    }
  #else
    Serial.println("[WARN] MOTOR_NODE_MAC_STR não definido");
  #endif
}

void computeSpeeds() {
  // Motor 1: R2/L2
  if (ps5.R2Value() > 0) m1 = mapTrigger(ps5.R2Value());
  else if (ps5.L2Value() > 0) m1 = -mapTrigger(ps5.L2Value());
  else m1 = 0;

  // Motor 2: Left Stick X
  m2 = mapStick(ps5.LStickX());
  // Motor 3: Right Stick X
  m3 = mapStick(ps5.RStickX());

  // Motor 4: L1 / R1
  if (ps5.L1() && !ps5.R1()) m4 = MAX_SPEED_THROTTLE;
  else if (ps5.R1() && !ps5.L1()) m4 = -MAX_SPEED_THROTTLE;
  else m4 = 0;
}

void updateDisplayIfNeeded() {
  unsigned long now = millis();
  if (now - lastDisplay >= DISPLAY_INTERVAL) {
    updateAllMotorsDisplay(m1,m2,m3,m4);
    uint8_t bat = ps5.Battery();
  bat = map(bat,0,BATTERY_RAW_MAX,0,100);
    updateConnectionStatus(connected, bat);
    lastDisplay = now;
  }
}

void loop() {
  bool isConn = ps5.isConnected();
  if (isConn && !connected) {
    connected = true;
    Serial.println("DualSense conectado.");
  }
  if (!isConn && connected) {
    connected = false;
    Serial.println("DualSense desconectado.");
    showWaitingMessage();
  }

  if (connected) {
    if (ps5.Options()) {
      Serial.println("Options -> Reiniciar controlador");
      ESP.restart();
    }
    computeSpeeds();
    unsigned long now = millis();
    if (now - lastSend >= SEND_INTERVAL) {
      protocolSendSpeeds(m1,m2,m3,m4);
      lastSend = now;
    }
    updateDisplayIfNeeded();
  } else {
    // simples delay aguardando
    delay(50);
  }
  yield();
}

#endif // ROLE_CONTROLLER