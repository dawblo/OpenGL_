#ifndef MYTEXTURE_CLASS_H
#define MYTEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>


class MyTexture
{
public:
	GLuint ID;
	GLuint type;
	GLuint unit;

	MyTexture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	void Bind();
	void Unbind();
	void Delete();



};
#endif
