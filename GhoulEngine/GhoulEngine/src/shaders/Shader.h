#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

//Kind of feel like the setters here could just use a <T>. 
//(?) inherit instead? virtual shader, and public : Vertex, Fragment, Geo?
class Shader {
public:
	GLuint ID;

	//better to treat this as a 3 index array? with 0 always vertex?
	//then same with the files getting the codes?
	Shader(const char* vertexPath, const char* fragmentPath) {
		//Feel that these being preset for smaller projects is better,
		//This class may feel better in a Game Engine
		//(?) Way to run "Pre-Compiled Shaders" like CoD?
		std::string vertexCode, fragmentCode;
		//std::string geometryCode;
		std::ifstream vShaderFile, fShaderFile;
		//std::ifstream gShaderFile;
		//This ensures ifstreams can throw errors (ifstream level instead?)
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		//gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			//gShaderFile.open(geometryPath);
			std::stringstream vShaderStream, fShaderStream;
			//std::stringstream gShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//gShaderStream << gShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			//gShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			//geometryCode = gShaderStream.str();

			//shaderCodes[0] = shaderStreams[0];
			//ShaderCodes[1] = shaderStreams[1];
			//etc
		}
		catch (std::ifstream::failure e) {
			//setup some enums for warnings instead
			std::cout << "ERROR:: Shader file not read" << std::endl;
		}
		//this feels very C and not C++ style.
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		//const char* gShaderCode = geometryCode.c_str();


		//Compiling the Shaders now...(different function?)
		GLuint vertex, fragment;
		int success = 0;
		char infoLog[512] = {};

		//Could I multithread the compilation using threads or CUDA?
			//Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, "VERTEX");
		

			//Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, "FRAGMENT");
			
			//Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		CheckCompileErrors(ID, "PROGRAM");
		

		//Delete the shaders now as they are linked, no longer needed
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Use() {
		glUseProgram(ID);
	}

	//Maybe there are typeof() ways to get around this redundancy here.
	void SetBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void SetInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void Delete() {
		glDeleteProgram(ID);
	}

private:
	void CheckCompileErrors(unsigned int shader, std::string type) {
		GLint hasCompiled;
		char infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (!hasCompiled) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR:: " << type << " Shader failed to compile. " << infoLog << std::endl;
			}
			
		}
		else {
			glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (!hasCompiled) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR:: Program " << type << " link error. " << infoLog << std::endl;
			}
		}
	}

};

#endif