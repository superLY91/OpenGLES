#pragma once

#include "utils.h"
#include "vertexbuffer.h"
#include "shader.h"
class Ground{
    VertexBuffer *mVertexBuffer;
    Shader *mShader;
    glm::mat4 mModeMatrix;
    // ------------------------------封装Shader类后注释掉----------------------------------
//    GLuint mProgram;
//    GLint mPositionLocation, mColorLocation, mNormalLocation;
//    GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;
    // ------------------------------封装Shader类后注释掉----------------------------------

    // ------------------------------封装VBO封装到VertexBuffer后注释掉----------------------------------
    // GLuint mVBO;
    // ------------------------------封装VBO封装到VertexBuffer后注释掉----------------------------------

public:
    void Init();
    void Draw(glm::mat4 & viewMatrix, glm::mat4 & projectionMatrix);
};