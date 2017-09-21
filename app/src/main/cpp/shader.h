#pragma once

#include "ggl.h"
struct UniformTexture {
    GLint mLocation; // 插槽位置
    GLuint mTexture;
    UniformTexture() {
        mLocation = -1;
        mTexture = 0;
    }
};

class Shader {
public:
    GLuint mProgram;
    // 储存多张纹理
    std::map<std::string, UniformTexture*> mUniformTextures;
    GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;
    GLint mPositionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
    // vs fs 分别指向VertexShader FragmentShader 的路径
    void Init(const char * vs, const char * fs);
    void Bind(float *M, float *V, float *P);
    void SetTexture(const char * name, const char * imagePath);
};