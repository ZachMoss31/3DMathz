#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"

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
	//Position Vectors		//Color Vectors (r, g, b)
	-0.9f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	-0.45f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f
};

GLfloat flipTri[] = {
	0.0f, 0.5f, 0.0f,
	0.9f, 0.5f, 0.0f,
	0.45f, -0.5f, 0.0f
};

int main() {
	//-------------------------------------------------------- GLFW -------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.5);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.5);
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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	///-------------------------------------------------------- GLAD -------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	//-------------------------------------------------------- OpenGL Version -------
	std::cout << "Using OpenGL Version " << glGetString(GL_VERSION) << std::endl;
	
	//-------------------------------------------------------- SHADERS -------
	Shader ourShader("src/shaders/3.3.shader.vs", "src/shaders/Rainbow.fs");
	Shader secondShader("src/shaders/3.3.shader.vs", "src/shaders/GreenGlow.fs");

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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	//Link attribut 1 is to add second set of vectors as colors in 'vertices'.
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
		//Do we unbind VBO or VAO first?
	GLuint uniIDr = glGetUniformLocation(ourShader.ID, "redVal");
	GLuint uniIDg = glGetUniformLocation(ourShader.ID, "greenVal");
	GLuint uniIDb = glGetUniformLocation(ourShader.ID, "blueVal");
	GLuint uniIDa = glGetUniformLocation(ourShader.ID, "alpha");

	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(flipTri, sizeof(flipTri));
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 0, (void*)0);
	VAO2.Unbind();
	VBO2.Unbind();
	GLuint uniID = glGetUniformLocation(secondShader.ID, "greenVal");
	float timeValue = 1.0f;
	float redGlow = 1.0f, greenGlow = 1.0f, blueGlow = 1.0f;
	float greenGlow2 = 1.0f;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		//Background Color
		glClearColor(0.3f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		timeValue = glfwGetTime();
		redGlow = cos(timeValue) / 2.0f + 0.5f;
		if (redGlow < 0)
			redGlow *= -1;
		greenGlow = sin(timeValue) / 2.0f + 0.5f;
		greenGlow2 = greenGlow;
		if (greenGlow < 0)
			greenGlow *= -1;
		blueGlow = sin(timeValue) / 2.0f;
		if (blueGlow < 0)
			blueGlow *= -1;
		//First Triangle
			//(?) Would a better name be ourShader.Bind(), as 'bind' is like 'select' in OpenGL it seems?
		ourShader.Use();
		glUniform1f(uniIDr, redGlow);
		glUniform1f(uniIDg, greenGlow);
		glUniform1f(uniIDb, blueGlow);
		
		//(!) If I dont want this fading to black, I need to catch when these vals cross 0, with a value like 'green glow').
		VAO1.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Second Triangle
		secondShader.Use();
		
		glUniform1f(uniID, greenGlow2);
		VAO2.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//How to make sure we refresh each frame
			//Swaps the forward for the rear buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	VAO2.Delete();
	VBO2.Delete();
		//Maybe just make a list of shaders and append onto that list, so we can mass delete them? idk
	ourShader.Delete();
	secondShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}