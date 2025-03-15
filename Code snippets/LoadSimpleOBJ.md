# 📄 Leitor de Arquivo Wavefront .OBJ em C++

Esta documentação descreve a função `loadSimpleOBJ`, que lê arquivos no formato **Wavefront .OBJ**, recupera **vértices, coordenadas de textura e normais**, e os organiza em um **Vertex Buffer Object (VBO) e Vertex Array Object (VAO)** para uso no OpenGL.

## 📌 Funcionamento da Função `loadSimpleOBJ`

```cpp
int loadSimpleOBJ(string filePath, int &nVertices)
```

### **🟢 Entrada**
- `filePath`: **string** com o caminho do arquivo `.OBJ` a ser carregado.
- `nVertices`: **inteiro por referência** para armazenar o número de vértices processados.

### **🔵 Saída**
- **Retorna o identificador VAO** gerado pelo OpenGL.
- **Se houver erro na leitura do arquivo**, retorna `-1`.

### 📂 **Forma de Uso**: Carregar um arquivo 
Para carregar um arquivo `.OBJ` e armazená-lo no VAO:
```cpp
int nVertices;
GLuint objVAO = loadSimpleOBJ("../Modelos3D/Cube.obj", nVertices);
```

### 🎨 **Chamada de desenho (Polígono Preenchido - GL_TRIANGLES)**
No loop de renderização:
```cpp
glBindVertexArray(objVAO);
glDrawArrays(GL_TRIANGLES, 0, nVertices);
```


---

## 🔍 **Passo a Passo do Código**

### **1️⃣ Declaração de Estruturas de Dados**

A função utiliza `std::vector` para armazenar temporariamente:
- **`vertices`**: lista de posições `(x, y, z)` dos vértices.
- **`texCoords`**: lista de coordenadas de textura `(s, t)`.
- **`normals`**: lista de vetores normais `(nx, ny, nz)`.
- **`vBuffer`**: buffer auxiliar que armazena todos os valores dos atributos juntos para mandar para o VBO (Vertex Buffer Object). Correspondente ao nosso array `GLfloat vertices[]`dos exemplos iniciais.

---

### **2️⃣ Leitura do Arquivo .OBJ**

A função abre o arquivo `.OBJ` e processa linha por linha:

- **`v x y z`** → Armazena os vértices em `vertices`.
- **`vt s t`** → Armazena as coordenadas de textura em `texCoords`.
- **`vn nx ny nz`** → Armazena as normais em `normals`.
- **`f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3`** → Processa cada índice e recupera os valores de `vertices`, `texCoords` e `normals`, armazenando-os no `vBuffer`.

📌 **OBS:** O código ajusta os índices para iniciar em `0` (já que o formato .OBJ começa em `1`).

---

### **3️⃣ Envio dos Dados ao OpenGL (VBO e VAO)**

1️⃣ **Criação do VBO:**
```cpp
glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), vBuffer.data(), GL_STATIC_DRAW);
```
- Gera um **identificador de buffer (VBO)**.
- Associa o buffer e carrega os dados processados do `vBuffer`.

2️⃣ **Criação do VAO:**
```cpp
glGenVertexArrays(1, &VAO);
glBindVertexArray(VAO);
```
- Gera um **Vertex Array Object (VAO)** e o associa ao contexto OpenGL.

3️⃣ **Configuração dos Atributos de Vértice:**

- **Posição dos vértices (x, y, z)**
```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
```
- **Cor do vértice (r, g, b)**
```cpp
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
glEnableVertexAttribArray(1);
```
 ⚠️**ATENÇÃO!** Cada vértice contém **6 valores (x, y, z, r, g, b)** no buffer que criamos para o VBO e registramos no VAO. Por isso, `nVertices` recebe o valor do número de elementos do `vBuffer` **dividido pelo número de valores total dos atributos**. Este valor precisará ser atualizado quando os demais atributos (vetores normais e coordenadas de texturas forem incorporados)

```cpp
nVertices = vBuffer.size() / 6;  // x, y, z, r, g, b (valores atualmente armazenados por vértice)
```

4️⃣ **Desvinculação dos Buffers**
```cpp
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);
```
- Garante que **nenhum buffer ou VAO fique preso ao contexto**.

---

### **5️⃣ Retorno do Identificador do VAO**

```cpp
return VAO;
```
- É pelo identificador **VAO** gerado pela OpenGL que poderemos acessar qual geometria desejamos desenhar (conectar antes da chamada de desenho `glDrawArrays` através do comando `glBindVertexArray`).

Se houver erro na leitura do arquivo, exibe uma mensagem e retorna `-1`.
```cpp
std::cerr << "Erro ao tentar ler o arquivo " << filePath << std::endl;
return -1;
```

---

## ✅ **Resumo do Código**

- **Abre e lê o arquivo .OBJ**, processando as linhas com informações das coordenadas dos vértices, texturas e normais.
- **Processa a informação das faces** (triângulos), recuperando os índices (de vértice, coord de texturas e normais) - usa por enquanto apenas o índice dos vértices para montar o buffer
- **Monta um buffer com os atributos dos vértices** temporário (`vBuffer`) que será utilizado para passar os dados para o VBO, utilizando no momento apenas a informação das coordenadas dos vértices e acrescentando (temporariamente) uma cor por vértice (vermelho).
- **Cria e configura um VBO e um VAO**
- Calcula o número de vértices e atualiza a variável nVertices, *passada por referência* para a função (& no cabeçalho)
- **Retorna o identificador do VAO gerado** para uso na renderização.

---

## 🎯 **Próximos Passos**
📌 Incluir os atributos **coordenadas de textura** e **componentes do vetor normal** ao **VAO**.  
📌 Implementar **carga de materiais (.MTL) para atribuir cores e texturas** (Módulo 3).


## 📚 Referências

- [`std::vector`](https://cplusplus.com/reference/vector/vector/) - Estrutura de dados dinâmica utilizada para armazenar vértices, texturas e normais.  
- [`std::fstream`](https://cplusplus.com/reference/fstream/fstream/) - Manipulação de arquivos para leitura do `.OBJ`.  
- [`std::sstream`](https://cplusplus.com/reference/sstream/istringstream/) - Processamento de strings para extrair dados das linhas do arquivo.  
- [VAO, VBO e Shaders no OpenGL](https://learnopengl.com/Getting-started/Shaders) - Explicação detalhada sobre buffers e sua utilização na renderização.

