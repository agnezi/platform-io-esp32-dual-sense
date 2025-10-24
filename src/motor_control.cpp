/**
 * ========================================
 * IMPLEMENTAÇÃO DO CONTROLE DE MOTORES
 * ========================================
 */

#include "motor_control.h"
#include "config.h"
#include <Arduino.h>

// Instâncias dos motores AccelStepper
AccelStepper rearMotor(AccelStepper::FULL4WIRE, REAR_IN1, REAR_IN3, REAR_IN2, REAR_IN4);
AccelStepper frontMotor(AccelStepper::FULL4WIRE, FRONT_IN1, FRONT_IN3, FRONT_IN2, FRONT_IN4);

// Variáveis globais
int currentSpeed = 0;
int steeringSpeed = 0;

/**
 * Inicializa os motores de passo
 * Configura velocidade máxima e aceleração
 */
void initMotors() {
    // Configurar motor traseiro (throttle)
    rearMotor.setMaxSpeed(1000.0);
    rearMotor.setAcceleration(500.0);
    
    // Configurar motor dianteiro (direção)
    frontMotor.setMaxSpeed(1000.0);
    frontMotor.setAcceleration(500.0);
    
    Serial.println("Motores inicializados!");
}

/**
 * Controla o throttle usando os gatilhos R2/L2
 * R2 = frente, L2 = ré
 * Mapeia a pressão (0-255) para velocidade proporcional
 */
void controlThrottle(int r2Pressure, int l2Pressure) {
    if (r2Pressure > 0) {
        // Acelerar para frente
        currentSpeed = map(r2Pressure, 0, 255, 0, MAX_SPEED_THROTTLE);
    } else if (l2Pressure > 0) {
        // Acelerar para trás (velocidade negativa)
        currentSpeed = -map(l2Pressure, 0, 255, 0, MAX_SPEED_THROTTLE);
    } else {
        // Sem aceleração
        currentSpeed = 0;
    }
    
    // Aplicar velocidade ao motor traseiro (com proteção)
    if (rearMotor.maxSpeed() > 0) {
        rearMotor.setSpeed(currentSpeed);
    }
}

/**
 * Controla a direção usando o analógico esquerdo (eixo X)
 * Esquerda = negativo, Direita = positivo
 * Inclui zona morta para evitar drift
 */
void controlSteering(int leftStickX) {
    int tempSpeed;
    
    if (leftStickX < 0) {
        // Virar para esquerda
        tempSpeed = -map(abs(leftStickX), 0, 128, 0, MAX_SPEED_STEERING);
    } else {
        // Virar para direita
        tempSpeed = map(leftStickX, 0, 127, 0, MAX_SPEED_STEERING);
    }
    
    // Aplicar zona morta (deadzone)
    if (abs(leftStickX) < ANALOG_DEADZONE) {
        tempSpeed = 0;
    }
    
    steeringSpeed = tempSpeed;
    
    // Aplicar velocidade ao motor dianteiro (com proteção)
    if (frontMotor.maxSpeed() > 0) {
        frontMotor.setSpeed(steeringSpeed);
    }
}

/**
 * Executa o movimento dos motores
 * Deve ser chamado continuamente no loop principal
 */
void runMotors() {
    rearMotor.runSpeed();
    frontMotor.runSpeed();
}
