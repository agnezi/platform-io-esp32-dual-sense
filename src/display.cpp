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
    // Inicializar I2C com pinos customizados
    Wire.begin(LCD_SDA, LCD_SCL);
    
    lcd.init();
    lcd.backlight();
    Serial.print("Display LCD inicializado nos pinos SDA:");
    Serial.print(LCD_SDA);
    Serial.print(" SCL:");
    Serial.println(LCD_SCL);
}

/**
 * Mostra mensagem de aguardando controle
 */
void showWaitingMessage() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("4-Motor Controller");
    lcd.setCursor(0, 1);
    lcd.print("Waiting for");
    lcd.setCursor(0, 2);
    lcd.print("DualSense...");
    lcd.setCursor(0, 3);
    lcd.print("MAC: Last 6 digits");
}

/**
 * Atualiza o display com status dos 4 motores
 * LCD 20x4 - Uma linha para cada motor
 * Linha 1: Motor 1 (R2/L2)
 * Linha 2: Motor 2 (Analog Left)
 * Linha 3: Motor 3 (Analog Right)  
 * Linha 4: Motor 4 (L1/R1) + Bateria
 */
void updateDisplay(int motor1Speed, int motor2Speed, int r2, int l2) {
    static int lastM1 = 999999;
    static int lastM2 = 999999;
    static unsigned long lastUpdate = 0;
    unsigned long currentTime = millis();
    
    // Atualizar a cada intervalo ou mudança significativa
    if ((currentTime - lastUpdate > LCD_UPDATE_INTERVAL) && 
        (abs(motor1Speed - lastM1) > LCD_UPDATE_THRESHOLD || 
         abs(motor2Speed - lastM2) > LCD_UPDATE_THRESHOLD)) {
        
        // Linha 1: Motor 1 (R2/L2)
        lcd.setCursor(0, 0);
        lcd.print("M1(R2/L2): ");
        if (motor1Speed > 0) {
            lcd.print("FWD ");
            lcd.print(abs(motor1Speed));
        } else if (motor1Speed < 0) {
            lcd.print("BWD ");
            lcd.print(abs(motor1Speed));
        } else {
            lcd.print("STOP    ");
        }
        
        // Linha 2: Motor 2 (Analógico Esquerdo)
        lcd.setCursor(0, 1);
        lcd.print("M2(LS): ");
        if (motor2Speed > 0) {
            lcd.print("R ");
            lcd.print(motor2Speed);
            lcd.print("     ");
        } else if (motor2Speed < 0) {
            lcd.print("L ");
            lcd.print(abs(motor2Speed));
            lcd.print("     ");
        } else {
            lcd.print("CENTER   ");
        }
        
        lastM1 = motor1Speed;
        lastM2 = motor2Speed;
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
 * Atualiza display com todos os 4 motores (LCD 20x4)
 */
void updateAllMotorsDisplay(int m1, int m2, int m3, int m4) {
    static int lastM1 = 999999, lastM2 = 999999, lastM3 = 999999, lastM4 = 999999;
    static unsigned long lastUpdate = 0;
    unsigned long currentTime = millis();
    
    // Atualizar apenas se houver mudança significativa
    if ((currentTime - lastUpdate > LCD_UPDATE_INTERVAL) && 
        (abs(m1 - lastM1) > LCD_UPDATE_THRESHOLD || 
         abs(m2 - lastM2) > LCD_UPDATE_THRESHOLD ||
         abs(m3 - lastM3) > LCD_UPDATE_THRESHOLD ||
         abs(m4 - lastM4) > LCD_UPDATE_THRESHOLD)) {
        
        // Linha 1: Motor 1 (R2/L2)
        lcd.setCursor(0, 0);
        lcd.print("M1(R2/L2):");
        if (m1 > 0) {
            lcd.print("FWD ");
            lcd.print(abs(m1));
        } else if (m1 < 0) {
            lcd.print("BWD ");
            lcd.print(abs(m1));
        } else {
            lcd.print("STOP   ");
        }
        
        // Linha 2: Motor 2 (Analógico Esquerdo)
        lcd.setCursor(0, 1);
        lcd.print("M2(LS):");
        if (m2 > 0) {
            lcd.print("R ");
            lcd.print(m2);
            lcd.print("      ");
        } else if (m2 < 0) {
            lcd.print("L ");
            lcd.print(abs(m2));
            lcd.print("      ");
        } else {
            lcd.print("CENTER    ");
        }
        
        // Linha 3: Motor 3 (Analógico Direito)
        lcd.setCursor(0, 2);
        lcd.print("M3(RS):");
        if (m3 > 0) {
            lcd.print("R ");
            lcd.print(m3);
            lcd.print("      ");
        } else if (m3 < 0) {
            lcd.print("L ");
            lcd.print(abs(m3));
            lcd.print("      ");
        } else {
            lcd.print("CENTER    ");
        }
        
        // Linha 4: Motor 4 (L1/R1)
        lcd.setCursor(0, 3);
        lcd.print("M4(L1/R1):");
        if (m4 > 0) {
            lcd.print("FWD ");
            lcd.print(abs(m4));
        } else if (m4 < 0) {
            lcd.print("BWD ");
            lcd.print(abs(m4));
        } else {
            lcd.print("STOP   ");
        }
        
        lastM1 = m1; lastM2 = m2; lastM3 = m3; lastM4 = m4;
        lastUpdate = currentTime;
    }
}

/**
 * Desliga o display (limpa tela e apaga backlight)
 */
void turnOffDisplay() {
    lcd.clear();
    lcd.noBacklight();
}
