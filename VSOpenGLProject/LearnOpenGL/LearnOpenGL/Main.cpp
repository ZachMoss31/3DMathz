#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"

#include <iostream>

//Frame buffers and Window data...
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//First Triangle
GLfloat vertices[] = {
	//Position Vectors		//Color Vectors (r, g, b)
	-0.9f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	-0.45f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f
};

//Second Triangle
GLfloat flipTri[] = {
	0.0f, 0.5f, 0.0f,
	0.9f, 0.5f, 0.0f,
	0.45f, -0.5f, 0.0f
};

int main() {

	//Glfw initialized
	glfwInit();

	//Tell GLFW which versions we are planning to use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Using the CORE profile. The other is 'Compatibility' for older GL optionality
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window Build
	// 4th option is whether to render full screen
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

	//-------------------------------------------------------- SHADERS -------
	Shader ourShader("3.3.shader.vs", "3.3.shader.fs");
	Shader secondShader("3.3.shader.vs", "GreenGlow.fs");

	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	//Link attribut 1 to color vectors passed in by shader.
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();

	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(flipTri, sizeof(flipTri));
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 0, (void*)0);
	VAO2.Unbind();
	VBO2.Unbind();

#pragma region Green Glow
	////Glowing Shader
	/*float timeValue = glfwGetTime();
	float greenValue = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/

	//then add this in while loop
		//timeValue = glfwGetTime();
		//greenValue = sin(timeValue) / 2.0f + 0.5f;
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
#pragma endregion

	while(!glfwWindowShouldClose(window)) {
		processInput(window);

		//Background Color
		glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//First Triangle
		ourShader.Use();
		VAO1.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		secondShader.Use();
		VAO2.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//How to make sure we refresh each frame
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	VAO2.Delete();
	VBO2.Delete();
	ourShader.Delete();
	secondShader.Delete();

	glfwDestroyWindow(window);
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