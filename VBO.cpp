#include"VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size) 
{
    glGenBuffers(1, &ID); //Generuj VBO dla 1 obiektu
        //Przypisz VBO jako GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, ID); //GL_ARRAY_BUFFER - u¿ywany do vertex bufferów
    //Wprowadz vertices do VBO
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); //rodzaj bufferu, rozmiar obiektu, vertices, rodzaj
    //rodzaj | GL_STREAM - bêdzie modyfikowany raz i u¿ywany kilka razy
    //       | GL_DYNAMIC - bêdzie modyfikowany wiele raz i u¿ywany wiele razy
    //       | GL_STATIC - bêdzie modyfikowany raz i u¿ywany wiele razy

}

void VBO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() 
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() 
{
    glDeleteBuffers(1, &ID);
}