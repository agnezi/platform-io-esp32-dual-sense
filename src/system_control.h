/**
 * ========================================
 * CONTROLE DO SISTEMA
 * ========================================
 * 
 * Funções auxiliares para gerenciar o sistema
 */

#ifndef SYSTEM_CONTROL_H
#define SYSTEM_CONTROL_H

#include <Arduino.h>

// Funções públicas
void handleShutdown();              // Gerencia sequência de desligamento
void handleConnection();            // Gerencia detecção de conexão
void handleDisconnection();         // Gerencia desconexão do controle
void handleWaitingState();          // Gerencia estado de espera por conexão
void processControllerInputs();     // Processa entradas do controle
void updateSystemStatus();          // Atualiza status do sistema (display, bateria, etc)

// Variáveis externas
extern bool wasConnected;
extern unsigned long lastConnectionCheck;
extern unsigned long lastConnectionAttempt;

#endif // SYSTEM_CONTROL_H
