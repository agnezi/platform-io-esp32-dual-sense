/**
 * ========================================
 * CONTROLE DE MOTORES
 * ========================================
 *
 * Interface para controlar os motores de passo 28BYJ-48
 * usando a biblioteca AccelStepper.
 */

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <AccelStepper.h>

// Variáveis globais exportadas
extern int motor1Speed; // Motor 1: R2/L2
extern int motor2Speed; // Motor 2: analógico esquerdo
extern int motor3Speed; // Motor 3: analógico direito
extern int motor4Speed; // Motor 4: L1/R1

// Funções públicas
void initMotors();
void controlMotor1(int r2, int l2);   // Motor 1: R2/L2
void controlMotor2(int leftStickX);   // Motor 2: analógico esquerdo
void controlMotor3(int rightStickX);  // Motor 3: analógico direito
void controlMotor4(bool l1, bool r1); // Motor 4: L1/R1
void runMotors();

#endif // MOTOR_CONTROL_H
