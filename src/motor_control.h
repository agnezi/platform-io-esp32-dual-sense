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
extern int currentSpeed;      // Velocidade atual do throttle
extern int steeringSpeed;     // Velocidade atual da direção

// Funções públicas
void initMotors();                              // Inicializa os motores
void controlThrottle(int r2, int l2);           // Controla throttle (R2/L2)
void controlSteering(int leftStickX);           // Controla direção (analógico)
void runMotors();                               // Executa movimento dos motores

#endif // MOTOR_CONTROL_H
