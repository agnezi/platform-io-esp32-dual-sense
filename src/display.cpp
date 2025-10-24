/**
 * ========================================
 * IMPLEMENTAÇÃO DO CONTROLE DE DISPLAY
 * ========================================
 */

#include "display.h"
#include "config.h"
#include <Arduino.h>

// Instância do LCD I2C
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

/**
 * Inicializa o display LCD
 */
void initDisplay() {
    lcd.init();
    lcd.backlight();
    Serial.println("Display LCD inicializado!");
}

/**
 * Mostra mensagem de aguardando controle
 */
void showWaitingMessage() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting for");
    lcd.setCursor(0, 1);
    lcd.print("controller...");
}

/**
 * Atualiza o display com status de throttle e direção
 * Linha 1: Status do throttle (T:FWD/BWD/STOP + %)
 * Linha 2: Status da direção (S:L/R/CENTER + velocidade) + Bateria
 * 
 * Otimizado para atualizar apenas quando houver mudança significativa
 */
void updateDisplay(int speed, int steering, int r2, int l2) {
    static int lastSpeed = 999999;
    static int lastSteering = 999999;
    static unsigned long lastUpdate = 0;
    unsigned long currentTime = millis();
    
    // Atualizar apenas a cada intervalo definido ou quando houver mudança significativa
    if ((currentTime - lastUpdate > LCD_UPDATE_INTERVAL) && 
        (abs(speed - lastSpeed) > LCD_UPDATE_THRESHOLD || 
         abs(steering - lastSteering) > LCD_UPDATE_THRESHOLD)) {
        
        lcd.clear();
        
        // Linha 1: Status do throttle
        lcd.setCursor(0, 0);
        if (speed > 0) {
            lcd.print("T:FWD ");
            int percent = map(r2, 0, 255, 0, 100);
            if (percent < 10) lcd.print(" ");
            lcd.print(percent);
            lcd.print("%");
        } else if (speed < 0) {
            lcd.print("T:BWD ");
            int percent = map(l2, 0, 255, 0, 100);
            if (percent < 10) lcd.print(" ");
            lcd.print(percent);
            lcd.print("%");
        } else {
            lcd.print("T:STOP");
        }
        
        // Linha 2: Status da direção (primeiras 10 colunas, deixa espaço para bateria)
        lcd.setCursor(0, 1);
        if (abs(steering) > 10) {
            lcd.print("S:");
            lcd.print(steering < 0 ? "L" : "R");
            lcd.print(" ");
            // Limitar steering para não ocupar muito espaço
            int steerDisplay = abs(steering);
            if (steerDisplay > 999) steerDisplay = 999;
            lcd.print(steerDisplay);
        } else {
            lcd.print("S:CENTER");
        }
        
        lastSpeed = speed;
        lastSteering = steering;
        lastUpdate = currentTime;
    }
}

/**
 * Atualiza status da bateria no display
 * Linha 2 (final): Bateria - "XX%" onde XX é a porcentagem
 */
void updateConnectionStatus(bool isConnected, uint8_t battery) {
    // Linha 2: Bateria (colunas 11-15)
    lcd.setCursor(11, 1);
    
    if (isConnected) {
        // Garantir que bateria está entre 0-100
        if (battery > 100) battery = 100;
        
        // Formatar bateria com 2 ou 3 dígitos + %
        if (battery < 10) {
            lcd.print(" ");
        }
        lcd.print(battery);
        lcd.print("%");
    } else {
        lcd.print(" --%");  // Desconectado
    }
}

/**
 * Mostra mensagem de desconexão do DualSense
 */
void showDisconnectingMessage() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Disconnecting");
    lcd.setCursor(0, 1);
    lcd.print("DualSense...");
}

/**
 * Mostra mensagem de desligamento do LCD
 */
void showLCDOffMessage() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turning off");
    lcd.setCursor(0, 1);
    lcd.print("LCD display...");
}

/**
 * Mostra mensagem de reinício do sistema
 */
void showRestartingMessage() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Restarting");
    lcd.setCursor(0, 1);
    lcd.print("system...");
}

/**
 * Desliga o display (limpa tela e apaga backlight)
 */
void turnOffDisplay() {
    lcd.clear();
    lcd.noBacklight();
}
