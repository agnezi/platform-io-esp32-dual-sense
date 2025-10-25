/**
 * Protocolo de comunicação entre controlador (DualSense+LCD)
 * e nó de motores (4x 28BYJ-48). Usa ESP-NOW.
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Arduino.h>

typedef struct __attribute__((packed)) {
  int16_t m1;
  int16_t m2;
  int16_t m3;
  int16_t m4;
  uint32_t seq;
} MotorSpeedsMessage;

#ifdef ROLE_CONTROLLER
bool protocolInitController(const char *motorNodeMacStr);
bool protocolSendSpeeds(int m1, int m2, int m3, int m4);
#endif

#ifdef ROLE_MOTOR_NODE
bool protocolInitMotorNode();
// Retorna true se já recebeu algum pacote; preenche velocidades e seq; ageMs = ms desde último pacote.
bool protocolGetLatestSpeeds(int &m1, int &m2, int &m3, int &m4, uint32_t &seq, unsigned long &ageMs);
#endif

#endif // PROTOCOL_H