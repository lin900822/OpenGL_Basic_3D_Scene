#define GLEW_STATIC		// 使用靜態函式庫

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Material.h"
#include "Camera.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include "ObjLoader.h"

unsigned int VAO;
unsigned int VBO;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0,  0.0f),
    glm::vec3(0.0f,  20.0f,  0.0f),
    glm::vec3(20.0f,  20.0f,  0.0f),
    glm::vec3(-20.0f,  20.0f,  0.0f),
    glm::vec3(0.0f,  20.0f,  20.0f),
    glm::vec3(0.0f,  20.0f,  -20.0f),
    glm::vec3(0.0f,  0,  0.0f),
    glm::vec3(0.0f,  0,  0.0f),
    glm::vec3(31.0f,  0,  0.0f),
    glm::vec3(-31.0f,  0,  0.0f),
    glm::vec3(0.0f,  0,  31.0f),
    glm::vec3(0.0f,  0,  -31.0f)
};

glm::vec3 cubeScales[] = {
    glm::vec3(40.0f,  1.0f,  40.0f),
    glm::vec3(40.0f,  1.0f,  40.0f),
    glm::vec3(1.0f,  40.0f,  40.0f),
    glm::vec3(1.0f,  40.0f,  40.0f),
    glm::vec3(40.0f,  40.0f,  1.0f),
    glm::vec3(40.0f,  40.0f,  1.0f),
    glm::vec3(18.0f,  40.0f,  1.0f),
    glm::vec3(1.0f,  40.0f,  18.0f),
    glm::vec3(40.0f,  40.0f,  1.0f),
    glm::vec3(40.0f,  40.0f,  1.0f),
    glm::vec3(1.0f,  40.0f,  40.0f),
    glm::vec3(1.0f,  40.0f,  40.0f)
};

glm::vec3 monkeyPositions[] = {
    glm::vec3(10.0f, 2.0f, 10.0f),
    glm::vec3(-5.0f, 2.0f, 5.0f),
    glm::vec3(5.0f, 2.0f, 5.0f),
    glm::vec3(-7.0f, 2.0f, -5.0f)
};

float monkeyRotations[] = {
    glm::radians(.0f),
    glm::radians(135.0f),
    glm::radians(-135.0f),
    glm::radians(0.0f)
};

glm::vec3 monkeyScales[] = {
    glm::vec3(.5f,  .5f,  .5f),
    glm::vec3(.5f,  .5f,  .5f),
    glm::vec3(.5f,  .5f,  .5f),
    glm::vec3(.5f,  .5f,  .5f)
};

Camera* camera = new Camera(glm::vec3(10, 2, -10.0f), glm::radians(15.0f), glm::radians(180.0f), glm::vec3(0, 1.0f, 0));

LightDirectional* lightDirectional = new LightDirectional(
    glm::vec3(1.0f, 1.0f, 0.0f),
    glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
    glm::vec3(1.0f, 1.0f, 1.0f));

LightPoint* lightPoint0 = new LightPoint(
    glm::vec3(10.0f, 5.0f, 10.0f),
    glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
    glm::vec3(1.0f, 1.0f, 1.0f));

LightPoint* lightPoint1 = new LightPoint(
    glm::vec3(10.0f, 5.0f, -10.0f),
    glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
    glm::vec3(1.0f, 1.0f, 1.0f));

LightPoint* lightPoint2 = new LightPoint(
    glm::vec3(-10.0f, 5.0f, 10.0f),
    glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
    glm::vec3(1.0f, 1.0f, 1.0f));

LightPoint* lightPoint3 = new LightPoint(
    glm::vec3(-10.0f, 5.0f, -10.0f),
    glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
    glm::vec3(1.0f, 1.0f, 1.0f));

LightPoint* lightPoint4 = new LightPoint(
    glm::vec3(10.0f, 5.0f, 10.0f),
    glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0),
    glm::vec3(1.0f, 1.0f, 1.0f));

bool isSpotLightOpen[3]{ true, true, true };

Material* myMaterial;
Material* monkeyMaterials[4];
Material* megumiMaterial;
Material* wineMaterial;
Shader* myShader;

unsigned int monkeyVAO;
unsigned int monkeyVBO;

