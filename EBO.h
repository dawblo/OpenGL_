#ifndef EBO_CLASS_H
#define EBO_CLASS_H
#include<glad/glad.h>

class EBO
{
public:
	GLuint ID; //ID EBO
	EBO(GLuint* indices, GLsizeiptr size); //Konstruktor EBO

	void Bind(); //Podpina EBO
	void Unbind(); //Odpina EBO
	void Delete(); //Usuwa EBO

};

#endif