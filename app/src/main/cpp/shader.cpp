#include "shader.h"
#include "utils.h"
#include "vertexbuffer.h"
void Shader::Init(const char *vs, const char *fs) {
    int nFileSize = 0;
    const char * vsCode = (char *) LoadFileContent(vs, nFileSize);
    const char * fsCode = (char *) LoadFileContent(fs, nFileSize);

    GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsCode);
    if (vsShader == 0) {
        return;
    }
    GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, fsCode);
    if (fsShader == 0) {
        return;
    }
    mProgram = CreateProgram(vsShader, fsShader);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);
    if (mProgram != 0) {
        mModelMatrixLocation = glGetUniformLocation(mProgram, "ModelMatrix");
        mViewMatrixLocation = glGetUniformLocation(mProgram, "ViewMatrix");
        mProjectionMatrixLocation = glGetUniformLocation(mProgram, "ProjectionMatrix");
        mPositionLocation = glGetAttribLocation(mProgram, "position");
        mColorLocation = glGetAttribLocation(mProgram, "color");
        mTexcoordLocation = glGetAttribLocation(mProgram, "texcoord");
        mNormalLocation = glGetAttribLocation(mProgram, "normal");
    }
}

void Shader::Bind(float *M, float *V, float *P) {
    glUseProgram(mProgram);
    glUniformMatrix4fv(mModelMatrixLocation, 1, GL_FALSE, M);
    glUniformMatrix4fv(mViewMatrixLocation, 1, GL_FALSE, V);
    glUniformMatrix4fv(mProjectionMatrixLocation, 1, GL_FALSE, P);

    // shader中每个变量都对应着一组插槽，
    // 设置纹理时有另一组纹理插槽（纹理单元texture unit)，
    // 纹理对象指图片
    // 需要将shader中的sampler2D(纹理采样器)的插槽，与纹理单元对应起来，并且将纹理对象绑定到纹理单元上
    int index = 0;
    for (auto iter = mUniformTextures.begin(); iter != mUniformTextures.end(); ++iter) {
        // 激活一个纹理单元(texture unit),从0号位置开始
        glActiveTexture(GL_TEXTURE0 + index);
        // 将某一个纹理对象设置当前的纹理对象，并把她指派给当前激活了的纹理单元
        glBindTexture(GL_TEXTURE_2D, iter->second->mTexture);
        // 告诉哪一个插槽应该取第几个纹理单元中取纹理，插槽和sampler2D是关联的，所以sampler2D就指定去哪采集纹理
        glUniform1i(iter->second->mLocation, index++);
    }

    for (auto iter = mUniformVec4s.begin(); iter != mUniformVec4s.end(); ++iter) {
        // 哪个槽上，放置多少个，物体
        glUniform4fv(iter->second->mLocation, 1, iter->second->v);
    }

    glEnableVertexAttribArray(mPositionLocation);
    glVertexAttribPointer(mPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    glEnableVertexAttribArray(mColorLocation);
    glVertexAttribPointer(mColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 4));

    glEnableVertexAttribArray(mTexcoordLocation);
    glVertexAttribPointer(mTexcoordLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 8));

    glEnableVertexAttribArray(mNormalLocation);
    glVertexAttribPointer(mNormalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 12));
}
/**
 *
 * @param name uniform变量在shader中的名字
 * @param imagePath 图片的路径
 */
void Shader::SetTexture(const char * name, const char * imagePath) {
    // 如果找到了证明之前设置过 只需要更新纹理，
    // 如果没有找到，创建新UniformTexture，并添加到mUniformTextures中
    auto iter = mUniformTextures.find(name);
    if (iter == mUniformTextures.end()) {
        GLuint location = glGetUniformLocation(mProgram, name);
        if (location != -1) {
            UniformTexture *t = new UniformTexture;
            t -> mLocation = location;
            t -> mTexture =  CreateTexture2DFromBMP(imagePath);
            mUniformTextures.insert(std::pair<std::string, UniformTexture*>(name, t));
        }
    } else {
        glDeleteTextures(1, &iter->second->mTexture);
        iter->second->mTexture = CreateTexture2DFromBMP(name);
    }
}

void Shader::SetVec4(const char *name, float x, float y, float z, float w) {
    auto iter = mUniformVec4s.find(name);
    if (iter == mUniformVec4s.end()) {
        GLint location = glGetUniformLocation(mProgram, name);
        if (location != -1) {
            UniformVector4f * v = new UniformVector4f;
            v -> v[0] = x;
            v -> v[1] = y;
            v -> v[2] = z;
            v -> v[3] = w;
            v -> mLocation = location;
            mUniformVec4s.insert(std::pair<std::string, UniformVector4f*>(name, v));
        }
    } else {
        iter -> second -> v[0] = x;
        iter -> second -> v[1] = y;
        iter -> second -> v[2] = z;
        iter -> second -> v[3] = w;
    }
}