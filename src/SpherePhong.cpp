/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 13/08/2024
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace glm;

#include <cmath>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();
GLuint loadTexture(string filePath, int &width, int &height);

void drawGeometry(GLuint shaderID, GLuint VAO, vec3 position, vec3 dimensions, float angle, int nVertices, vec3 color= vec3(1.0,0.0,0.0), vec3 axis = (vec3(0.0, 0.0, 1.0)));
GLuint generateSphere(float radius, int latSegments, int lonSegments, int &nVertices);
 
// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 800;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = R"(
#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texc;

uniform mat4 projection;
uniform mat4 model;

out vec2 texCoord;
out vec3 vNormal;
out vec4 fragPos; 
out vec4 vColor;
void main()
{
   	gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);
	fragPos = model * vec4(position.x, position.y, position.z, 1.0);
	texCoord = texc;
	vNormal = normal;
	vColor = vec4(color,1.0);
})";

// Código fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = R"(
#version 400
in vec2 texCoord;
uniform sampler2D texBuff;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float ka;
uniform float kd;
uniform float ks;
uniform float q;
out vec4 color;
in vec4 fragPos;
in vec3 vNormal;
in vec4 vColor;
void main()
{

	vec3 lightColor = vec3(1.0,1.0,1.0);
	//vec4 objectColor = texture(texBuff,texCoord);
	vec4 objectColor = vColor;

	//Coeficiente de luz ambiente
	vec3 ambient = ka * lightColor;

	//Coeficiente de reflexão difusa
	vec3 N = normalize(vNormal);
	vec3 L = normalize(lightPos - vec3(fragPos));
	float diff = max(dot(N, L),0.0);
	vec3 diffuse = kd * diff * lightColor;

	//Coeficiente de reflexão especular
	vec3 R = normalize(reflect(-L,N));
	vec3 V = normalize(camPos - vec3(fragPos));
	float spec = max(dot(R,V),0.0);
	spec = pow(spec,q);
	vec3 specular = ks * spec * lightColor; 

	vec3 result = (ambient + diffuse) * vec3(objectColor) + specular;
	color = vec4(result,1.0);

})";

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Muita atenção aqui: alguns ambientes não aceitam essas configurações
	// Você deve adaptar para a versão do OpenGL suportada por sua placa
	// Sugestão: comente essas linhas de código para desobrir a versão e
	// depois atualize (por exemplo: 4.5 com 4 e 5)
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

	// Essencial para computadores da Apple
	// #ifdef __APPLE__
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// #endif

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Ola esfera iluminada!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de um triângulo
	int nVertices;
	GLuint VAO = generateSphere(0.5, 16, 16, nVertices);

	// Carregando uma textura e armazenando seu id
	int imgWidth, imgHeight;
	GLuint texID = loadTexture("../assets/tex/pixelWall.png",imgWidth,imgHeight);

	float ka = 0.1, kd =0.5, ks = 0.5, q = 10.0;
	vec3 lightPos = vec3(0.6, 1.2, -0.5);
	vec3 camPos = vec3(0.0,0.0,-3.0);


	glUseProgram(shaderID);

	// Enviar a informação de qual variável armazenará o buffer da textura
	glUniform1i(glGetUniformLocation(shaderID, "texBuff"), 0);

	glUniform1f(glGetUniformLocation(shaderID, "ka"), ka);
	glUniform1f(glGetUniformLocation(shaderID, "kd"), kd);
	glUniform1f(glGetUniformLocation(shaderID, "ks"), ks);
	glUniform1f(glGetUniformLocation(shaderID, "q"), q);
	glUniform3f(glGetUniformLocation(shaderID, "lightPos"), lightPos.x,lightPos.y,lightPos.z);
	glUniform3f(glGetUniformLocation(shaderID, "camPos"), camPos.x,camPos.y,camPos.z);

	//Ativando o primeiro buffer de textura da OpenGL
	glActiveTexture(GL_TEXTURE0);
	

	// Matriz de projeção paralela ortográfica
	// mat4 projection = ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);
	mat4 projection = ortho(-1.0, 1.0, -1.0, 1.0, -3.0, 3.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

	// Matriz de modelo: transformações na geometria (objeto)
	mat4 model = mat4(1); // matriz identidade
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO); // Conectando ao buffer de geometria
		glBindTexture(GL_TEXTURE_2D, texID); //conectando com o buffer de textura que será usado no draw

		// Primeiro Triângulo
		drawGeometry(shaderID, VAO, vec3(0, 0, 0), vec3(1, 1, 1), 0.0, nVertices);

	
		glBindVertexArray(0); // Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
