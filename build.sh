#!/bin/bash

# ========================================
# Script para Compilar e Upload
# ========================================
# Este script carrega as variáveis de ambiente
# e compila o projeto

# Carregar variáveis de ambiente
if [ -f .env ]; then
    echo "📦 Carregando variáveis de ambiente..."
    export $(cat .env | grep -v '^#' | xargs)
    echo "✅ MAC Address configurado: $PS5_MAC_ADDRESS"
else
    echo "⚠️  Arquivo .env não encontrado. Usando valor padrão do config.h"
fi

# Compilar
echo "🔨 Compilando projeto..."
platformio run --environment esp32doit-devkit-v1

# Se quiser fazer upload automaticamente, descomente a linha abaixo:
# platformio run --target upload --environment esp32doit-devkit-v1

echo "✅ Concluído!"
