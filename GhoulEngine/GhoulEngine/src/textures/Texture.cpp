#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	type = texType;
	int imgW, imgH, imgNumColCh;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageBytes = stbi_load(image, &imgW, &imgH, &imgNumColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	//Do these need to be exposed somehow to change their parameters? Maybe a default constructor option? 
	//	i.e. (GLenum...pixelType, Glenum interpolMethod = GL_NEAREST, Glenum mapType = GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Needs to detect GL_RGBA filetype and run GL_RGB if needed, and if not, load default texture on fail load.
	glTexImage2D(texType, 0, GL_RGBA, imgW, imgH, 0, format, pixelType, imageBytes);
	glGenerateMipmap(texType);
	
	
	stbi_image_free(imageBytes);
	glBindTexture(texType, 0);
}

//Feels super weird tying the shader to the texture this way, not passing texture to shader instead?
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Use();
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

//Should the destructor just call this
void Texture::Delete() {
	glDeleteTextures(1, &ID);
}