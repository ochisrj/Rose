#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaderclass.h"

class Cube
{
public:
    Cube();
    ~Cube();

    bool Init(const char* texturePath);
    void Draw(Shader& shader, const float color[4], const float rotation[3], bool showMultipleCubes);

private:
    unsigned int VAO, VBO;
    unsigned int texture;
    glm::vec3 cubePositions[10];

    void SetupMesh();
};
