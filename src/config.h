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
// Endereço MAC do DualSense (definido diretamente no código)
// Para alterar, modifique a string abaixo.
#define PS5_MAC_ADDRESS "XX:XX:XX:XX:XX:XX" // Endereço MAC mascarado

// Endereço MAC do motor-node (mascarado)
#define MOTOR_NODE_MAC_STR "YY:YY:YY:YY:YY:YY" // Endereço MAC mascarado

// ========================================
// PINOS DOS MOTORES DE PASSO (NÓ DE MOTORES - 4x 28BYJ-48)
// Unificado: estes pinos refletem a fiação real do motor-node.
// Ordem física IN1..IN4 listada; AccelStepper (FULL4WIRE) usa (IN1, IN3, IN2, IN4) na construção.
// M1
#define MOTOR1_IN1 13
#define MOTOR1_IN2 14
#define MOTOR1_IN3 16
#define MOTOR1_IN4 17
// M2
#define MOTOR2_IN1 18
#define MOTOR2_IN2 19
#define MOTOR2_IN3 21
#define MOTOR2_IN4 22
// M3
#define MOTOR3_IN1 23
#define MOTOR3_IN2 25
#define MOTOR3_IN3 26
#define MOTOR3_IN4 27
// M4 (contém pinos strapping 2 e 4 - manter LOW no boot)
#define MOTOR4_IN1 32
#define MOTOR4_IN2 33
#define MOTOR4_IN3 2
#define MOTOR4_IN4 4

// ========================================
// CONFIGURAÇÕES DO LCD I2C
// ========================================
#define LCD_SDA 21 // SDA padrão do ESP32 (placa do controlador)
#define LCD_SCL 22 // SCL padrão do ESP32 (placa do controlador)
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 20
#define LCD_ROWS 4

// ========================================
// CONFIGURAÇÕES DO MOTOR 28BYJ-48
// ========================================
// Especificações do motor:
// - Steps por revolução: 2048 (com redução 1/64)
// - Velocidade máxima recomendada: 600-800 steps/segundo
// - Torque ótimo: 200-400 steps/segundo

#define MAX_SPEED_THROTTLE 600 // Velocidade máxima do throttle (steps/segundo)
#define MAX_SPEED_STEERING MAX_SPEED_THROTTLE // Velocidade máxima da direção igual ao throttle

// ========================================
// CONFIGURAÇÕES DE ATUALIZAÇÃO
// ========================================
#define LCD_UPDATE_INTERVAL 200   // Intervalo de atualização do LCD (ms)
#define SERIAL_DEBUG_INTERVAL 500 // Intervalo de debug serial (ms)
#define ANALOG_DEADZONE 10        // Zona morta do analógico
#define LCD_UPDATE_THRESHOLD 20   // Diferença mínima para atualizar LCD

// ========================================
// CONFIGURAÇÕES DO CONTROLADOR (ENV ROLE_CONTROLLER)
// ========================================
#define CONTROLLER_SEND_INTERVAL 50      // Intervalo de envio de velocidades (ms)
#define CONTROLLER_DISPLAY_INTERVAL 200  // Intervalo de refresh de display (ms)
#define BATTERY_RAW_MAX 15               // Valor máximo bruto da bateria PS5 (0-15)

// ========================================
// CONFIGURAÇÕES ESP-NOW
// ========================================
#define ESP_NOW_CHANNEL 0                // Canal padrão (0 = auto)

// ========================================
// Parâmetros de teste (podem ser removidos após integração completa)
#define MOTOR_NODE_TEST_SPEED 300      // steps/s para modo de teste
#define MOTOR_NODE_TEST_PHASE_MS 2000  // ms cada fase no teste

// Fail-safe de comunicação ESP-NOW (nó de motores)
#define MOTOR_NODE_PACKET_TIMEOUT_MS 500  // Se não receber pacote por este tempo, zera velocidades

#endif // CONFIG_H
