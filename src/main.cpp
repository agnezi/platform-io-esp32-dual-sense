// /**
//  * ========================================
//  * CARRINHO RC COM PS5 DUALSENSE
//  * ========================================
//  *
//  * Sistema de controle de carrinho RC usando:
//  * - Controlador PS5 DualSense
//  * - Motores de passo 28BYJ-48 com drivers ULN2003
//  * - Display LCD 16x2 I2C
//  *
//  * Controles:
//  * - R2: Acelerar para frente (proporcional)
//  * - L2: Acelerar para trás (proporcional)
//  * - Analógico Esquerdo (eixo X): Controlar direção
//  * - Options: Desligar sistema e reiniciar ESP32
//  */

// #include "config.h"
// #include "display.h"
// #include "motor_control.h"
// #include "system_control.h"
// #include <Wire.h>
// #include <ps5Controller.h>

// // ========================================
// // CONFIGURAÇÃO INICIAL
// // ========================================
// void setup() {
//   // Inicializar comunicação serial
//   Serial.begin(115200);
//   delay(1000);

//   Serial.println("\n========================================");
//   Serial.println("Sistema de Controle RC Iniciando...");
//   Serial.println("========================================\n");

//   // Inicializar componentes
//   initDisplay();
//   showWaitingMessage();

//   ps5.begin(PS5_MAC_ADDRESS);
//   Serial.print("Aguardando controle PS5: ");
//   Serial.println(PS5_MAC_ADDRESS);

//   initMotors();

//   Serial.println("Setup completo!");
// }

// // ========================================
// // LOOP PRINCIPAL
// // ========================================
// void loop() {
//   bool isConnected = ps5.isConnected();

//   // Throttling de verificações quando desconectado
//   if (!isConnected && !wasConnected) {
//     unsigned long currentTime = millis();

//     if (currentTime - lastConnectionAttempt < 5000) {
//       delay(100);
//       yield();
//       return;
//     }

//     lastConnectionAttempt = currentTime;
//   }

//   // Processamento baseado no estado de conexão
//   if (isConnected) {
//     // Detectar nova conexão
//     if (!wasConnected) {
//       handleConnection();
//     }

//     // Verificar botão de desligamento
//     if (ps5.Options()) {
//       handleShutdown();
//       return;
//     }

//     // Processar controles dos 4 motores
//     controlMotor1(ps5.R2Value(), ps5.L2Value());
//     controlMotor2(ps5.LStickX());
//     controlMotor3(ps5.RStickX());
//     controlMotor4(ps5.L1(), ps5.R1());
//     runMotors();

//     // Atualizar status do sistema (display, bateria, debug)
//     updateSystemStatus();

//     yield();
//   } else {
//     // Gerenciar desconexão
//     if (wasConnected) {
//       handleDisconnection();
//     }

//     // Mostrar estado de espera
//     handleWaitingState();

//     delay(100);
//     yield();
//   }

//   delay(1);
// }


#include <Arduino.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(500);
  WiFi.mode(WIFI_STA);            // Ensure we're in station mode
  Serial.println();
  Serial.print("ESP-NOW (STA) MAC: ");
  Serial.println(WiFi.macAddress());  
}

void loop() {
  // Nothing
}