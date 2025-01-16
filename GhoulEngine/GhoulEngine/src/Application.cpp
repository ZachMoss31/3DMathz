#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#incluee memory
#include "shaders/Shader.h"
#include "textures/Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#include <iostream>

//Frame buffers and Window data...
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLfloat vertices[] = {
	//Position Vectors		//Color Vectors (r, g, b)	//Texture Coords
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
	-0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
	0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
	0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};


int main() {
	//-------------------------------------------------------- GLFW -------
	//glfwSetErrorCallback([](int errorr, const char* description) { fprintf(stderr, "Error: %s\n", description); });
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//Using the CORE profile. The other is 'Compatibility' for older GL optionality
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 4th option is whether to render full screen
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ghoul Engine", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetWindowSizeCallback(); //resizable window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	///-------------------------------------------------------- GLAD -------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	//-------------------------------------------------------- OpenGL Version -------
	std::cout << "Using OpenGL Version " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Using Vendor " << glGetString(GL_VENDOR) << std::endl;
	
	//-------------------------------------------------------- SHADERS -------
	Shader ourShader("src/shaders/3.3.shader.vs", "src/shaders/3.3.shader.fs");

	//Should I create a 'Mesh' abstract, and make 'TriangleMesh' inherit that and draw the below?
	//https://www.youtube.com/watch?v=8EEoxYgVaus&list=PLn3eTxaOtL2PHxN8EHf-ktAcN-sGETKfw&index=5
		//Even further, should I create an IRenderableObject class that has a Mesh, vector<Shader>, Transform, State?
		//I.e., RenderObject myTriangle(Mesh mesh, Material material [which is a std::vector<string> shaders, std::vector<string> textures], Transform trans, State enabled); ?
		//Renderer singlePassRenderer; 
		//singlePassRenderer.Render(myScene);, where scene is a tree of objects
		//Renderer postProcessRenderer;
		//postProcessRenderer.Render(myPostProcessList);

	VAO VAO1;
	VAO1.Bind();
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Texture

	Texture defTex("src/textures/2D_DefaultText.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	defTex.texUnit(ourShader, "tex0", 0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		//Background Color
		glClearColor(0.3f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//First Triangle
		ourShader.Use();
		defTex.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	
		//Frame rendered in back buffer; Swap back buffer and front buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	
	defTex.Delete();
	ourShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}