std::vector<float> monkeyModel;
ObjLoader* objLoader = new ObjLoader();
bool res = objLoader->loadOBJ("Monkey.obj", monkeyModel);

unsigned int megumiVAO;
unsigned int megumiVBO;

std::vector<float> megumiModel;
bool res1 = objLoader->loadOBJ("player1.obj", megumiModel);

unsigned int wineVAO;
unsigned int wineVBO;

std::vector<float> wineModel;
bool res2 = objLoader->loadOBJ("book.obj", wineModel);

unsigned int TexBuffer;

glm::mat4 modelMat;
glm::mat4 viewMat;
glm::mat4 projectionMat;

unsigned int LoadImageToGPU(const char* fileName, GLint internalFormat, GLenum format, int textureSlot) {
    unsigned int textureBuffer;

    glGenTextures(1, &textureBuffer);
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, textureBuffer);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return textureBuffer;
}

void OnFrameInit() {
    myShader = new Shader("vertexSource.vert", "fragmentSource.frag");
    myMaterial = new Material(myShader,
        glm::vec3(.01f, .01f, .01f),
        LoadImageToGPU("Brick.jpg", GL_RGB, GL_RGB, Shader::DIFFUSE),
        LoadImageToGPU("Specular.jpg", GL_RGB, GL_RGB, Shader::SPECULAR),
        32);

    monkeyMaterials[0] = new Material(myShader,
        glm::vec3(.01f, .01f, .01f),
        LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, Shader::DIFFUSE),
        LoadImageToGPU("Specular.jpg", GL_RGB, GL_RGB, Shader::SPECULAR),
        128);

    monkeyMaterials[1] = new Material(myShader,
        glm::vec3(.01f, .01f, .01f),
        LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, Shader::DIFFUSE),
        LoadImageToGPU("Specular.jpg", GL_RGB, GL_RGB, Shader::SPECULAR),
        32);

    monkeyMaterials[2] = new Material(myShader,
        glm::vec3(.01f, .01f, .01f),
        LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, Shader::DIFFUSE),
        LoadImageToGPU("Specular.jpg", GL_RGB, GL_RGB, Shader::SPECULAR),
        32);

    monkeyMaterials[3] = new Material(myShader,
        glm::vec3(.01f, .01f, .01f),
        LoadImageToGPU("container2.png", GL_RGBA, GL_RGBA, Shader::DIFFUSE),
        LoadImageToGPU("Specular.jpg", GL_RGB, GL_RGB, Shader::SPECULAR),
        32);

    megumiMaterial = new Material(myShader,
        glm::vec3(.01f, .01f, .01f),
        LoadImageToGPU("player1.png", GL_RGB, GL_RGB, Shader::DIFFUSE),
        LoadImageToGPU("Specular.jpg", GL_RGB, GL_RGB, Shader::SPECULAR),
        32);

    wineMaterial = new Material(myShader,
        glm::vec3(.01f, .01f, .01f),
        LoadImageToGPU("book.png", GL_RGB, GL_RGB, Shader::DIFFUSE),
        LoadImageToGPU("Specular.jpg", GL_RGB, GL_RGB, Shader::SPECULAR),
        32);

    glGenVertexArrays(1, &VAO);     // 生成一個VAO
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);          // 生成一個VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 搬資料到第0個attribute(第幾個attribute location, 1筆資料有幾個, 資料型別, 是否align 到+-1之間, 每筆資料間隔長度, 資料起始偏移量)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   // 啟用第0個attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // Monkey Model
    glGenVertexArrays(1, &monkeyVAO);     
    glBindVertexArray(monkeyVAO);

    glGenBuffers(1, &monkeyVBO);          
    glBindBuffer(GL_ARRAY_BUFFER, monkeyVBO);
    glBufferData(GL_ARRAY_BUFFER, monkeyModel.size() * sizeof(float), &monkeyModel[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Megumi Model
    glGenVertexArrays(1, &megumiVAO);
    glBindVertexArray(megumiVAO);

    glGenBuffers(1, &megumiVBO);
    glBindBuffer(GL_ARRAY_BUFFER, megumiVBO);
    glBufferData(GL_ARRAY_BUFFER, megumiModel.size() * sizeof(float), &megumiModel[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Wine Model
    glGenVertexArrays(1, &wineVAO);
    glBindVertexArray(wineVAO);

    glGenBuffers(1, &wineVBO);
    glBindBuffer(GL_ARRAY_BUFFER, wineVBO);
    glBufferData(GL_ARRAY_BUFFER, wineModel.size() * sizeof(float), &wineModel[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Projection Matrix
    projectionMat = glm::perspective(glm::radians(45.0f), 1600.0f / 1200.0f, 0.1f, 100.0f);
}

float pointCircularTimer = 0.0f;

void OnFrameUpdate(float deltaTime) {

    glBindVertexArray(VAO);             // 跟OpenGL說我們要綁定哪個物件的VAO

    viewMat = camera->GetViewMatrix();

    pointCircularTimer += deltaTime / 3;

    float lightTheta = pointCircularTimer * (float)3.1415926f * 2;
    if (lightTheta >= (float)3.1415926f * 2.0f) {
        lightTheta -= (float)3.1415926f * 2.0f;
        pointCircularTimer = 0.0f;
    }
    float x = 5 * cosf(lightTheta);
    float z = 5 * sinf(lightTheta);

    lightPoint0->position.x = x;
    lightPoint0->position.z = z;

    for (int i = 0; i < sizeof(cubePositions) / sizeof cubePositions[0]; i++)
    {
        glm::mat4 moveModelMat;
        moveModelMat = glm::translate(moveModelMat, cubePositions[i]);
        moveModelMat = glm::scale(moveModelMat, cubeScales[i]);

        myShader->UseProgram();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, myMaterial->diffuse);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, myMaterial->specular);

        glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(moveModelMat));
        glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMat));

        glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"), camera->Position.x, camera->Position.y, camera->Position.z);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.pos"), lightDirectional->position.x, lightDirectional->position.y, lightDirectional->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.color"), lightDirectional->color.x, lightDirectional->color.y, lightDirectional->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.dirToLight"), lightDirectional->direction.x, lightDirectional->direction.y, lightDirectional->direction.z);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.pos"), lightPoint0->position.x, lightPoint0->position.y, lightPoint0->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.color"), lightPoint0->color.x, lightPoint0->color.y, lightPoint0->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.dirToLight"), lightPoint0->direction.x, lightPoint0->direction.y, lightPoint0->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.constant"), lightPoint0->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.linear"), lightPoint0->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.quadratic"), lightPoint0->quadratic);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.pos"), lightPoint1->position.x, lightPoint1->position.y, lightPoint1->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.color"), lightPoint1->color.x, lightPoint1->color.y, lightPoint1->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.dirToLight"), lightPoint1->direction.x, lightPoint1->direction.y, lightPoint1->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.constant"), lightPoint1->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.linear"), lightPoint1->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.quadratic"), lightPoint1->quadratic);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.pos"), lightPoint2->position.x, lightPoint2->position.y, lightPoint2->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.color"), lightPoint2->color.x, lightPoint2->color.y, lightPoint2->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.dirToLight"), lightPoint2->direction.x, lightPoint2->direction.y, lightPoint2->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.constant"), lightPoint2->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.linear"), lightPoint2->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.quadratic"), lightPoint2->quadratic);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.pos"), lightPoint3->position.x, lightPoint3->position.y, lightPoint3->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.color"), lightPoint3->color.x, lightPoint3->color.y, lightPoint3->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.dirToLight"), lightPoint3->direction.x, lightPoint3->direction.y, lightPoint3->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.constant"), lightPoint3->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.linear"), lightPoint3->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.quadratic"), lightPoint3->quadratic);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.pos"), lightPoint4->position.x, lightPoint4->position.y, lightPoint4->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.color"), lightPoint4->color.x, lightPoint4->color.y, lightPoint4->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.dirToLight"), lightPoint4->direction.x, lightPoint4->direction.y, lightPoint4->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.constant"), lightPoint4->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.linear"), lightPoint4->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.quadratic"), lightPoint4->quadratic);

        myMaterial->shader->SetUniform3f("material.ambient", myMaterial->ambient);
        myMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
        myMaterial->shader->SetUniform1i("material.specular", Shader::SPECULAR);
        myMaterial->shader->SetUniform1f("material.shininess", myMaterial->shininess);

        glDrawArrays(GL_TRIANGLES, 0, 36);   // (畫的形式, 從哪個頂點開始畫, 要畫幾個頂點) 
    }

    glBindVertexArray(monkeyVAO);

    for (int i = 0; i < 4; i++) {
        glm::mat4 moveModelMat;
        moveModelMat = glm::translate(moveModelMat, monkeyPositions[i]);
        moveModelMat = glm::rotate(moveModelMat, monkeyRotations[i], glm::vec3(0.0, 1.0, 0.0));
        moveModelMat = glm::scale(moveModelMat, monkeyScales[i]);

        myShader->UseProgram();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, monkeyMaterials[i]->diffuse);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, monkeyMaterials[i]->specular);

        glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(moveModelMat));
        glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMat));

        glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"), camera->Position.x, camera->Position.y, camera->Position.z);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.pos"), lightDirectional->position.x, lightDirectional->position.y, lightDirectional->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.color"), lightDirectional->color.x, lightDirectional->color.y, lightDirectional->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.dirToLight"), lightDirectional->direction.x, lightDirectional->direction.y, lightDirectional->direction.z);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.pos"), lightPoint0->position.x, lightPoint0->position.y, lightPoint0->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.color"), lightPoint0->color.x, lightPoint0->color.y, lightPoint0->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.dirToLight"), lightPoint0->direction.x, lightPoint0->direction.y, lightPoint0->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.constant"), lightPoint0->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.linear"), lightPoint0->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.quadratic"), lightPoint0->quadratic);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.pos"), lightPoint1->position.x, lightPoint1->position.y, lightPoint1->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.color"), lightPoint1->color.x, lightPoint1->color.y, lightPoint1->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.dirToLight"), lightPoint1->direction.x, lightPoint1->direction.y, lightPoint1->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.constant"), lightPoint1->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.linear"), lightPoint1->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.quadratic"), lightPoint1->quadratic);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.pos"), lightPoint2->position.x, lightPoint2->position.y, lightPoint2->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.color"), lightPoint2->color.x, lightPoint2->color.y, lightPoint2->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.dirToLight"), lightPoint2->direction.x, lightPoint2->direction.y, lightPoint2->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.constant"), lightPoint2->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.linear"), lightPoint2->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.quadratic"), lightPoint2->quadratic);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.pos"), lightPoint3->position.x, lightPoint3->position.y, lightPoint3->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.color"), lightPoint3->color.x, lightPoint3->color.y, lightPoint3->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.dirToLight"), lightPoint3->direction.x, lightPoint3->direction.y, lightPoint3->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.constant"), lightPoint3->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.linear"), lightPoint3->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.quadratic"), lightPoint3->quadratic);

        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.pos"), lightPoint4->position.x, lightPoint4->position.y, lightPoint4->position.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.color"), lightPoint4->color.x, lightPoint4->color.y, lightPoint4->color.z);
        glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.dirToLight"), lightPoint4->direction.x, lightPoint4->direction.y, lightPoint4->direction.z);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.constant"), lightPoint4->constant);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.linear"), lightPoint4->linear);
        glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.quadratic"), lightPoint4->quadratic);

        monkeyMaterials[i]->shader->SetUniform3f("material.ambient", monkeyMaterials[i]->ambient);
        monkeyMaterials[i]->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
        monkeyMaterials[i]->shader->SetUniform1i("material.specular", Shader::SPECULAR);
        monkeyMaterials[i]->shader->SetUniform1f("material.shininess", monkeyMaterials[i]->shininess);

        glDrawArrays(GL_TRIANGLES, 0, monkeyModel.size() / 8);   // (畫的形式, 從哪個頂點開始畫, 要畫幾個頂點) 
    }

