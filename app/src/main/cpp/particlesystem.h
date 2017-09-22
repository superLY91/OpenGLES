#pragma once

#include "vertexbuffer.h"
#include "shader.h"
class ParticleSystem {
    VertexBuffer *mVertexBuffer;
    glm::mat4 mModelMatrix;
    Shader *mShader;
public:
    void Init(float x, float y, float z);
    void Draw(glm::mat4 &V, glm::mat4 &P);
    // 两帧之间的间隔
    void Update(float deltaTime);
};