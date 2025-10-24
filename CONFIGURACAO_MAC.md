# 🎮 Configuração do Endereço MAC do DualSense

## Método 1: Usando Variável de Ambiente (Recomendado)

### Passo 1: Descobrir o MAC do seu DualSense

**No macOS:**
```bash
# Abrir Preferências do Sistema > Bluetooth
# O endereço MAC aparece ao lado do nome do dispositivo
```

**No Windows:**
```bash
# Settings > Bluetooth & devices > Devices
# Clicar no DualSense e ver propriedades
```

**No Linux:**
```bash
bluetoothctl
devices
# Procurar por "Wireless Controller"
```

### Passo 2: Configurar a Variável de Ambiente

#### macOS / Linux (temporário - apenas para sessão atual):
```bash
export PS5_MAC_ADDRESS="AA:BB:CC:DD:EE:FF"
```

#### macOS / Linux (permanente):
Adicionar ao `~/.zshrc` ou `~/.bashrc`:
```bash
echo 'export PS5_MAC_ADDRESS="AA:BB:CC:DD:EE:FF"' >> ~/.zshrc
source ~/.zshrc
```

#### Windows (temporário):
```cmd
set PS5_MAC_ADDRESS=AA:BB:CC:DD:EE:FF
```

#### Windows (permanente):
```powershell
[System.Environment]::SetEnvironmentVariable('PS5_MAC_ADDRESS', 'AA:BB:CC:DD:EE:FF', 'User')
```

### Passo 3: Compilar o Projeto
```bash
platformio run --environment esp32doit-devkit-v1
```

## Método 2: Usando arquivo .env (Alternativo)

### Passo 1: Criar arquivo .env
```bash
cp .env.example .env
```

### Passo 2: Editar o arquivo .env
```bash
# Abrir .env e alterar:
PS5_MAC_ADDRESS=AA:BB:CC:DD:EE:FF
```

### Passo 3: Carregar variáveis antes de compilar
```bash
# macOS / Linux
source .env
platformio run

# Windows (PowerShell)
Get-Content .env | ForEach-Object {
    $var = $_.Split('=')
    [Environment]::SetEnvironmentVariable($var[0], $var[1])
}
platformio run
```

## Método 3: Editar Diretamente o config.h (Não Recomendado)

Se não quiser usar variáveis de ambiente, você pode editar diretamente:

`src/config.h`:
```cpp
#define PS5_MAC_ADDRESS "AA:BB:CC:DD:EE:FF"
```

⚠️ **Desvantagem:** O endereço MAC ficará exposto se você compartilhar o código.

## Verificar se Funcionou

Após compilar, verifique no Serial Monitor:
```
========================================
Sistema de Controle RC Iniciando...
========================================

Aguardando controle PS5: AA:BB:CC:DD:EE:FF
```

O endereço MAC exibido deve ser o seu!

## Troubleshooting

### Erro: "sysenv.PS5_MAC_ADDRESS not defined"
- Certifique-se que exportou a variável de ambiente
- Reinicie o terminal ou VS Code
- Verifique com: `echo $PS5_MAC_ADDRESS` (macOS/Linux) ou `echo %PS5_MAC_ADDRESS%` (Windows)

### DualSense não conecta
- Verifique se o MAC está correto (formato: `AA:BB:CC:DD:EE:FF`)
- Coloque o DualSense em modo pareamento (PS + Share por 3 segundos)
- Verifique se não está conectado a outro dispositivo