#pragma region - draw Megumi -
    glBindVertexArray(megumiVAO);

    glm::mat4 moveModelMat;
    moveModelMat = glm::translate(moveModelMat, glm::vec3(10, 1, -13));

    myShader->UseProgram();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, megumiMaterial->diffuse);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, megumiMaterial->specular);

    glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(moveModelMat));
    glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMat));

    glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"), camera->Position.x, camera->Position.y, camera->Position.z);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.pos"), lightDirectional->position.x, lightDirectional->position.y, lightDirectional->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.color"), lightDirectional->color.x, lightDirectional->color.y, lightDirectional->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.dirToLight"), lightDirectional->direction.x, lightDirectional->direction.y, lightDirectional->direction.z);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.pos"), lightPoint0->position.x, lightPoint0->position.y, lightPoint0->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.color"), lightPoint0->color.x, lightPoint0->color.y, lightPoint0->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.dirToLight"), lightPoint0->direction.x, lightPoint0->direction.y, lightPoint0->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.constant"), lightPoint0->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.linear"), lightPoint0->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.quadratic"), lightPoint0->quadratic);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.pos"), lightPoint1->position.x, lightPoint1->position.y, lightPoint1->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.color"), lightPoint1->color.x, lightPoint1->color.y, lightPoint1->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.dirToLight"), lightPoint1->direction.x, lightPoint1->direction.y, lightPoint1->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.constant"), lightPoint1->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.linear"), lightPoint1->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.quadratic"), lightPoint1->quadratic);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.pos"), lightPoint2->position.x, lightPoint2->position.y, lightPoint2->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.color"), lightPoint2->color.x, lightPoint2->color.y, lightPoint2->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.dirToLight"), lightPoint2->direction.x, lightPoint2->direction.y, lightPoint2->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.constant"), lightPoint2->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.linear"), lightPoint2->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.quadratic"), lightPoint2->quadratic);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.pos"), lightPoint3->position.x, lightPoint3->position.y, lightPoint3->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.color"), lightPoint3->color.x, lightPoint3->color.y, lightPoint3->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.dirToLight"), lightPoint3->direction.x, lightPoint3->direction.y, lightPoint3->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.constant"), lightPoint3->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.linear"), lightPoint3->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.quadratic"), lightPoint3->quadratic);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.pos"), lightPoint4->position.x, lightPoint4->position.y, lightPoint4->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.color"), lightPoint4->color.x, lightPoint4->color.y, lightPoint4->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.dirToLight"), lightPoint4->direction.x, lightPoint4->direction.y, lightPoint4->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.constant"), lightPoint4->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.linear"), lightPoint4->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.quadratic"), lightPoint4->quadratic);

    megumiMaterial->shader->SetUniform3f("material.ambient", megumiMaterial->ambient);
    megumiMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
    megumiMaterial->shader->SetUniform1i("material.specular", Shader::SPECULAR);
    megumiMaterial->shader->SetUniform1f("material.shininess", megumiMaterial->shininess);

    glDrawArrays(GL_TRIANGLES, 0, megumiModel.size() / 8);   // (畫的形式, 從哪個頂點開始畫, 要畫幾個頂點) 
