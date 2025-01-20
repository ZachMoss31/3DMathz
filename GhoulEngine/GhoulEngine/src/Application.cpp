#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

//#include memory
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

// Vertices coordinates
GLfloat vertices[] = {
	//     COORDINATES       /      COLORS       /   TEXCOORDS   //
	// Front face
	-0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // Bottom-left
	 0.5f, -0.5f,  0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom-right
	 0.5f,  0.5f,  0.5f,     0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top-right
	-0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // Top-left

	// Back face
	-0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom-left
	 0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom-right
	 0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top-right
	-0.5f,  0.5f, -0.5f,     0.5f, 0.5f, 1.0f,   0.0f, 1.0f, // Top-left
};

// Indices for vertices order
GLuint indices[] = {
	// Front face
	0, 1, 2,
	2, 3, 0,
	// Back face
	4, 5, 6,
	6, 7, 4,
	// Left face
	4, 0, 3,
	3, 7, 4,
	// Right face
	1, 5, 6,
	6, 2, 1,
	// Top face
	3, 2, 6,
	6, 7, 3,
	// Bottom face
	4, 5, 1,
	1, 0, 4
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

	// View port
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

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


	// Time

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// 3D needs depth buffer
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		//Background Color
		glClearColor(0.3f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Activate Shader
		ourShader.Use();

		//Rotation Update
		double timeNow = glfwGetTime();
		if (timeNow - prevTime >= 1 / 60) {
			rotation += 0.5f;
			prevTime = timeNow;
		}

		//Quaternion Rotation
		glm::vec3 rotationAxis(0.4f, 0.4f, 0.4f);
		rotationAxis = glm::normalize(rotationAxis);
		glm::quat quaternion = glm::angleAxis(glm::radians(rotation), rotationAxis);
		glm::mat4 rotationMatrix = glm::toMat4(quaternion);

		//MVP Matrix Initialization
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		//Assignment of the matrices
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.4f, 0.4f, 0.4f));
		model = model * rotationMatrix;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

		//Inputs to uniforms
		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projLoc = glGetUniformLocation(ourShader.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));


		defTex.Bind();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
	
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