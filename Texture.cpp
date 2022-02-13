#include"Texture.h"

MyTexture::MyTexture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
{
    //Przypisz teksture do tekstury obiektu
	type = texType;

    //Przetrzymuje szerokosc, wysokosc i liczbe kolorow zdjecia
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true); //Obraca zdjecie
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0); //Czyta zdjecie z pliku i przetrzymuje w bajtach

    glGenTextures(1, &ID); //Generuje obiekt tekstury
    glActiveTexture(GL_TEXTURE0 + slot); //Przypisz teksture do jednostki Tekstury
    unit = slot;
    glBindTexture(texType, ID);

    //PARAMETRY
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // GL_NEAREST - tak jak jest
    // GL_LINEAR - uzupe³nia brakuj¹ce piksele, co czasami daje efekt blura
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Gdybyœmy chcieli obramowanie textury
    // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor)
    glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    // Generowanie MipMapy
    glGenerateMipmap(texType);

    //Odczepienie textury
    stbi_image_free(bytes);

    //Odbinduj teksture ¿eby przypadkowo nie zmodyfikowaæ
    glBindTexture(texType, 0);


}

void MyTexture::Bind()
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(type, ID);
}

void MyTexture::Unbind()
{
    glBindTexture(type, 0);
}

void MyTexture::Delete()
{
    glDeleteTextures(1, &ID);
}