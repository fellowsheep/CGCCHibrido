/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 02/03/2022
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

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STL
#include <vector>

#include <random>
#include <algorithm>

//Classes utilitárias
#include "Shader.h"

#include "Hermite.h"
#include "Bezier.h"
#include "CatmullRom.h"

using namespace std;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
vector <glm::vec3> generateControlPointsSet(int nPoints);
vector <glm::vec3> generateControlPointsSet();
std::vector<glm::vec3> generateUnisinosPointsSet();
GLuint generateControlPointsBuffer(vector <glm::vec3> controlPoints);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 600, HEIGHT = 600;


bool rotateX = false, rotateY = false, rotateZ = false;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Curvas Parametricas!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	
	// Compilando e buildando o programa de shader
	Shader shader = Shader("../shaders/hello.vs", "../shaders/hello.fs");

	
	glUseProgram(shader.ID);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//Neste exemplo, a cor da curva é passada como uma variável uniform
	shader.setVec4("finalColor", 0, 0, 0, 1);

	//Conjunto de pontos de controle

	std::vector<glm::vec3> controlPoints = generateControlPointsSet();
	GLuint VAO = generateControlPointsBuffer(controlPoints);

	Hermite hermite;
	hermite.setControlPoints(controlPoints);
	hermite.setShader(&shader);
	hermite.generateCurve(5);

	CatmullRom catmull;
	catmull.setControlPoints(controlPoints);
	catmull.setShader(&shader);
	catmull.generateCurve(10);
	
	//std::vector<glm::vec3> uniPoints = generateUnisinosPointsSet();
	//GLuint VAOUni = generateControlPointsBuffer(uniPoints);

	Bezier bezier;
	bezier.setControlPoints(controlPoints);
	bezier.setShader(&shader);
	bezier.generateCurve(10);

	int nbCurvePoints = bezier.getNbCurvePoints();
	int i = 0;

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		//Desenha o conjunto de pontos de controle
		glBindVertexArray(VAO);
		shader.setVec4("finalColor", 0, 0, 1, 1);
		glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());
		glBindVertexArray(0);

		//hermite.drawCurve(glm::vec4(1, 0, 0, 1));
		bezier.drawCurve(glm::vec4(0, 1, 0, 1));
		catmull.drawCurve(glm::vec4(1, 0, 1, 1));

		glm::vec3 pointOnCurve = bezier.getPointOnCurve(i);
		vector <glm::vec3> aux;
		aux.push_back(pointOnCurve);
		GLuint VAOPoint = generateControlPointsBuffer(aux);

		glBindVertexArray(VAOPoint);

		shader.setVec4("finalColor", 0, 0, 0, 1);
		// Chamada de desenho - drawcall
		// CONTORNO e PONTOS - GL_LINE_LOOP e GL_POINTS
		glDrawArrays(GL_POINTS, 0, aux.size());
		//glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());
		glBindVertexArray(0);

		i = (i + 1) % nbCurvePoints;

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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

std::vector<glm::vec3> generateControlPointsSet(int nPoints) {
	std::vector<glm::vec3> controlPoints;

	// Gerar números aleatórios
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distribution(-0.9f, 0.9f);  // Intervalo aberto (-0.9, 0.9)

	for (int i = 0; i < nPoints; i++) {
		glm::vec3 point;
		do {
			// Gerar coordenadas x e y aleatórias
			point.x = distribution(gen);
			point.y = distribution(gen);
		} while (std::find(controlPoints.begin(), controlPoints.end(), point) != controlPoints.end());

		// Definir coordenada z como 0.0
		point.z = 0.0f;

		controlPoints.push_back(point);
	}

	return controlPoints;
}

