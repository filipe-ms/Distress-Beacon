#!/bin/bash

# --- Configurações ---
DISTRO_NAME="Dev-Ubuntu-24.04" # Nome da sua Distrobox (Dev-Ubuntu-Versão)
DISTRO_IMAGE="ubuntu:24.04" # Imagem da distro a ser usada (Ubuntu 24.04 LTS)

# --- Parte 1: Criar e Configurar a Distrobox ---
echo "--- Criando a Distrobox '$DISTRO_NAME' com a imagem '$DISTRO_IMAGE' ---"
# O --yes garante que não haverá perguntas de confirmação durante a criação
distrobox create --name "$DISTRO_NAME" --image "$DISTRO_IMAGE" --yes

echo "--- Instalação concluída. Use o comando \"distrobox enter \"$DISTRO_NAME\" para entrar na distro criada na distrobox. Em seguida instale o visual studio code e execute o setup do ubuntu para raylib"