#pragma endregion

#pragma region - draw Wine -
    glBindVertexArray(wineVAO);

    glm::mat4 bookModelMat;
    bookModelMat = glm::translate(bookModelMat, glm::vec3(7, 1, 13));
    //bookModelMat = glm::rotate(bookModelMat, glm::radians(180.0f), glm::vec3(0, 1, 0));

    myShader->UseProgram();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wineMaterial->diffuse);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, wineMaterial->specular);

    glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(bookModelMat));
    glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(glGetUniformLocation(myShader->ID, "projectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMat));

    glUniform3f(glGetUniformLocation(myShader->ID, "cameraPos"), camera->Position.x, camera->Position.y, camera->Position.z);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.pos"), lightDirectional->position.x, lightDirectional->position.y, lightDirectional->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.color"), lightDirectional->color.x, lightDirectional->color.y, lightDirectional->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightDirectional.dirToLight"), lightDirectional->direction.x, lightDirectional->direction.y, lightDirectional->direction.z);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.pos"), lightPoint0->position.x, lightPoint0->position.y, lightPoint0->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.color"), lightPoint0->color.x, lightPoint0->color.y, lightPoint0->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint0.dirToLight"), lightPoint0->direction.x, lightPoint0->direction.y, lightPoint0->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.constant"), lightPoint0->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.linear"), lightPoint0->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint0.quadratic"), lightPoint0->quadratic);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.pos"), lightPoint1->position.x, lightPoint1->position.y, lightPoint1->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.color"), lightPoint1->color.x, lightPoint1->color.y, lightPoint1->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint1.dirToLight"), lightPoint1->direction.x, lightPoint1->direction.y, lightPoint1->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.constant"), lightPoint1->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.linear"), lightPoint1->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint1.quadratic"), lightPoint1->quadratic);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.pos"), lightPoint2->position.x, lightPoint2->position.y, lightPoint2->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.color"), lightPoint2->color.x, lightPoint2->color.y, lightPoint2->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint2.dirToLight"), lightPoint2->direction.x, lightPoint2->direction.y, lightPoint2->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.constant"), lightPoint2->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.linear"), lightPoint2->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint2.quadratic"), lightPoint2->quadratic);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.pos"), lightPoint3->position.x, lightPoint3->position.y, lightPoint3->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.color"), lightPoint3->color.x, lightPoint3->color.y, lightPoint3->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint3.dirToLight"), lightPoint3->direction.x, lightPoint3->direction.y, lightPoint3->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.constant"), lightPoint3->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.linear"), lightPoint3->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint3.quadratic"), lightPoint3->quadratic);

    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.pos"), lightPoint4->position.x, lightPoint4->position.y, lightPoint4->position.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.color"), lightPoint4->color.x, lightPoint4->color.y, lightPoint4->color.z);
    glUniform3f(glGetUniformLocation(myShader->ID, "lightPoint4.dirToLight"), lightPoint4->direction.x, lightPoint4->direction.y, lightPoint4->direction.z);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.constant"), lightPoint4->constant);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.linear"), lightPoint4->linear);
    glUniform1f(glGetUniformLocation(myShader->ID, "lightPoint4.quadratic"), lightPoint4->quadratic);

    wineMaterial->shader->SetUniform3f("material.ambient", wineMaterial->ambient);
    wineMaterial->shader->SetUniform1i("material.diffuse", Shader::DIFFUSE);
    wineMaterial->shader->SetUniform1i("material.specular", Shader::SPECULAR);
    wineMaterial->shader->SetUniform1f("material.shininess", wineMaterial->shininess);

    glDrawArrays(GL_TRIANGLES, 0, wineModel.size() / 8);   // (畫的形式, 從哪個頂點開始畫, 要畫幾個頂點) 