//  shader simples e único neste exemplo de código
//  O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
//  fragmentShader source no iniçio deste arquivo
//  A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		// x    y    z   s    t    nx  ny    nz
		// T0
		-0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0,    // v0
		 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 0.0, -1.0,   // v1
		 0.0,  0.5, 0.0, 0.5, 1.0, 0.0, 0.0, -1.0 	  // v2
	};

	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);
	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	//  Tipo do dado
	//  Se está normalizado (entre zero e um)
	//  Tamanho em bytes
	//  Deslocamento a partir do byte zero

	//Atributo posição - coord x, y, z - 3 valores
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Atributo coordenada de textura - coord s, t - 2 valores
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo componentes vetor normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(5* sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

GLuint loadTexture(string filePath, int &width, int &height)
{
	GLuint texID; // id da textura a ser carregada

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Ajuste dos parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Carregamento da imagem usando a função stbi_load da biblioteca stb_image
	int nrChannels;

	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // assume que é 4 canais png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture " << filePath << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void drawGeometry(GLuint shaderID, GLuint VAO, vec3 position, vec3 dimensions, float angle, int nVertices, vec3 color, vec3 axis)
{
	// Matriz de modelo: transformações na geometria (objeto)
	mat4 model = mat4(1); // matriz identidade
	// Translação
	model = translate(model, position);
	// Rotação
	model = rotate(model, radians(angle), axis);
	// Escala
	model = scale(model, dimensions);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

	//glUniform4f(glGetUniformLocation(shaderID, "inputColor"), color.r, color.g, color.b, 1.0f); // enviando cor para variável uniform inputColor
																								//  Chamada de desenho - drawcall
																								//  Poligono Preenchido - GL_TRIANGLES
	glDrawArrays(GL_TRIANGLES, 0, nVertices);
}

GLuint generateSphere(float radius, int latSegments, int lonSegments, int &nVertices) {
    vector<GLfloat> vBuffer; // Posição + Cor + Normal + UV

    vec3 color = vec3(1.0f, 0.0f, 0.0f); // Laranja

    auto calcPosUVNormal = [&](int lat, int lon, vec3& pos, vec2& uv, vec3& normal) {
        float theta = lat * pi<float>() / latSegments;
        float phi = lon * 2.0f * pi<float>() / lonSegments;

        pos = vec3(
            radius * cos(phi) * sin(theta),
            radius * cos(theta),
            radius * sin(phi) * sin(theta)
        );

        uv = vec2(
            phi / (2.0f * pi<float>()),  // u
            theta / pi<float>()          // v
        );

        // Normal é a posição normalizada (posição/radius)
        normal = normalize(pos);
    };

    for (int i = 0; i < latSegments; ++i) {
        for (int j = 0; j < lonSegments; ++j) {
            vec3 v0, v1, v2, v3;
            vec2 uv0, uv1, uv2, uv3;
            vec3 n0, n1, n2, n3;

            calcPosUVNormal(i, j, v0, uv0, n0);
            calcPosUVNormal(i + 1, j, v1, uv1, n1);
            calcPosUVNormal(i, j + 1, v2, uv2, n2);
            calcPosUVNormal(i + 1, j + 1, v3, uv3, n3);

            // Primeiro triângulo
            vBuffer.insert(vBuffer.end(), { v0.x, v0.y, v0.z, color.r, color.g, color.b, n0.x, n0.y, n0.z, uv0.x, uv0.y });
            vBuffer.insert(vBuffer.end(), { v1.x, v1.y, v1.z, color.r, color.g, color.b, n1.x, n1.y, n1.z, uv1.x, uv1.y });
            vBuffer.insert(vBuffer.end(), { v2.x, v2.y, v2.z, color.r, color.g, color.b, n2.x, n2.y, n2.z, uv2.x, uv2.y });

            // Segundo triângulo
            vBuffer.insert(vBuffer.end(), { v1.x, v1.y, v1.z, color.r, color.g, color.b, n1.x, n1.y, n1.z, uv1.x, uv1.y });
            vBuffer.insert(vBuffer.end(), { v3.x, v3.y, v3.z, color.r, color.g, color.b, n3.x, n3.y, n3.z, uv3.x, uv3.y });
            vBuffer.insert(vBuffer.end(), { v2.x, v2.y, v2.z, color.r, color.g, color.b, n2.x, n2.y, n2.z, uv2.x, uv2.y });
        }
    }

    // Criar VAO e VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), vBuffer.data(), GL_STATIC_DRAW);

    // Layout da posição (location 0)
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(0));
glEnableVertexAttribArray(0);

// Layout da cor (location 1)
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);

// Layout da normal (location 2)
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
glEnableVertexAttribArray(2);

// Layout da UV (location 3)
glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    nVertices = vBuffer.size() / 11; // Cada vértice agora tem 11 floats!

    return VAO;
}