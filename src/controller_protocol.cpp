/**
 * Implementação ESP-NOW para o controlador (env:controller)
 */
#ifdef ROLE_CONTROLLER

#include "protocol.h"
#include "config.h" // for ESP_NOW_CHANNEL
#include <esp_now.h>
#include <WiFi.h>

static uint8_t peerMac[6];
static bool peerReady = false;
static uint32_t seqCounter = 0;

static bool parseMac(const char *macStr, uint8_t out[6]) {
  // Formato esperado: XX:XX:XX:XX:XX:XX
  if (!macStr) return false;
  int values[6];
  if (sscanf(macStr, "%x:%x:%x:%x:%x:%x", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]) != 6) {
    return false;
  }
  for (int i=0;i<6;i++) out[i] = (uint8_t) values[i];
  return true;
}

bool protocolInitController(const char *motorNodeMacStr) {
  WiFi.mode(WIFI_STA);
  if (!parseMac(motorNodeMacStr, peerMac)) {
    Serial.println("[CTRL_PROTO] MAC inválido para nó de motores");
    return false;
  }
  if (esp_now_init() != ESP_OK) {
    Serial.println("[CTRL_PROTO] Falha ao inicializar ESP-NOW");
    return false;
  }
  esp_now_peer_info_t peerInfo{};
  memcpy(peerInfo.peer_addr, peerMac, 6);
  peerInfo.channel = ESP_NOW_CHANNEL;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[CTRL_PROTO] Falha ao adicionar peer");
    return false;
  }
  peerReady = true;
  Serial.print("[CTRL_PROTO] Peer registrado: ");
  for (int i=0;i<6;i++){ Serial.printf("%02X", peerMac[i]); if(i<5) Serial.print(":"); }
  Serial.println();
  return true;
}

bool protocolSendSpeeds(int m1, int m2, int m3, int m4) {
  if (!peerReady) return false;
  MotorSpeedsMessage msg{};
  msg.m1 = (int16_t)m1;
  msg.m2 = (int16_t)m2;
  msg.m3 = (int16_t)m3;
  msg.m4 = (int16_t)m4;
  msg.seq = ++seqCounter;
  esp_err_t r = esp_now_send(peerMac, (uint8_t*)&msg, sizeof(msg));
  if (r != ESP_OK) {
    Serial.printf("[CTRL_PROTO] Erro envio: %d\n", r);
    return false;
  }
  return true;
}

#endif // ROLE_CONTROLLER