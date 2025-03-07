# Configuração do Ambiente para Computação Gráfica com <img src="https://github.com/user-attachments/assets/db96047c-5e1a-4656-a3af-ee3432f9f35a" alt="OpenGL Logo" height="32" style="vertical-align: middle;" />

Este tutorial irá guiá-lo pela instalação e configuração do ambiente para rodar projetos OpenGL com **CMake** e **VS Code**, utilizando o **MinGW-UCRT64** como compilador (portanto, no Windows).

---

## 📌 1. Instalando as Ferramentas Necessárias

Antes de começar, certifique-se de ter os seguintes programas instalados:

### 🔹 1.1 Instalar o <img src="https://github.com/user-attachments/assets/5e9c8078-53b0-4497-bbf2-1f379293aa60" alt="CMake Logo" height="32" style="vertical-align: middle;" />

Baixe e instale o **CMake** a partir do site oficial:
🔗 [CMake Download](https://cmake.org/download/)

Durante a instalação, **habilite a opção "Add CMake to system PATH"** para facilitar o uso no terminal.

---

### 🔹 1.2 Instalar o Compilador Compilador MinGW-UCRT64 através do <img src="https://github.com/user-attachments/assets/a473c44b-1fe1-4399-911a-d921225f53a6" alt="MSYS2 Logo" width="32" style="vertical-align: middle;" />


Baixe o **MSYS2** através do link:
🔗 [MSYS2 Download](https://www.msys2.org/)

Provavelmente ao terminar de instalar, abrirá um terminal. Caso isso não aconteça, digite "msys" na barra de pesquisa do Windows e entre neste aplicativo:

![image](https://github.com/user-attachments/assets/8d5603b3-6338-4235-9775-6aeaa900da7d)


Execute o seguinte comando para instalar os pacotes necessários:

```sh
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```

### Configurando a variável de ambiente no Sistema Operacional (Windows)

Esse passo é necessário para que, de qualquer diretório, o sistema operacional encontre o executável do compilador. A forma mais simples de fazer isso (se você tiver permissão de administrador do sistema) é a seguinte:
 - Edite a variável de caminhos do sistema operacional (`PATH`), acrescentando o caminho onde ficaram os executáveis do compilador (provavelmente ficaram em `C:\msys64\ucrt64\bin` - ou onde você escolheu instalar)

<img src="https://github.com/user-attachments/assets/645846a5-38e2-4a26-a307-a961afb80116" alt="" style="width:30%; height:auto;" align="middle"/>
<img src="https://github.com/user-attachments/assets/89bc66d1-c269-45ac-9f5f-e528325a9e95" alt="" style="width:30%; height:auto;" align="middle"/>
<img src="https://github.com/user-attachments/assets/25c305fa-239c-4119-afd3-20d8dbf318e6" alt="" style="width:30%; height:auto;" align="middle"/>

 - Se estiver em um computador que não tenha acesso de adm, adicionar temporariamente ao path com este comando (via terminal CMD): 
```sh
   set PATH=%PATH%;C:\msys64\ucrt64\bin
```
---

### 🔹 1.3 Instalar o VS Code <img src="https://github.com/user-attachments/assets/0b0e314b-8910-4311-b862-8f4c2e012d33" alt="Visual Studio Code Logo" width="32" style="vertical-align: middle;" />

Baixe e instale o **VS Code** pelo link:
🔗 [VS Code Download](https://code.visualstudio.com/)

Após a instalação, abra o **VS Code** e instale as seguintes extensões:

- **CMake Tools** ➝ Para integração com o CMake.
- **C/C++** ➝ Para suporte à IntelliSense e depuração.
  
Para isso, você pode ir no menu View -> Extensions ou clicar no ícone <img src="https://github.com/user-attachments/assets/ce6873db-1dbb-4a74-887f-4ea90f90dfc4" alt="" width="32" style="vertical-align: middle;" /> da interface do Visual Studio Code.

---

## 📌 2. Clonando o Repositório de Exemplo

Agora vamos baixar o código de exemplo:

1️⃣ **Abra o terminal do VS Code (`Ctrl + J`)**
2️⃣ **Clone o repositório** no diretório de sua escolha:

```sh
git clone https://github.com/fellowsheep/PGCCHIB.git
```

3️⃣ **Entre na pasta do projeto**:

```sh
cd PGCCHIB
```

---

## 📌 3. Configurando o CMake no VS Code

1️⃣ **No VS Code, pressione** `Ctrl + Shift + P` e procure por **"CMake: Select a Kit"**.  
2️⃣ **Selecione o compilador correto**:
   - Escolha **"GCC for MSYS2 UCRT64"**.
3️⃣ Se o kit não aparecer, execute `CMake: Scan for Kits` e tente novamente.

Agora, configure o projeto:

4️⃣ **Pressione `Ctrl + Shift + P` e execute "CMake: Configure"**
   - Isso fará o **CMake detectar o compilador correto e preparar o projeto**.

Se houver erro de compilador, rode este comando no terminal:

```sh
cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe
```

---

## 📌 4. Compilando e Executando o Projeto

Agora que o CMake está configurado, compile o projeto:

1️⃣ **Pressione `Ctrl + Shift + P` e execute "CMake: Build"**
   - Ou rode manualmente no terminal:

   ```sh
   cmake --build .
   ```

2️⃣ **Execute o programa**:
   ```sh
   ./HelloTriangle.exe
   ```

Se tudo estiver correto, o projeto será compilado e executado com sucesso! 🚀

---

## 📌 5. Próximos Passos

Agora que seu ambiente está configurado, você pode:
✅ Criar novos exercícios dentro da pasta `src/`.
✅ Adicionar novas funcionalidades ao código.
✅ Explorar OpenGL e computação gráfica com mais liberdade! 😃


