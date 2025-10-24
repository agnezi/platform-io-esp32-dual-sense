/**
 * ========================================
 * CONTROLE DO DISPLAY LCD
 * ========================================
 * 
 * Interface para gerenciar o display LCD 16x2 I2C
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <LiquidCrystal_I2C.h>

// Funções públicas
void initDisplay();                                     // Inicializa o display
void showWaitingMessage();                              // Mostra mensagem de espera
void updateDisplay(int speed, int steering, int r2, int l2);  // Atualiza display com status
void updateConnectionStatus(bool isConnected, uint8_t battery);  // Atualiza BT e bateria
void showDisconnectingMessage();                        // Mostra mensagem de desconexão do DualSense
void showLCDOffMessage();                               // Mostra mensagem de desligamento do LCD
void showRestartingMessage();                           // Mostra mensagem de reinício do sistema
void turnOffDisplay();                                  // Desliga o display (apaga backlight e limpa)

#endif // DISPLAY_H
