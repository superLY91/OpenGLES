#pragma once

#include "vertexbuffer.h"
#include "shader.h"

class SkyBox {
    Shader *mShader;
    VertexBuffer *mVertexBuffer;
    glm::mat4 mModelMatrix;
public:
    void Init(const char * imageDir);
    void InitFront(const char *imageDir);
    void InitBack(const char *imageDir);
    void InitLeft(const char *imageDir);
    void InitRight(const char *imageDir);
    void InitTop(const char *imageDir);
    void InitBottom(const char *imageDir);
    // xyz 摄像机的位置
    void Draw(glm::mat4 &V, glm::mat4 &P, float x, float y, float z);
};