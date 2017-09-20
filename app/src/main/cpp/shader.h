#pragma once

#include "ggl.h"
class Shader {
public:
    GLuint mProgram;
    GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;
    GLint mPositionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
    // vs fs 分别指向VertexShader FragmentShader 的路径
    void Init(const char * vs, const char * fs);
    void Bind(float *M, float *V, float *P);
};