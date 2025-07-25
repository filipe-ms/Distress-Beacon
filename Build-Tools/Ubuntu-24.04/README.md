# Desenvolvimento do Projeto Distress Call no Ubuntu 24.04

## 📌 Pré-requisitos

- **Conexão com a Internet**: Necessária para baixar pacotes e imagens de contêiner.

---

## 1. Instalar o Visual Studio Code no seu Ambiente Atual

O VS Code será executado no SteamOS, mas configurado para interagir com o ambiente da Distrobox.

### Via Interface Gráfica

- Procure por **Visual Studio Code** e instale a versão **Flatpak**.

---

### ✅ Verifique o script:

- Certifique-se de que `~/setup_dev_ubuntu.sh` existe.
- Dê permissão de execução:

```bash
chmod +x ~/setup_dev_ubuntu.sh
```

## 2. Instalar Dependências da Raylib

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

## 3. Copiar Arquivos de Configuração do VS Code

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

## 4. Iniciar o Desenvolvimento

1. Inicie o VS Code com o comando:

```bash
code ~/raiz/projeto/  # Mesmo diretório onde está o arquivo .sln
```

2. Instale a extensão **C/C++** da Microsoft (Ctrl+Shift+X).

---

## 5. Compilar e Depurar seu Projeto

### 🔧 Compilar (Debug/Release)

- Pressione `Ctrl+Shift+B` e selecione uma das tarefas:
  - **Build Debug**
  - **Build Release**

### 🐞 Executar e Depurar

- Vá para a aba **Run and Debug**.
- Selecione uma configuração e clique em **Play**.

---

Para mais informações, acesse: https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux

---

Pronto! Agora você tem um ambiente de desenvolvimento isolado, persistente e pronto para compilar e depurar o **Distress Call** no Ubuntu 24.04. 🚀
