/**
 * ========================================
 * IMPLEMENTAÇÃO DO CONTROLE DO SISTEMA
 * ========================================
 */

#include "system_control.h"
#include "config.h"
#include "display.h"
#include "motor_control.h"
#include <ps5Controller.h>

// Constantes
const unsigned long CONNECTION_ATTEMPT_INTERVAL = 5000; // 5 segundos entre tentativas

// Variáveis exportadas
bool wasConnected = false;
unsigned long lastConnectionCheck = 0;
unsigned long lastConnectionAttempt = 0;

/**
 * Gerencia a sequência de desligamento do sistema
 */
void handleShutdown() {
  Serial.println("\n*** Botão Options pressionado - Iniciando desligamento ***");

  // Parar todos os motores
  motor1Speed = 0;
  motor2Speed = 0;
  motor3Speed = 0;
  motor4Speed = 0;
  controlMotor1(0, 0);
  controlMotor2(0);
  controlMotor3(0);
  controlMotor4(false, false);

  // Mostrar mensagem de desligamento do DualSense no LCD
  showDisconnectingMessage();
  Serial.println("Desconectando DualSense...");
  delay(1500);

  // Desconectar o controle
  ps5.end();
  Serial.println("DualSense desconectado!");
  delay(500);

  // Mostrar mensagem de reinício do sistema
  showRestartingMessage();
  Serial.println("Reiniciando sistema...");
  delay(1500);

  // Desligar o display
  turnOffDisplay();
  Serial.println("Sistema desligado! Reiniciando ESP32...");
  delay(500);

  // Reiniciar o ESP32 para evitar reconexão automática do DualSense
  ESP.restart();
}

/**
 * Gerencia detecção de nova conexão
 */
void handleConnection() {
  Serial.println("\n*** Controle conectado! ***");
  wasConnected = true;
  delay(100); // Pequeno delay para estabilizar conexão
}

/**
 * Gerencia desconexão do controle
 */
void handleDisconnection() {
  Serial.println("\n*** Controle desconectado! ***");
  wasConnected = false;

  // Parar motores imediatamente
  motor1Speed = 0;
  motor2Speed = 0;
  motor3Speed = 0;
  motor4Speed = 0;
  controlMotor1(0, 0);
  controlMotor2(0);
  controlMotor3(0);
  controlMotor4(false, false);
}

/**
 * Gerencia estado de espera por conexão
 */
void handleWaitingState() {
  unsigned long currentTime = millis();

  if (currentTime - lastConnectionCheck > 5000) {
    showWaitingMessage();
    updateConnectionStatus(false, 0);

    Serial.print("Aguardando conexão... (próxima tentativa em ");
    Serial.print((CONNECTION_ATTEMPT_INTERVAL - (currentTime - lastConnectionAttempt)) / 1000);
    Serial.println("s)");

    lastConnectionCheck = currentTime;
  }
}

/**
 * Processa entradas do controle (4 motores independentes)
 */
void processControllerInputs() {
  // Motor 1: R2/L2
  controlMotor1(ps5.R2Value(), ps5.L2Value());

  // Motor 2: analógico esquerdo
  controlMotor2(ps5.LStickX());

  // Motor 3: analógico direito
  controlMotor3(ps5.RStickX());

  // Motor 4: L1/R1
  controlMotor4(ps5.L1(), ps5.R1());

  // Executar movimento dos motores
  runMotors();

  // Atualizar display com status dos 4 motores
  updateAllMotorsDisplay(motor1Speed, motor2Speed, motor3Speed, motor4Speed);
}

/**
 * Atualiza status do sistema (bateria, display, debug)
 */
void updateSystemStatus() {
  // Atualizar bateria no display
  uint8_t battery = ps5.Battery();       // 0-15
  battery = map(battery, 0, 15, 0, 100); // Converte para porcentagem
  updateConnectionStatus(true, battery);

  // Debug no serial (throttling)
  static unsigned long lastDebug = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastDebug > SERIAL_DEBUG_INTERVAL) {
    Serial.print("M1:");
    Serial.print(motor1Speed);
    Serial.print(" M2:");
    Serial.print(motor2Speed);
    Serial.print(" M3:");
    Serial.print(motor3Speed);
    Serial.print(" M4:");
    Serial.println(motor4Speed);

    lastDebug = currentTime;
  }
}