vector<glm::vec3> generateControlPointsSet()
{
	vector <glm::vec3> controlPoints;

	controlPoints.push_back(glm::vec3(-0.6, -0.4, 0.0));
	controlPoints.push_back(glm::vec3(-0.4, -0.6, 0.0));
	controlPoints.push_back(glm::vec3(-0.2, -0.2, 0.0));
	controlPoints.push_back(glm::vec3(0.0, 0.0, 0.0));
	controlPoints.push_back(glm::vec3(0.2, 0.2, 0.0));
	controlPoints.push_back(glm::vec3(0.4, 0.6, 0.0));
	controlPoints.push_back(glm::vec3(0.6, 0.4, 0.0));

	return controlPoints;
}

GLuint generateControlPointsBuffer(vector <glm::vec3> controlPoints)
{
	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(GLfloat)* 3, controlPoints.data(), GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

std::vector<glm::vec3> generateUnisinosPointsSet()
{
	float vertices[] = {
		-0.262530, 0.376992, 0.000000,
-0.262530, 0.377406, 0.000000,
-0.262530, 0.334639, 0.000000,
-0.262530, 0.223162, 0.000000,
-0.262530, 0.091495, 0.000000,
-0.262371, -0.006710, 0.000000,
-0.261258, -0.071544, -0.000000,
-0.258238, -0.115777, -0.000000,
-0.252355, -0.149133, -0.000000,
-0.242529, -0.179247, -0.000000,
-0.227170, -0.208406, -0.000000,
-0.205134, -0.237216, -0.000000,
-0.177564, -0.264881, -0.000000,
-0.146433, -0.289891, -0.000000,
-0.114730, -0.309272, -0.000000,
-0.084934, -0.320990, -0.000000,
-0.056475, -0.328224, -0.000000,
-0.028237, -0.334170, -0.000000,
0.000000, -0.336873, -0.000000,
0.028237, -0.334170, -0.000000,
0.056475, -0.328224, -0.000000,
0.084934, -0.320990, -0.000000,
0.114730, -0.309272, -0.000000,
0.146433, -0.289891, -0.000000,
0.177564, -0.264881, -0.000000,
0.205134, -0.237216, -0.000000,
0.227170, -0.208406, -0.000000,
0.242529, -0.179247, -0.000000,
0.252355, -0.149133, -0.000000,
0.258238, -0.115777, -0.000000,
0.261258, -0.071544, -0.000000,
0.262371, -0.009704, 0.000000,
0.262530, 0.067542, 0.000000,
0.262769, 0.153238, 0.000000,
0.264438, 0.230348, 0.000000,
0.268678, 0.284286, 0.000000,
0.275462, 0.320338, 0.000000,
0.284631, 0.347804, 0.000000,
0.296661, 0.372170, 0.000000,
0.311832, 0.396628, 0.000000,
0.328990, 0.419020, 0.000000,
0.347274, 0.436734, 0.000000,
0.368420, 0.450713, 0.000000,
0.393395, 0.462743, 0.000000,
0.417496, 0.474456, 0.000000,
0.436138, 0.487056, 0.000000,
0.450885, 0.500213, 0.000000,
0.464572, 0.513277, 0.000000,
0.478974, 0.525864, 0.000000,
0.494860, 0.538133, 0.000000,
0.510031, 0.552151, 0.000000,
0.522127, 0.570143, 0.000000,
0.531124, 0.593065, 0.000000,
0.537629, 0.620809, 0.000000,
0.542465, 0.650303, 0.000000,
0.546798, 0.678259, 0.000000,
0.552959, 0.703513, 0.000000,
0.563121, 0.725745, 0.000000,
0.577656, 0.745911, 0.000000,
0.596563, 0.764858, 0.000000,
0.620160, 0.781738, 0.000000,
0.648302, 0.795385, 0.000000,
0.678670, 0.805057, 0.000000,
0.710336, 0.810741, 0.000000,
0.750111, 0.814914, 0.000000,
0.802994, 0.819945, 0.000000,
0.860771, 0.825435, 0.000000,
	};

	vector <glm::vec3> uniPoints;

	for (int i = 0; i < 67*3; i+=3)
	{
		glm::vec3 point;
		point.x = vertices[i];
		point.y = vertices[i + 1];
		point.z = 0.0;

		uniPoints.push_back(point);
	}

	return uniPoints;
}

