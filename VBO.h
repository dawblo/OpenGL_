#ifndef VBO_CLASS_H
#define VBO_CLASS_H
#include<glad/glad.h>

class VBO
{
public:
	GLuint ID; //ID VBO
	VBO(GLfloat* vertices, GLsizeiptr size); //Konstruktor VBO

	void Bind(); //Podpina VBO
	void Unbind(); //Odpina VBO
	void Delete(); //Usuwa VBO

};

#endif
