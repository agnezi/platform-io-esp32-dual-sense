/**
 * ========================================
 * IMPLEMENTAÇÃO DO CONTROLE DE MOTORES
 * ========================================
 */

#include "motor_control.h"
#include "config.h"
#include <Arduino.h>

// Instâncias dos motores AccelStepper
AccelStepper motor1(AccelStepper::FULL4WIRE, MOTOR1_IN1, MOTOR1_IN3, MOTOR1_IN2, MOTOR1_IN4); // Motor 1: R2/L2
AccelStepper motor2(AccelStepper::FULL4WIRE, MOTOR2_IN1, MOTOR2_IN3, MOTOR2_IN2, MOTOR2_IN4); // Motor 2: analógico esquerdo
AccelStepper motor3(AccelStepper::FULL4WIRE, MOTOR3_IN1, MOTOR3_IN3, MOTOR3_IN2, MOTOR3_IN4); // Motor 3: analógico direito
AccelStepper motor4(AccelStepper::FULL4WIRE, MOTOR4_IN1, MOTOR4_IN3, MOTOR4_IN2, MOTOR4_IN4); // Motor 4: L1/R1

// Variáveis globais
int motor1Speed = 0;
int motor2Speed = 0;
int motor3Speed = 0;
int motor4Speed = 0;

/**
 * Inicializa os motores de passo
 * Configura velocidade máxima e aceleração
 */
void initMotors() {
  motor1.setMaxSpeed(1000.0);
  motor1.setAcceleration(500.0);
  motor2.setMaxSpeed(1000.0);
  motor2.setAcceleration(500.0);
  motor3.setMaxSpeed(1000.0);
  motor3.setAcceleration(500.0);
  motor4.setMaxSpeed(1000.0);
  motor4.setAcceleration(500.0);
  Serial.println("Motores inicializados!");
}

/**
 * Controla o throttle usando os gatilhos R2/L2
 * R2 = frente, L2 = ré
 * Mapeia a pressão (0-255) para velocidade proporcional
 */
void controlMotor1(int r2Pressure, int l2Pressure) {
  if (r2Pressure > 0) {
    motor1Speed = map(r2Pressure, 0, 255, 0, MAX_SPEED_THROTTLE);
  } else if (l2Pressure > 0) {
    motor1Speed = -map(l2Pressure, 0, 255, 0, MAX_SPEED_THROTTLE);
  } else {
    motor1Speed = 0;
  }
  motor1.setSpeed(motor1Speed);
}

/**
 * Controla a direção usando o analógico esquerdo (eixo X)
 * Esquerda = negativo, Direita = positivo
 * Inclui zona morta para evitar drift
 */
void controlMotor2(int leftStickX) {
  int tempSpeed;
  if (leftStickX < 0) {
    tempSpeed = -map(abs(leftStickX), 0, 128, 0, MAX_SPEED_STEERING);
  } else {
    tempSpeed = map(leftStickX, 0, 127, 0, MAX_SPEED_STEERING);
  }
  if (abs(leftStickX) < ANALOG_DEADZONE) {
    tempSpeed = 0;
  }
  motor2Speed = tempSpeed;
  motor2.setSpeed(motor2Speed);
}

void controlMotor3(int rightStickX) {
  int tempSpeed;
  if (rightStickX < 0) {
    tempSpeed = -map(abs(rightStickX), 0, 128, 0, MAX_SPEED_STEERING);
  } else {
    tempSpeed = map(rightStickX, 0, 127, 0, MAX_SPEED_STEERING);
  }
  if (abs(rightStickX) < ANALOG_DEADZONE) {
    tempSpeed = 0;
  }
  motor3Speed = tempSpeed;
  motor3.setSpeed(motor3Speed);
}

void controlMotor4(bool l1, bool r1) {
  if (l1 && !r1) {
    motor4Speed = MAX_SPEED_THROTTLE;
  } else if (r1 && !l1) {
    motor4Speed = -MAX_SPEED_THROTTLE;
  } else {
    motor4Speed = 0;
  }
  motor4.setSpeed(motor4Speed);
}

/**
 * Executa o movimento dos motores
 * Deve ser chamado continuamente no loop principal
 */
void runMotors() {
  motor1.runSpeed();
  motor2.runSpeed();
  motor3.runSpeed();
  motor4.runSpeed();
}
