/**
 * Motor Node Firmware (ESP32 dedicado aos 4 motores 28BYJ-48)
 * Teste inicial: gira cada motor em sequência e depois todos juntos.
 * Pinos escolhidos (evitando maioria dos strapping pins):
 *  M1: 13,14,16,17
 *  M2: 18,19,21,22
 *  M3: 23,25,26,27
 *  M4: 32,33,2,4   (2 e 4 são strapping; manter LOW no boot)
 *
 * Use este firmware para validar fiação antes de integrar comunicação.
 */

#ifdef ROLE_MOTOR_NODE

#include <Arduino.h>
#include <AccelStepper.h>
#include <WiFi.h>
#include "config.h"

// Definição dos motores (FULL4WIRE usa ordem IN1, IN3, IN2, IN4 na AccelStepper)
AccelStepper motor1(AccelStepper::FULL4WIRE, MOTOR_NODE_M1_IN1, MOTOR_NODE_M1_IN3, MOTOR_NODE_M1_IN2, MOTOR_NODE_M1_IN4);
AccelStepper motor2(AccelStepper::FULL4WIRE, MOTOR_NODE_M2_IN1, MOTOR_NODE_M2_IN3, MOTOR_NODE_M2_IN2, MOTOR_NODE_M2_IN4);
AccelStepper motor3(AccelStepper::FULL4WIRE, MOTOR_NODE_M3_IN1, MOTOR_NODE_M3_IN3, MOTOR_NODE_M3_IN2, MOTOR_NODE_M3_IN4);
AccelStepper motor4(AccelStepper::FULL4WIRE, MOTOR_NODE_M4_IN1, MOTOR_NODE_M4_IN3, MOTOR_NODE_M4_IN2, MOTOR_NODE_M4_IN4);

// Parâmetros de teste
const int TEST_SPEED = MOTOR_NODE_TEST_SPEED;      // steps/seg para teste (config.h)
const int TEST_DURATION = MOTOR_NODE_TEST_PHASE_MS;  // ms cada fase (config.h)
unsigned long phaseStart = 0;
int phase = 0;

void setupMotor(AccelStepper &m) {
  m.setMaxSpeed(800);
  m.setAcceleration(400);
  m.setSpeed(TEST_SPEED);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== Motor Node Test Init ===");
  Serial.printf("M1: %d,%d,%d,%d | M2: %d,%d,%d,%d | M3: %d,%d,%d,%d | M4: %d,%d,%d,%d\n",
    MOTOR_NODE_M1_IN1, MOTOR_NODE_M1_IN2, MOTOR_NODE_M1_IN3, MOTOR_NODE_M1_IN4,
    MOTOR_NODE_M2_IN1, MOTOR_NODE_M2_IN2, MOTOR_NODE_M2_IN3, MOTOR_NODE_M2_IN4,
    MOTOR_NODE_M3_IN1, MOTOR_NODE_M3_IN2, MOTOR_NODE_M3_IN3, MOTOR_NODE_M3_IN4,
    MOTOR_NODE_M4_IN1, MOTOR_NODE_M4_IN2, MOTOR_NODE_M4_IN3, MOTOR_NODE_M4_IN4);
  Serial.println("Certifique-se de fonte 5V >=2A e GND comum.");

  // Mostrar MAC address (STA) para uso no controller
  WiFi.mode(WIFI_STA);
  String mac = WiFi.macAddress();
  Serial.print("Motor Node STA MAC: ");
  Serial.println(mac);

  setupMotor(motor1);
  setupMotor(motor2);
  setupMotor(motor3);
  setupMotor(motor4);

  phaseStart = millis();
  phase = 0;
}

void allStop() {
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);
}

void setForward(AccelStepper &m) { m.setSpeed(TEST_SPEED); }
void setBackward(AccelStepper &m) { m.setSpeed(-TEST_SPEED); }

void loop() {
  unsigned long now = millis();

  // Sequência de fases:
  // 0: M1 fwd
  // 1: M1 bwd
  // 2: M2 fwd
  // 3: M2 bwd
  // 4: M3 fwd
  // 5: M3 bwd
  // 6: M4 fwd
  // 7: M4 bwd
  // 8: Todos fwd
  // 9: Todos bwd
  // 10+: loop reinicia

  if (now - phaseStart > TEST_DURATION) {
    phase++;
    phaseStart = now;
    if (phase > 9) phase = 0;
    Serial.printf("Fase %d\n", phase);
  }

  // Reset speeds antes de definir nova fase
  allStop();

  switch (phase) {
    case 0: setForward(motor1); break;
    case 1: setBackward(motor1); break;
    case 2: setForward(motor2); break;
    case 3: setBackward(motor2); break;
    case 4: setForward(motor3); break;
    case 5: setBackward(motor3); break;
    case 6: setForward(motor4); break;
    case 7: setBackward(motor4); break;
    case 8: setForward(motor1); setForward(motor2); setForward(motor3); setForward(motor4); break;
    case 9: setBackward(motor1); setBackward(motor2); setBackward(motor3); setBackward(motor4); break;
  }

  // Rodar motores
  motor1.runSpeed();
  motor2.runSpeed();
  motor3.runSpeed();
  motor4.runSpeed();

  // Debug reduzido
  static unsigned long lastDbg = 0;
  if (now - lastDbg > 1000) {
    Serial.printf("Speeds M1:%d M2:%d M3:%d M4:%d\n", (int)motor1.speed(), (int)motor2.speed(), (int)motor3.speed(), (int)motor4.speed());
    lastDbg = now;
  }
}

#endif // ROLE_MOTOR_NODE