//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX;
int lastMousePosY;

typedef struct _Vertex {
	float m_Pos[3];
	float m_Color[3];
} Vertex;


double deltaTime;



// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

int var1 = 0; 

GLint vertexShaderID, fragmentShaderID, shaderProgramID;
GLuint VAO[2], VBO[2];

// Codigo de los shaders, por ahora se crean en una cadena de texto
// Shader de vertices
const GLchar * vertexShaderSource = "#version 330 core\n"
"layout (location=0) in vec3 in_position;\n"
"layout (location=1) in vec3 in_color;\n"
"out vec3 our_color;\n"
"void main(){\n"
"gl_Position = vec4(in_position, 1.0);\n"
"our_color = in_color;\n"
"}\0";
// Shader de fragmento
const GLchar * fragmentShaderSource = "#version 330 core\n"
"in vec3 our_color;\n"
"out vec4 color;\n"
"void main(){\n"
"color = vec4(our_color, 1.0);\n"
"}\0";

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	// Compilacion de los shaders

	// Se crea el id del Vertex Shader
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	// Se agrega el codigo fuente al ID
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	// Compilación de Vertex Shader
	glCompileShader(vertexShaderID);
	GLint success;
	GLchar infoLog[512];// Se obtiene el estatus de la compilacion del vertex shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		// En caso de error se obtiene el error y lanza mensaje con error
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << "Error al compilar el VERTEX_SHADER." << infoLog << std::endl;
	}

	
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cout << "Error al compilar el Fragment_SHADER." << infoLog << std::endl;
	}

	// Programa con los shaders
	shaderProgramID = glCreateProgram();
	// Se agregan el vertex y fragment shader al program
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	// Proceso de linkeo
	glLinkProgram(shaderProgramID);
	// Revision de error de linkeo del programa
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR al linkear el programa." << infoLog << std::endl;
	}
	/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
	{
		// 	estrella
		Vertex vertices[] = 
		{
			//pico 1

			{ {0.0f, 0.5f, 0.0f},  {0.070, 0.615, 0.647} },
			{ {0.2f, 0.5f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {0.1f, 0.65f, 0.0f}, {0.592, 0.074, 0.6} },

			{ {0.0f, 0.5f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {0.2f, 0.5f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {0.1f, 0.1f, 0.0f}, {0.592, 0.074, 0.6} },

			//pico 2

			{ {-0.25f, 0.0f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {-0.25f, 0.2f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {-0.4f, 0.1f, 0.0f}, {0.592, 0.074, 0.6} },
			

			{ {-0.25f, 0.2f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {-0.25f, 0.0f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {0.1f, 0.1f, 0.0f}, {0.592, 0.074, 0.6} },

			//pico 3

			{ {0.0f, -0.3f, 0.0f},  {0.070, 0.615, 0.647} },
			{ {0.2f, -0.3f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {0.1f, -0.45f, 0.0f}, {0.592, 0.074, 0.6} },

			{ {0.0f, -0.3f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {0.2f, -0.3f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {0.1f, 0.1f, 0.0f}, {0.592, 0.074, 0.6} },
		

			//pico 4

			{ {0.45f, 0.0f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {0.45f, 0.2f, 0.0f}, {0.070, 0.615, 0.647} },
		    { {0.6f, 0.1f, 0.0f}, {0.592, 0.074, 0.6} },
			

			{ {0.45f, 0.2f, 0.0f}, {0.070, 0.615, 0.647} },
			{ {0.45f, 0.0f, 0.0f},  {0.070, 0.615, 0.647} },
			{ {0.1f, 0.1f, 0.0f}, {0.592, 0.074, 0.6} },

		};


		size_t bufferSize = sizeof(vertices);
		size_t vertexSize = sizeof(vertices[0]);
		size_t rgbOffset = sizeof(vertices[0].m_Pos);

		std::cout << "Vertices:" << std::endl;
		std::cout << "bufferSize:" << bufferSize << std::endl;
		std::cout << "vertexSize:" << vertexSize << std::endl;
		std::cout << "rgbOffset:" << rgbOffset << std::endl;

		// Se crea el ID del VAO
		/*
		El VAO es un objeto que nos permite almacenar la estructura de nuestros datos,
		Esto es de gran utilidad debido a que solo se configura la estructura una vez
		y se puede utilizar en el loop de renderizado
		*/
		glGenVertexArrays(1, &VAO[0]);	// Cambiamos el estado para indicar que usaremos el id del VAO
		glBindVertexArray(VAO[0]);
		// Se crea el VBO (buffer de datos) asociado al VAO
		glGenBuffers(1, &VBO[0]);

		// Cambiamos el estado para indicar que usaremos el id del VBO como Arreglo de vertices (GL_ARRAY_BUFFER)
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		// Copiamos los datos de los vertices a memoria del procesador grafico
		//           TIPO DE BUFFER     TAMANIO          DATOS    MODO (No cambian los datos)
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)rgbOffset);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	// Se definen los vertices de la geometria a dibujar
	//GLfloat vertices[] = { -0.5, -0.5, 0.0, 0.5, -0.5, 0.0, 0.0, 0.5, 0.0 };

	/*|||||||||||||||||||||||||||||||||||||||||||||||||||*/
	{	// casa
		Vertex vertices[] = 
		{
			//Frente
			{{0.5f, -0.5f, 0.0f}, {0.921, 0.713, 0.925}},
			{{0.5f, 0.5f, 0.0f}, {0.921, 0.713, 0.925}},
			{{-0.5f, 0.5f, 0.0f}, {0.921, 0.713, 0.925}},

			{{0.5f, -0.5f, 0.0f}, {0.921, 0.713, 0.925}},
			{{-0.5f, -0.5f, 0.0f}, {0.921, 0.713, 0.925}},
			{{-0.5f, 0.5f, 0.0f}, {0.921, 0.713, 0.925}},

			//techo	
		    {{-0.55f, 0.5f, 0.0f}, {0.886, 0.513, 0.349}},
			{{0.55f, 0.5f, 0.0f}, {0.886, 0.513, 0.349}},
			{{0.0f, 0.9f, 0.0f}, {0.886, 0.513, 0.349}},

			//ventana de techo
			{{-0.05f, 0.6f, 0.0f}, {0.517, 0.984, 0.847}},
			{{0.1f, 0.6f, 0.0f}, {0.517, 0.984, 0.847}},
			{{0.1f, 0.7f, 0.0f}, {0.517, 0.984, 0.847}},

			{{0.1f, 0.7f, 0.0f}, {0.517, 0.984, 0.847}},
			{{-0.05f, 0.7f, 0.0f}, {0.517, 0.984, 0.847}},
			{{-0.05f, 0.6f, 0.0f}, {0.517, 0.984, 0.847}},	

			//banqueta
			{{-0.7f, -0.5f, 0.0f}, {0.690, 0.690, 0.690}},
			{{-0.7f, -0.55f, 0.0f}, {0.690, 0.690, 0.690}},
			{{0.7f, -0.5f, 0.0f}, {0.690, 0.690, 0.690}},

			{{0.7f, -0.55f, 0.0f}, {0.690, 0.690, 0.690}},
			{{0.7f, -0.5f, 0.0f}, {0.690, 0.690, 0.690}},
			{{-0.7f, -0.55f, 0.0f}, {0.690, 0.690, 0.690}},

			//puerta
			{{-0.45f, -0.5f, 0.0f}, {0.678, 0.498, 0.403}},
			{{-0.2f, -0.5f, 0.0f}, {0.678, 0.498, 0.403}},
			{{-0.2f, 0.1f, 0.0f}, {0.678, 0.498, 0.403} },

			{{-0.2f, 0.1f, 0.0f}, {0.678, 0.498, 0.403}},
			{{-0.45f, 0.1f, 0.0f}, {0.678, 0.498, 0.403}},
			{{-0.45f, -0.5f, 0.0f}, {0.678, 0.498, 0.403}}, 

			//manija
			{{-0.26f, -0.22f, 0.0f}, {0.925, 0.854, 0.235}},
			{{-0.22f, -0.22f, 0.0f}, {0.925, 0.854, 0.235}},
			{{-0.22f, -0.2f, 0.0f}, {0.925, 0.854, 0.235} },

			{{-0.22f, -0.2f, 0.0f}, {0.925, 0.854, 0.235}},
			{{-0.26f, -0.2f, 0.0f}, {0.925, 0.854, 0.235}},
			{{-0.26f, -0.22f, 0.0f}, {0.925, 0.854, 0.235}},

			//ventana 

			{{0.0f, 0.15f, 0.0f}, {0.486, 0.803, 0.992}},
			{{0.15f, 0.15f, 0.0f}, {0.486, 0.803, 0.992}},
			{{0.0f, -0.2f, 0.0f}, {0.486, 0.803, 0.992}},

			{{0.0f, -0.2f, 0.0f}, {0.486, 0.803, 0.992}},
			{{0.15f, -0.2f, 0.0f}, {0.486, 0.803, 0.992}},
			{{0.15f, 0.15f, 0.0f}, {0.486, 0.803, 0.992}},

			{{0.15f, 0.15f, 0.0f}, {0.486, 0.674, 0.992}},
			{{0.3f, 0.15f, 0.0f}, {0.486, 0.674, 0.992}},
			{{0.15f, -0.2f, 0.0f}, {0.486, 0.674, 0.992}},

			{{0.15f, -0.2f, 0.0f}, {0.486, 0.674, 0.992}},
			{{0.3f, -0.2f, 0.0f}, {0.486, 0.674, 0.992}},
			{{0.3f, 0.15f, 0.0f}, {0.486, 0.674, 0.992}},

			{{-0.05f, -0.28f, 0.0f}, {0.662, 0.356, 0.760}},
			{{-0.05f, -0.2f, 0.0f}, {0.662, 0.356, 0.760}},
			{{0.35f, -0.2f, 0.0f}, {0.662, 0.356, 0.760}},

			{{0.35f, -0.28f, 0.0f}, {0.662, 0.356, 0.760}},
			{{-0.05f, -0.28f, 0.0f}, {0.662, 0.356, 0.760}},
			{{0.35f, -0.2f, 0.0f}, {0.662, 0.356, 0.760}}, 

			//Chimenea
			{{0.18f, 0.5f, 0.0f}, {0.886, 0.513, 0.349} },
			{{0.32f, 0.5f, 0.0f}, {0.886, 0.513, 0.349} },
			{{0.18f, 0.85f, 0.0f}, {0.886, 0.513, 0.349} },

			{{0.18f, 0.85f, 0.0f}, {0.886, 0.513, 0.349} },
			{{0.32f, 0.85f, 0.0f}, {0.886, 0.513, 0.349} },
			{{0.32f, 0.5f, 0.0f}, {0.886, 0.513, 0.349} }, 

			{ {0.15f, 0.85f, 0.0f}, {0.874, 0.407, 0.384} },
			{ {0.35f, 0.85f, 0.0f}, {0.874, 0.407, 0.384} },
			{ {0.15f, 0.9f, 0.0f}, {0.874, 0.407, 0.384} },

			{ {0.15f, 0.9f, 0.0f}, {0.874, 0.407, 0.384} },
			{ {0.35f, 0.9f, 0.0f}, {0.874, 0.407, 0.384} },
			{ {0.35f, 0.85f, 0.0f}, {0.874, 0.407, 0.384} }, 

		};



		size_t bufferSize = sizeof(vertices);
		size_t vertexSize = sizeof(vertices[0]);
		size_t rgbOffset = sizeof(vertices[0].m_Pos);

		std::cout << "Vertices:" << std::endl;
		std::cout << "bufferSize:" << bufferSize << std::endl;
		std::cout << "vertexSize:" << vertexSize << std::endl;
		std::cout << "rgbOffset:" << rgbOffset << std::endl;

		// Se crea el ID del VAO
		/*
		El VAO es un objeto que nos permite almacenar la estructura de nuestros datos,
		Esto es de gran utilidad debido a que solo se configura la estructura una vez
		y se puede utilizar en el loop de renderizado
		*/
		glGenVertexArrays(1, &VAO[1]);
		// Cambiamos el estado para indicar que usaremos el id del VAO
		glBindVertexArray(VAO[1]);
		// Se crea el VBO (buffer de datos) asociado al VAO
		glGenBuffers(1, &VBO[1]);

		// Cambiamos el estado para indicar que usaremos el id del VBO como Arreglo de vertices (GL_ARRAY_BUFFER)
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		// Copiamos los datos de los vertices a memoria del procesador grafico
		//           TIPO DE BUFFER     TAMANIO          DATOS    MODO (No cambian los datos)
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Se crea un indice para el atributo del vertice posicion, debe corresponder al location del atributo del shader
		// indice del atributo, Cantidad de datos, Tipo de dato, Normalizacion, Tamanio del bloque (Stride), offset
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (GLvoid*)rgbOffset);

		// Se habilita el atributo del vertice con indice 0 (posicion)
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}

	// Ya que se configuro, se regresa al estado original
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	glUseProgram(0);
	glDetachShader(shaderProgramID, vertexShaderID);
	glDetachShader(shaderProgramID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glDeleteProgram(shaderProgramID);

	glBindVertexArray(VAO[0]);
	glBindVertexArray(VAO[1]);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glDeleteBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glDeleteBuffers(1, &VBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO[0]);
	glDeleteVertexArrays(1, &VAO[1]);
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		case GLFW_KEY_E:
			var1 = 0;  
			break;
		case GLFW_KEY_C:
			var1 = 1;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);

		// Esta linea esta comentada debido a que de momento no se usan los shaders
		glUseProgram(shaderProgramID);

		// Se indica el buffer de datos que se usará deacuerdo a la opcion
		if (var1 == 0) {	
			glBindVertexArray(VAO[0]);
			// Primitiva de ensamble
			glDrawArrays(GL_TRIANGLES, 0, 24); 
			glBindVertexArray(0);
		}
		else {
			glBindVertexArray(VAO[1]);
			// Primitiva de ensamble
			glDrawArrays(GL_TRIANGLES, 0, 63); 
			glBindVertexArray(0);
		}


		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "practica #3", false);  
	applicationLoop();
	destroy();
	return 1;
}
