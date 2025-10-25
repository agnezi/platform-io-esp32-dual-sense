/**
 * Motor Node Receiver Firmware
 * Recebe velocidades via ESP-NOW e aplica aos 4 motores 28BYJ-48.
 */
#ifdef ROLE_MOTOR_NODE

#include <Arduino.h>
#include <AccelStepper.h>
#include <WiFi.h>
#include "config.h"
#include "protocol.h"

// Steppers (lembrar ordem FULL4WIRE: IN1, IN3, IN2, IN4)
AccelStepper motor1(AccelStepper::FULL4WIRE, MOTOR1_IN1, MOTOR1_IN3, MOTOR1_IN2, MOTOR1_IN4);
AccelStepper motor2(AccelStepper::FULL4WIRE, MOTOR2_IN1, MOTOR2_IN3, MOTOR2_IN2, MOTOR2_IN4);
AccelStepper motor3(AccelStepper::FULL4WIRE, MOTOR3_IN1, MOTOR3_IN3, MOTOR3_IN2, MOTOR3_IN4);
AccelStepper motor4(AccelStepper::FULL4WIRE, MOTOR4_IN1, MOTOR4_IN3, MOTOR4_IN2, MOTOR4_IN4);

static uint32_t lastSeq = 0;
static unsigned long lastDebug = 0;

void setupMotor(AccelStepper &m) {
  m.setMaxSpeed(MAX_SPEED_THROTTLE);
  m.setAcceleration(400); // valor moderado; pode ajustar depois
  m.setSpeed(0);
}

void applySpeeds(int s1, int s2, int s3, int s4) {
  motor1.setSpeed(s1);
  motor2.setSpeed(s2);
  motor3.setSpeed(s3);
  motor4.setSpeed(s4);
}

void allStop() {
  applySpeeds(0,0,0,0);
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("\n=== Motor Node Receiver Init ===");
  Serial.printf("Pins M1 %d,%d,%d,%d | M2 %d,%d,%d,%d | M3 %d,%d,%d,%d | M4 %d,%d,%d,%d\n",
    MOTOR1_IN1, MOTOR1_IN2, MOTOR1_IN3, MOTOR1_IN4,
    MOTOR2_IN1, MOTOR2_IN2, MOTOR2_IN3, MOTOR2_IN4,
    MOTOR3_IN1, MOTOR3_IN2, MOTOR3_IN3, MOTOR3_IN4,
    MOTOR4_IN1, MOTOR4_IN2, MOTOR4_IN3, MOTOR4_IN4);

  setupMotor(motor1);
  setupMotor(motor2);
  setupMotor(motor3);
  setupMotor(motor4);

  if (!protocolInitMotorNode()) {
    Serial.println("[ERROR] Protocolo falhou. Reiniciando em 3s...");
    delay(3000);
    ESP.restart();
  }
}

void loop() {
  int m1,m2,m3,m4; uint32_t seq; unsigned long age;
  bool have = protocolGetLatestSpeeds(m1,m2,m3,m4,seq,age);
  if (have && age < MOTOR_NODE_PACKET_TIMEOUT_MS) {
    // Aplicar velocidades recebidas
    applySpeeds(m1,m2,m3,m4);
    lastSeq = seq;
  } else {
    // Sem pacote recente -> fail-safe
    allStop();
  }

  // Rodar motores
  motor1.runSpeed();
  motor2.runSpeed();
  motor3.runSpeed();
  motor4.runSpeed();

  unsigned long now = millis();
  if (now - lastDebug > 1000) {
    Serial.printf("SEQ:%lu age:%lums speeds:%d,%d,%d,%d\n", (unsigned long)lastSeq, have?age:999999UL, (int)motor1.speed(), (int)motor2.speed(), (int)motor3.speed(), (int)motor4.speed());
    lastDebug = now;
  }
}

#endif // ROLE_MOTOR_NODE
