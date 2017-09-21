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

struct UniformVector4f {
    GLint mLocation;
    float v[4];
    UniformVector4f() {
        mLocation = -1;
        memset(v, 0, sizeof(float) * 4);
    }
};
class Shader {
public:
    GLuint mProgram;
    // 储存多张纹理
    std::map<std::string, UniformTexture*> mUniformTextures;
    std::map<std::string, UniformVector4f*> mUniformVec4s;
    GLint mModelMatrixLocation, mViewMatrixLocation, mProjectionMatrixLocation;
    GLint mPositionLocation, mColorLocation, mTexcoordLocation, mNormalLocation;
    // vs fs 分别指向VertexShader FragmentShader 的路径
    void Init(const char * vs, const char * fs);
    void Bind(float *M, float *V, float *P);
    void SetTexture(const char * name, const char * imagePath);
    void SetVec4(const char * name, float x, float y, float z, float w);
};