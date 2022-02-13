#include"EBO.h"

EBO::EBO(GLuint* indices, GLsizeiptr size)
{
    glGenBuffers(1, &ID); //Generuj VBO dla 1 obiektu
        //Przypisz VBO jako GL_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); //GL_ARRAY_BUFFER - u¿ywany do vertex bufferów
    //Wprowadz vertices do VBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW); //rodzaj bufferu, rozmiar obiektu, vertices, rodzaj
    //rodzaj | GL_STREAM - bêdzie modyfikowany raz i u¿ywany kilka razy
    //       | GL_DYNAMIC - bêdzie modyfikowany wiele raz i u¿ywany wiele razy
    //       | GL_STATIC - bêdzie modyfikowany raz i u¿ywany wiele razy

}

void EBO::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
    glDeleteBuffers(1, &ID);
}