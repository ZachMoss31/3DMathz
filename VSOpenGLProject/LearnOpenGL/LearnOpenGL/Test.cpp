#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//Frame buffers and Window data...
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


//Shader data...
//Each shader begins by declaring it's version. This is dynamically linking vertex shader.
const char *vertexShaderSource= "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

//Frag Shader only requires ONE output, a vec4 that defines final color output.
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform vec4 ourColor;"
	"void main()\n"
	"{\n"
	"    FragColor = ourColor;\n"
	"}\0";
const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(0.0f, 0.1f, 1.0f, 1.0f);\n"
"}\0";

int main() {

	//Glfw initialized
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Window Build
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PurpleBox", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Glad function pointers call
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	


	//-------------------------------------------------------- SHADER -------
	//-----------------------------------------------------------------------

			//--------------------------------------------------Vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
			//Check compiliation
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR:: Failed vertex shader compilation\n" << infoLog << std::endl;
	}
			//--------------------------------------------------------Fragment Shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR:: Failed fragment shader compilation\n" << infoLog << std::endl;
	}
			//--------------------------------------------------------Second Fragment Shader
	unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader2);

			//-------------------------------------------------------Shader Linking
			//Both shaders compiled, now we must LINK them in...by creating a 'Shader program'
				//create the program and attach the compiled shaders to it.
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	unsigned int shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	//--------------------------------------------------------Our Triangle
	float vertices[] = {
		-0.9f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		-0.45f, 0.5f, 0.0f
	};
	float vertices2[] = {
		0.0f, 0.5f, 0.0f,
		0.9f, 0.5f, 0.0f,
		0.45f, -0.5f, 0.0f
	};


	//The EBO (Element Buffer Object) here is used to draw rectangles not just tris in OpenGL.
	unsigned int VBOs[2], VAOs[2];
	//unsigned int EBO;
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(VAOs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	//GL_STREAM_DRAW, GL_STATIC_DRAW, GL_DYNAMIC_DRAW are all options to draw here
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	
	
	float timeValue = glfwGetTime();
	float greenValue = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	while(!glfwWindowShouldClose(window)) {
		processInput(window);

		//Purple background color
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		glUseProgram(shaderProgram);
		timeValue = glfwGetTime();
		greenValue = sin(timeValue) / 2.0f + 0.5f;
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		//Draw the Blue Triangle as Wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}