#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION // dodane do pliku z kursu 
#define GLM_ENABLE_EXPERIMENTAL  // naprawia bledy

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>          //do transformacji, macierze, obracanie 
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>       //z kursu
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include"EBO.h"
#include"VBO.h"
#include"VAO.h"


#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void Inputs(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//PARAMETRY KAMERY
float FOVdeg = 45.0f;           // dystans renderowania 
float nearPlane = 0.1f;     //odleglosc wczytywania terenu, wglab etc
float farPlane = 100.0f;



glm::vec3 Position = glm::vec3(0.0f, 2.0f, 3.0f);                   //4 defaultowe ustawienia
glm::vec3 Orientation = glm::vec3(0.0f, -0.22f, -1.0f);
glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 cameraMatrix = glm::mat4(1.0f);  //do shadera vert

//Powoduje ze kamera nie skacze podczas klikania
bool firstClick = true;
float charSpeed = 0.1f;

//Poprawia szybkość i czułość podczas obracania
float speed = 0.001f;
float sensitivity = 100.0f;
//PARAMETRY KAMERY

GLfloat vertices[] = // wierzchołki
{
    //             KORDY  /    KOLORY                / TEXTURY      /   NORMALS
    -1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
     1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
     1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

GLuint indices[] = //Indices - oszczedzanie pamięci na sklonowane wierzchołki
{
    0, 1, 2, // Dolna scianka
    0, 2, 3, // Dolna scianka
};



GLfloat lightVertices[] =
{ //  KORDY
    -0.1f, -0.1f, 0.1f,
    -0.1f, -0.1f, -0.1f,
    0.1f, -0.1f, -0.1f,
    0.1f, -0.1f, 0.1f,

    -0.1f, 0.1f, 0.1f,
    -0.1f, 0.1f, -0.1f,
    0.1f, 0.1f, -0.1f,
    0.1f, 0.1f, 0.1f,
};

GLuint lightIndices[] =
{
    0,1,2,
    0,3,2,//
    0,4,7,
    0,7,3,//
    3,7,6,
    3,6,2,//
    2,6,5,
    2,1,5,//
    1,5,4,
    1,0,4,//
    4,7,6,
    4,5,6,//
};

GLfloat houseVertices[] =
{
    -1.0f,0.0f,1.0f,     0.9f, 0.9f, 0.8f,       0.0f, -1.0f, 0.0f,
    1.0f, 0.0f,1.0f,    0.9f, 0.9f, 0.8f,        0.0f, -1.0f, 0.0f,
    1.0f, 0.0f,-1.0f,    0.9f, 0.9f, 0.8f,       0.0f, -1.0f, 0.0f,
    -1.0f,0.0f, -1.0f,    0.9f, 0.9f, 0.8f,       0.0f, -1.0f, 0.0f,

   1.0f, 0.0f, -1.0f,   0.83f, 0.70f, 0.44f,       0.0f, 0.0f, -1.0f,
    -1.0f, 0.0f, -1.0f,    0.83f, 0.70f, 0.44f,     0.0f, 0.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,    0.83f, 0.70f, 0.44f,     0.0f, 0.0f, -1.0f,
    1.0f, 1.0f, -1.0f,   0.83f, 0.70f, 0.44f,      0.0f, 0.0f, -1.0f,

    -1.0f, 0.0f, 1.0f,    0.83f, 0.70f, 0.44f,       -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, -1.0f,    0.83f, 0.70f, 0.44f,      -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, -1.0f,    0.83f, 0.70f, 0.44f,      -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,    0.83f, 0.70f, 0.44f,       -1.0f, 0.0f, 0.0f,

    1.0f, 0.0f, 1.0f,    0.83f, 0.70f, 0.44f,       1.0f, 0.0f, 0.0f,
    1.0f, 0.0f,-1.0f,    0.83f, 0.70f, 0.44f,      1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, -1.0f,    0.83f, 0.70f, 0.44f,       1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,    0.83f, 0.70f, 0.44f,       1.0f, 0.0f, 0.0f,

    -1.0f, 0.0f, 1.0f,    0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f, //Scianka drzwi lewo
    -0.2f, 0.0f, 1.0f,    0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f,
    -0.2f, 1.0f, 1.0f,    0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,    0.83f, 0.70f, 0.44f,       0.0f, 0.0f, 1.0f,

    -0.2f, 1.0f, 1.0f,    0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f, //Scianka drzwi srodek
    -0.2f, 0.6f, 1.0f,    0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f,
    0.2f,  0.6f, 1.0f,    0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f,
    0.2f, 1.0f, 1.0f,     0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f,

    0.2f, 1.0f, 1.0f,     0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f, //Scianka drzwi prawo
    0.2f, 0.0f, 1.0f,    0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,    0.83f, 0.70f, 0.44f,       0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,    0.83f, 0.70f, 0.44f,        0.0f, 0.0f, 1.0f,

};

GLuint houseIndices[] =
{
    0,1,2, // Dolna scianka
    0,3,2,
    4,5,6, // Tylnia scianka
    4,7,6,
    8,9,10, // Lewa scianka
    8,11,10,
    12,13,14, // Prawa scianka
    12,15,14,
    16,17,18, //Lewo ramka drzwi
    16,19,18,
    20,21,22, //Srodek ramka drzwi
    20,23,22,
    24,25,26, //Prawo ramka drzwi
    24,27,26
};


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);          //wersja opengl
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);      //profil funkcji w opengl


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Projekt", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //ustawia obecne okno
    gladLoadGL(); //Ładowanie glada
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);        //generuje szerokosc ekranu od lewo do prawo

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Shader shaderProgram("default.vert", "default.frag");

    //Generuje VAO i przypisuje go
    VAO VAO1;
    VAO1.Bind();  //obiektowka do vao.cpp

    VBO VBO1(vertices, sizeof(vertices)); // Generuje VBO
    EBO EBO1(indices, sizeof(indices)); // Generuje EBO

    //Przypisuje VBO takie jak kordyynaty czy kolory do VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);                    //vertex
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));  //normalsy
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));  //textury
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));  //kolory

    //Odbindowanie
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();




    //Kwadrat obsługujący światło
    Shader lightShader("light.vert", "light.frag");

    //Generuje VAO i przypisuje
    VAO lightVAO;
    lightVAO.Bind();

    //Generuje VBO i przypisuje do vertexow
    VBO lightVBO(lightVertices, sizeof(lightVertices));
    //Generuje EBO i rzypisuje do indices
    EBO lightEBO(lightIndices, sizeof(lightIndices));

    //Przypisuje atrybuty VBO takie jak kordyynaty czy kolory
    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

    //Unbinduje zebysmy przypadkowo ich nie zmodyfikowali
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();

    Shader houseShader("house.vert", "house.frag");

    VAO houseVAO;
    houseVAO.Bind();

    VBO houseVBO(houseVertices, sizeof(houseVertices));
    EBO houseEBO(houseIndices, sizeof(houseIndices));

    houseVAO.LinkAttrib(houseVBO, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
    houseVAO.LinkAttrib(houseVBO, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    houseVAO.LinkAttrib(houseVBO, 2, 3, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));

    houseVAO.Unbind();
    houseVBO.Unbind();
    houseEBO.Unbind();

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);           //defaultowe pozycje modeli rgb+przezroczystosc

    glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, -5.22f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::scale(pyramidModel, glm::vec3(1.0f, 1.0f, 1.0f));
    pyramidModel = glm::translate(pyramidModel, pyramidPos);

    glm::vec3 housePos = glm::vec3(3.0f, 0.0f, 3.0f);
    glm::mat4 houseModel = glm::mat4(1.0f);
    houseModel = glm::translate(houseModel, housePos);

    glm::vec3 modelPos = glm::vec3(0.0f, 0.0f, 0.00f);
    glm::vec3 calculatedModelPos = modelPos;
    glm::mat4 model = glm::mat4(1.0f);

    //Aktualizuje i eksportuje martix kamery do VertexShadera
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    //Powoduje ze kamera patrzy sie we wlasciwym kierunku
    view = glm::lookAt(Position, modelPos, Up);
    //Dodaje perspektywe do sceny
    projection = glm::perspective(glm::radians(FOVdeg), (float)(SCR_WIDTH / SCR_HEIGHT), nearPlane, farPlane);
    //Eksportuje kamere do vertex shadera
    cameraMatrix = projection * view;

    lightShader.use();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel)); //wysylaja do shadera vertexow, pozwala na transformacje obiektow
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.use();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    houseShader.use();
    glUniformMatrix4fv(glGetUniformLocation(houseShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(houseModel));
    glUniform4f(glGetUniformLocation(houseShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(houseShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    //Texture

    //TEXTURA1-POCZĄTEK
    unsigned int texture;
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true); //Obraca zdjecie
    unsigned char* bytes = stbi_load("brick.png", &widthImg, &heightImg, &numColCh, 0); //Czyta zdjecie z pliku i przetrzymuje w bajtach

    glGenTextures(1, &texture); //Generuje obiekt tekstury
    glActiveTexture(GL_TEXTURE0); //Przypisz teksture do jednostki Tekstury
    glBindTexture(GL_TEXTURE_2D, texture);

    //PARAMETRY
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // GL_NEAREST - tak jak jest
    // GL_LINEAR - uzupełnia brakujące piksele, co czasami daje efekt blura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //Gdybyśmy chcieli obramowanie textury
    // float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    // Generowanie MipMapy
    glGenerateMipmap(GL_TEXTURE_2D);

    //Odczepienie textury
    stbi_image_free(bytes);

    //Odbinduj teksture żeby przypadkowo nie zmodyfikować
    glBindTexture(GL_TEXTURE_2D, 0);
    shaderProgram.use(); //Shader musi być aktywowany przed zmianą wartości uniformu
    glUniform1f(glGetUniformLocation(shaderProgram.ID, "tex0"), 0); //Ustawia wartość uniformu
    //TEXTURA1-KONIEC


    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vert", "1.model_loading.frag");

    // load models
    // -----------
    Model ourModel("../resources/objects/nanosuit/nanosuit.obj");


    glEnable(GL_DEPTH_TEST);   //glebia zeby nie nachodzily na siebie

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);  //kolor tla
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //oczyszcza bufor


        //Obs³uga klawiszów kamery
        Inputs(window);


        //Aktualizuje i eksportuje martix kamery do VertexShadera
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);


        //Powoduje ze kamera patrzy sie we wlasciwym kierunku
        view = glm::lookAt(Position, Position + Orientation, Up);
        //Dodaje perspektywe do sceny
        projection = glm::perspective(glm::radians(FOVdeg), (float)(SCR_WIDTH / SCR_HEIGHT), nearPlane, farPlane);
        //Eksportuje kamere do vertex shadera
        cameraMatrix = projection * view;

        glBindTexture(GL_TEXTURE_2D, texture);
        shaderProgram.use();
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), Position.x, Position.y, Position.z);  //wysylamy pozycje kamery do shadera
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));


        //planksSpec.Bind();
        VAO1.Bind(); //Przypisz VAO ¿eby OpenGL go u¿ywa³
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0); //Narysuj trójk¹t u¿ywaj¹c GL_TRIANGLE




        houseShader.use();

        glUniformMatrix4fv(glGetUniformLocation(houseShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(houseModel));     //wyslanie do shadera 
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix)); // i kamere


        houseVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(houseIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


        lightShader.use();
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);




        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            modelPos.z -= 0.01f;
            cout << "X:" + std::to_string(modelPos.x);
            cout << "Y:" + std::to_string(modelPos.y);
            cout << "Z:" + std::to_string(modelPos.z) << endl;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            modelPos.x -= 0.01f;
            cout << "X:" + std::to_string(modelPos.x);
            cout << "Y:" + std::to_string(modelPos.y);
            cout << "Z:" + std::to_string(modelPos.z) << endl;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            modelPos.x += 0.01f;
            cout << "X:" + std::to_string(modelPos.x);
            cout << "Y:" + std::to_string(modelPos.y);
            cout << "Z:" + std::to_string(modelPos.z) << endl;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            modelPos.z += 0.01f;
            cout << "X:" + std::to_string(modelPos.x);
            cout << "Y:" + std::to_string(modelPos.y);
            cout << "Z:" + std::to_string(modelPos.z) << endl;
        }
        

        //KOLEJNOŚĆ MA ZNACZENIE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        model = glm::mat4(1.f);
        model = glm::translate(model, modelPos);
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));   //rotacja
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);





        // don't forget to enable shader before setting uniforms
        lightShader.use();

        // view/projection transformations
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(cameraMatrix));


        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            lightPos.z -= 0.01f;
            cout << "X:" + std::to_string(lightPos.x);
            cout << "Y:" + std::to_string(lightPos.y);
            cout << "Z:" + std::to_string(lightPos.z) << endl;
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            lightPos.z += 0.01f;
            cout << "X:" + std::to_string(lightPos.x);
            cout << "Y:" + std::to_string(lightPos.y);
            cout << "Z:" + std::to_string(lightPos.z) << endl;
        }

        // tutaj jakąś pętle trzeba zrobic żeby wracała ta przeszkoda bo to nie działa //////////////////////////////////
        if (float2fixed(lightPos.z) > 2) {
            glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, -5.22f);
            glm::mat4 lightModel = glm::mat4(1.0f);
            lightModel = glm::translate(lightModel, lightPos);

        }


        if (float2fixed (modelPos.z) == float2fixed(lightPos.z) && float2fixed(modelPos.x) == float2fixed(lightPos.x)) {
            printf("WARNING!!!");
        }


        //KOLEJNOŚĆ MA ZNACZENIE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        lightModel = glm::mat4(1.f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::rotate(lightModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));   //rotacja
        lightModel = glm::scale(lightModel, glm::vec3(10.1f, 0.1f, 0.1f));
        ourShader.setMat4("model", lightModel);
        ourModel.Draw(ourShader);

        






        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void Inputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Position += speed * Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Position += speed * -Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        Position += speed * Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        Position += speed * -Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed += 0.00001f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) { //release przywraca domyslna wartosc
        speed = 0.001f;
    }

    //Obsługa myszy   
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        //Ukrywa kursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        //Powoduje że kamera nie skacze gdy klikamy
        if (firstClick)
        {
            glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));
            firstClick = false;
        }

        //Przetrzymuje kordyynaty myszki
        double mouseX;
        double mouseY;
        //Przechwytuje kordyynaty myszki
        glfwGetCursorPos(window, &mouseX, &mouseY);

        //Normalizuje kordynaty tak żeby zaczynały się na środku ekranu
        // wtedy transformuje je na stopnie(degrees)
        float rotx = sensitivity * (float)(mouseY - (SCR_HEIGHT / 2)) / SCR_HEIGHT;
        float roty = sensitivity * (float)(mouseX - (SCR_HEIGHT / 2)) / SCR_HEIGHT;

        //Kalkuluje nadchodzącą zmiane wertykalną w Orientation
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotx), glm::normalize(glm::cross(Orientation, Up)));

        //Decyzja czy zmiana jest dozwolona czy też nie
        if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) or (glm::angle(newOrientation, -Up) <= glm::radians(5.0f)))) {
            Orientation = newOrientation;
        }

        //Obraca w prawo i lewo
        Orientation = glm::rotate(Orientation, glm::radians(-roty), Up);

        //Ustawia kursor na środku ekranu
        glfwSetCursorPos(window, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        //Przywraca kursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //Powoduje że jest pewność że kamera nie będzie skakała przy kolejnym obracaniu
        firstClick = true;
    }
}
