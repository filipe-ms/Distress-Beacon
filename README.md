# Space Invaders - Roguelike Edition

Este é um clássico desenvolvido por nossa equipe usando a biblioteca **raylib** (v1.3). O jogo é uma recriação moderna do clássico, com a adição de funcionalidades como waves progressivas de inimigos e disparos automáticos. O jogador controla uma nave espacial e deve derrotar os inimigos que aparecem em ondas, enquanto tenta evitar colisões.

## Instruções para build/run em outros sistemas operacionais

- [Ubuntu 24.04](Build-Tools/Ubuntu-24.04/README.md)
- [ArchLinux (SteamOS)](Build-Tools/Arch-SteamOS/README.md)

# Instruções para Configuração do Builder para (Windows MSVC)

Abra o arquivo de solução (.sln) no Visual Studio.

Aperte Iniciar para iniciar a depuração.

### 📌 Pré-requisitos

- **Conexão com a Internet**: Necessária para baixar pacotes e imagens de contêiner.
- **Máquina Windows**: Necessária para instalar a IDE exclusiva para windows.

### Instalação

- 1. Visual Studio 2022 Community no seu Ambiente Atual. Lembrar de incluir "Desenvolvimento em C/C++" durante o setup.
- 2. (Opcional) Após a instalação, associe os arquivos ".sln" com o Visual Studio
- 3. Iniciar o Desenvolvimento - Inicie o Visual Studio ao clicar no arquivo .sln:
- 4. Compilar e Depurar seu Projeto

### 🔧 Compilar (Debug/Release)

- No canto superior da tela, podemos ver uma dropdown que dá opções para gerar versões de Debug e Release
  - **Build Debug**
  - **Build Release**

---

Para mais informações sobre como configurar o MinGW64, consultar documentação da raylib: https://github.com/raysan5/raylib/wiki/Working-on-Windows

Pronto! Agora você tem um ambiente de desenvolvimento isolado, persistente e pronto para compilar e depurar o **Distress Call** no Windows. 🚀

---
