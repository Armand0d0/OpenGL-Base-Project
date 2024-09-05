#include "gameItem.h"

unsigned int gameItem::loadMesh(float* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount) {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(float), vertices, GL_STATIC_DRAW);


    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount*sizeof(int), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    return VAO;
}
 unsigned int gameItem::loadTexture(const char* fileName) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);// or GL_LINEAR

    string fullFileName = "./textures/";
    fullFileName += fileName;
    int texWidth, texHeight, nrChannels;
    unsigned char* data = stbi_load(fullFileName.c_str(), &texWidth, &texHeight, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture : "<< fileName << std::endl;
    }
    stbi_image_free(data);
    return texture;
}
gameItem::gameItem(const char* name, float* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount, const char* textureFileName) :
    name(name),
    indices(indices),
    indexCount(indexCount),
    vertices(vertices),
    vertexCount(vertexCount),
    position(glm::vec3(0)),
    scale(glm::vec3(1.)),
    rotationAxis(Y),
    rotationAngle(0.) {

    this->VAO = gameItem::loadMesh(vertices, vertexCount, indices, indexCount);
    this->texture = gameItem::loadTexture(textureFileName);
}
