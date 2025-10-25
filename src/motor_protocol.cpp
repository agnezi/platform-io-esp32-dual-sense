/**
 * Implementação ESP-NOW para nó de motores (ROLE_MOTOR_NODE)
 */
#ifdef ROLE_MOTOR_NODE

#include "protocol.h"
#include "config.h"
#include <esp_now.h>
#include <WiFi.h>

static bool havePacket = false;
static MotorSpeedsMessage lastMsg{};
static unsigned long lastMsgMillis = 0;

static void onDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  if (len != sizeof(MotorSpeedsMessage)) {
    return; // ignora tamanhos inválidos
  }
  MotorSpeedsMessage incoming;
  memcpy(&incoming, data, sizeof(MotorSpeedsMessage));
  // Clamp speeds para segurança
  int maxAbs = MAX_SPEED_THROTTLE; // usar throttling limite como geral
  #define CLAMP(v) if((v) > maxAbs) (v)=maxAbs; else if((v) < -maxAbs) (v)=-maxAbs;
  CLAMP(incoming.m1); CLAMP(incoming.m2); CLAMP(incoming.m3); CLAMP(incoming.m4);
  lastMsg = incoming;
  lastMsgMillis = millis();
  havePacket = true;
}

bool protocolInitMotorNode() {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("[MOTOR_PROTO] Falha ao inicializar ESP-NOW");
    return false;
  }
  if (esp_now_register_recv_cb(onDataRecv) != ESP_OK) {
    Serial.println("[MOTOR_PROTO] Falha ao registrar callback recv");
    return false;
  }
  Serial.println("[MOTOR_PROTO] ESP-NOW pronto para receber.");
  return true;
}

bool protocolGetLatestSpeeds(int &m1, int &m2, int &m3, int &m4, uint32_t &seq, unsigned long &ageMs) {
  if (!havePacket) {
    return false;
  }
  // Como callback não é ISR, risco de race é mínimo para tipos nativos; copiar sem desabilitar interrupções.
  MotorSpeedsMessage copy = lastMsg;
  unsigned long ts = lastMsgMillis;
  m1 = copy.m1; m2 = copy.m2; m3 = copy.m3; m4 = copy.m4; seq = copy.seq;
  ageMs = millis() - ts;
  return true;
}

#endif // ROLE_MOTOR_NODE