#pragma endregion

    camera->UpdateCameraPos(deltaTime);
}

bool lastBKeyPressed = false;
bool lastNKeyPressed = false;
bool lastMKeyPressed = false;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    camera->speedX = 0.0f;
    camera->speedY = 0.0f;
    camera->speedZ = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->speedZ = 5.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->speedZ = -5.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->speedX = -5.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->speedX = 5.0f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        lightPoint0->color = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        lightPoint0->color = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        lightPoint0->color = glm::vec3(0.0f, 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        lightPoint0->color = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    if (camera->Position.x > 19) camera->Position.x = 19;
    if (camera->Position.x < -19) camera->Position.x = -19;
    if (camera->Position.z > 19) camera->Position.z = 19;
    if (camera->Position.z < -19) camera->Position.z = -19;
}

float lastX, lastY;
bool isFirstMouse = true;

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    if (isFirstMouse) {
        lastX = xPos;
        lastY = yPos;
        isFirstMouse = false;
    }

    float deltaX, deltaY;

    deltaX = xPos - lastX;
    deltaY = yPos - lastY;

    lastX = xPos;
    lastY = yPos;

    camera->ProcessMouseMovement(deltaX, deltaY);
}

int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1600 * 1.5f, 1200 * 1.5f, "HNBDOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    // 關閉鼠標
    glfwSetCursorPosCallback(window, mouse_callback);

    // Init GLEW
    glewExperimental = true;    // 有些功能還是實驗階段，要使用要打開
    if (glewInit() != GLEW_OK) {
        printf("Init GLEW failed.");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 1600 * 1.5f, 1200 * 1.5f);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);    // 启用深度测试

    // --------------------------------------------------

    OnFrameInit();

    double currentFrameTime = glfwGetTime();
    double lastFrameTime = currentFrameTime;
    double deltaTime;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // 也要清除深度Buffer

        currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        OnFrameUpdate(deltaTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}