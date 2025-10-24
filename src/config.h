/**
 * ========================================
 * CONFIGURAÇÕES DO PROJETO
 * ========================================
 * 
 * Arquivo centralizado com todas as configurações,
 * constantes e definições de pinos do sistema.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========================================
// ENDEREÇO MAC DO CONTROLE PS5
// ========================================
// Pode ser definido via variável de ambiente PS5_MAC_ADDRESS
// Exemplo: export PS5_MAC_ADDRESS="AA:BB:CC:DD:EE:FF"
#ifndef PS5_MAC_ADDRESS
#endif

// ========================================
// PINOS DOS MOTORES TRASEIROS
// Controlados por R2/L2 para throttle
// ========================================
#define REAR_IN1 25
#define REAR_IN2 26
#define REAR_IN3 27
#define REAR_IN4 14

// ========================================
// PINOS DOS MOTORES DIANTEIROS
// Controlados por analógico esquerdo para direção
// ========================================
#define FRONT_IN1 17
#define FRONT_IN2 16
#define FRONT_IN3 5
#define FRONT_IN4 4

// ========================================
// CONFIGURAÇÕES DO LCD
// ========================================
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// ========================================
// CONFIGURAÇÕES DO MOTOR 28BYJ-48
// ========================================
// Especificações do motor:
// - Steps por revolução: 2048 (com redução 1/64)
// - Velocidade máxima recomendada: 600-800 steps/segundo
// - Torque ótimo: 200-400 steps/segundo

#define MAX_SPEED_THROTTLE 600    // Velocidade máxima do throttle (steps/segundo)
#define MAX_SPEED_STEERING 600    // Velocidade máxima da direção (steps/segundo)

// ========================================
// CONFIGURAÇÕES DE ATUALIZAÇÃO
// ========================================
#define LCD_UPDATE_INTERVAL 200        // Intervalo de atualização do LCD (ms)
#define SERIAL_DEBUG_INTERVAL 500      // Intervalo de debug serial (ms)
#define ANALOG_DEADZONE 10             // Zona morta do analógico
#define LCD_UPDATE_THRESHOLD 20        // Diferença mínima para atualizar LCD

#endif // CONFIG_H
