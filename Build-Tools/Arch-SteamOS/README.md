# Desenvolvimento do Projeto Distress Call no SteamOS (Steam Deck)

Este tutorial detalha como configurar um ambiente de desenvolvimento robusto e persistente para o projeto **Distress Call** no seu Steam Deck, utilizando o **Distrobox** para isolar as ferramentas e bibliotecas necessárias do sistema operacional base (SteamOS/Arch Linux). Isso garante que suas configurações não sejam afetadas por atualizações do sistema e que você tenha um ambiente Ubuntu familiar para desenvolvimento.

---

## 📌 Pré-requisitos

- **Steam Deck em Modo Desktop**: Certifique-se de estar no modo desktop do Steam Deck.
- **Acesso ao Terminal**: Familiaridade básica com o uso do terminal Linux.
- **Conexão com a Internet**: Necessária para baixar pacotes e imagens de contêiner.

---

## 1. Instalar o Visual Studio Code no seu Ambiente Atual (SteamOS)

O VS Code será executado no SteamOS, mas configurado para interagir com o ambiente da Distrobox.

### Via Interface Gráfica

- Abra o **Discover Software Center**.
- Procure por **Visual Studio Code** e instale a versão **Flatpak**.

### Via Terminal

```bash
flatpak install flathub com.visualstudio.Code
```

---

## 2. Rodar o Script de Setup da Distrobox

Esse script cria uma Distrobox com Ubuntu 24.04 e instala ferramentas necessárias.

### ✅ Verifique o script:

- Certifique-se de que `~/setup_dev_ubuntu.sh` existe.
- Dê permissão de execução:

```bash
chmod +x ~/setup_dev_ubuntu.sh
```

### ▶️ Execute o script:

```bash
~/setup_dev_ubuntu.sh
```

Para "entrar" no ubuntu, execute:

```bash
# Assumindo que o nome Dev-Ubuntu-24.04 foi mantido
distrobox enter "Dev-Ubuntu-24.04"
```

---

## 3. Instalar Dependências da Raylib

Após entrar na Distrobox, instale as bibliotecas necessárias para compilar projetos com a Raylib:

```bash
#Forçar as dependências correntes a serem atualizadas
sudo apt update && sudo apt upgrade -y
sudo apt update && sudo apt upgrade -y

# Agora, instale os pacotes necessários:
sudo apt install -y gdb build-essential git \
libasound2-dev libx11-dev libxrandr-dev libxi-dev \
libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev \
libxinerama-dev libwayland-dev libxkbcommon-dev
```

Essas bibliotecas são essenciais para garantir o suporte gráfico e de áudio usado pela Raylib.

---

## 4. Copiar Arquivos de Configuração do VS Code

Copie os arquivos `launch.json` e `tasks.json` para a pasta `.vscode` na raiz do projeto.

```
root/
├── .vscode/
│   ├── launch.json
│   ├── tasks.json
│   └── settings.json
├── src/
├── ...
```

---

## 5. Iniciar o Desenvolvimento

1. Abra um **novo terminal** no modo Desktop.
2. Inicie o VS Code com o comando:

```bash
code ~/raiz/projeto/  # Mesmo diretório onde está o arquivo .sln
```

3. (Opcional) Verifique se o terminal integrado do VS Code está no ambiente da Distrobox:

```bash
which gcc
echo $PATH
```

4. Instale a extensão **C/C++** da Microsoft (Ctrl+Shift+X).

---

## 6. Compilar e Depurar seu Projeto

### 🔧 Compilar (Debug/Release)

- Pressione `Ctrl+Shift+B` e selecione uma das tarefas:
  - **Build Debug (Distrobox)**
  - **Build Release (Distrobox)**

### 🐞 Executar e Depurar

- Vá para a aba **Run and Debug**.
- Selecione uma configuração e clique em **Play**.

---

## 7. Remover a Distrobox (Opcional)

### 🧹 Passos para limpeza:

```bash
distrobox list
distrobox stop Dev-Ubuntu-24.04
distrobox rm Dev-Ubuntu-24.04
```

Para mais informações, acesse: https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux

---

Pronto! Agora você tem um ambiente de desenvolvimento isolado, persistente e pronto para compilar e depurar o **Distress Call** no Steam Deck com suporte à Raylib. 🚀
