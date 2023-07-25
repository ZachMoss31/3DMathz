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
	unsigned int ID;

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
			std::cout << "ERROR:: Shader file not read!" << std::endl;
		}
		//this feels very C and not C++ style.
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		//const char* gShaderCode = geometryCode.c_str();
	}
	void Use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
};

#endif