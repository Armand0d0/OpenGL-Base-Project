#pragma once

#include <iostream>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "glad/glad.h"
#include "stb_image.h"

#define X glm::vec3(1.f,.0f,.0f)
#define Y glm::vec3(0.f,1.f,.0f)
#define Z glm::vec3(0.f,.0f,1.0f)
#define X1 glm::vec4(1.f,.0f,.0f,1.0f)
#define Y1 glm::vec4(0.f,1.f,.0f,1.0f)
#define Z1 glm::vec4(0.f,.0f,1.0f,1.0f)

using namespace std;

class gameItem {
public:
    const char* name;
    unsigned int* indices;
    unsigned int indexCount;
    float* vertices;
    unsigned int vertexCount;
    unsigned int texture;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotationAxis;
    float rotationAngle;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    glm::vec3 edgesColor;
    unsigned int loadMesh(float* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);
    static unsigned int loadTexture(const char* fileName);
    gameItem(const char* name, float* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount, const char* textureFileName);


};

