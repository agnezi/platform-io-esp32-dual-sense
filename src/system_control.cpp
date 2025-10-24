/**
 * ========================================
 * IMPLEMENTAÇÃO DO CONTROLE DO SISTEMA
 * ========================================
 */

#include "system_control.h"
#include "config.h"
#include "motor_control.h"
#include "display.h"
#include <ps5Controller.h>

// Constantes
const unsigned long CONNECTION_ATTEMPT_INTERVAL = 5000;  // 5 segundos entre tentativas

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
    currentSpeed = 0;
    steeringSpeed = 0;
    controlThrottle(0, 0);
    controlSteering(0);
    
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
    currentSpeed = 0;
    steeringSpeed = 0;
    controlThrottle(0, 0);
    controlSteering(0);
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
 * Processa entradas do controle (gatilhos e analógicos)
 */
void processControllerInputs() {
    // Ler valores dos gatilhos R2/L2
    int r2Pressure = ps5.R2Value();  // 0-255
    int l2Pressure = ps5.L2Value();  // 0-255
    
    // Controlar throttle (motores traseiros)
    controlThrottle(r2Pressure, l2Pressure);

    // Ler valor do analógico esquerdo (eixo X)
    int leftStickX = ps5.LStickX();  // -128 a 127
    
    // Controlar direção (motores dianteiros)
    controlSteering(leftStickX);

    // Executar movimento dos motores
    runMotors();

    // Atualizar display com status
    updateDisplay(currentSpeed, steeringSpeed, r2Pressure, l2Pressure);
}

/**
 * Atualiza status do sistema (bateria, display, debug)
 */
void updateSystemStatus() {
    // Atualizar bateria no display
    uint8_t battery = ps5.Battery();  // 0-15
    battery = map(battery, 0, 15, 0, 100);  // Converte para porcentagem
    updateConnectionStatus(true, battery);

    // Debug no serial (throttling)
    static unsigned long lastDebug = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastDebug > SERIAL_DEBUG_INTERVAL) {
        Serial.print("T:");
        Serial.print(currentSpeed);
        Serial.print(" S:");
        Serial.println(steeringSpeed);
        
        lastDebug = currentTime;
    }
}
