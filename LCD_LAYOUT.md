# 📺 Layout do Display LCD 16x2

## 🎯 Distribuição do Display

### Quando Conectado:

```
┌────────────────┐
│T:FWD 85%  BT:4│  <- Linha 1: Throttle + Sinal BT
│S:CENTER    72%│  <- Linha 2: Direção + Bateria
└────────────────┘
```

### Exemplos de Estados:

#### 1. Acelerando para Frente, Virando Direita
```
┌────────────────┐
│T:FWD 100% BT:4│
│S:R 450     95%│
└────────────────┘
```

#### 2. Acelerando para Trás, Virando Esquerda
```
┌────────────────┐
│T:BWD 65%  BT:3│
│S:L 320     82%│
└────────────────┘
```

#### 3. Parado, Centralizado, Sinal Fraco
```
┌────────────────┐
│T:STOP     BT:1│
│S:CENTER    45%│
└────────────────┘
```

#### 4. Desconectado
```
┌────────────────┐
│Waiting for     │
│controller...   │
└────────────────┘
```

Após 2 segundos desconectado:
```
┌────────────────┐
│Waiting for BT:X│
│controller... -%│
└────────────────┘
```

## 📊 Legenda dos Indicadores

### Linha 1 (Throttle + Bluetooth Signal):
- **T:FWD XX%** - Acelerando para frente (0-100%)
- **T:BWD XX%** - Acelerando para trás (0-100%)
- **T:STOP** - Parado
- **BT:0-4** - Nível de sinal Bluetooth:
  - **BT:4** - Excelente (≥ -50 dBm)
  - **BT:3** - Bom (-60 a -50 dBm)
  - **BT:2** - Médio (-70 a -60 dBm)
  - **BT:1** - Fraco (-80 a -70 dBm)
  - **BT:0** - Muito fraco (< -80 dBm)
  - **BT:X** - Desconectado

### Linha 2 (Direção + Bateria):
- **S:L XXX** - Virando para esquerda (velocidade em steps/s)
- **S:R XXX** - Virando para direita (velocidade em steps/s)
- **S:CENTER** - Direção centralizada
- **XX%** - Nível de bateria do DualSense (0-100%)
- **--%** - Bateria desconhecida (desconectado)

## 🔄 Atualização

- **Display Principal**: Atualiza a cada 200ms ou quando houver mudança > 20 steps/s
- **Status BT/Bateria**: Atualiza sempre que o display principal atualiza
- **Mensagem de Espera**: Atualiza a cada 2 segundos quando desconectado

## 💡 Dicas

1. **Sinal BT fraco (0-1)**: Aproxime o controle do ESP32
2. **Bateria baixa (< 20%)**: Conecte o DualSense no cabo USB para carregar
3. **BT:X aparecendo**: Verifique se o controle está ligado e pareado

## 📐 Mapa de Colunas

```
Coluna: 0123456789012345
Linha 1: T:FWD 100% BT:4
        └─────┬─────┘└─┬─┘
        Throttle   Bluetooth
        
Linha 2: S:CENTER    72%
        └────┬────┘└─┬─┘
        Direção   Bateria
```

### Espaçamento:
- **Colunas 0-10**: Informação principal (throttle/direção)
- **Coluna 11**: Espaço
- **Colunas 12-15**: Status (BT/bateria)

---

**Última atualização**: Outubro 2025
