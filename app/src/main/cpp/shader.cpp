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

    if (mTexture.mLocation != -1) {
        glBindTexture(GL_TEXTURE_2D, mTexture.mTexture);
        glUniform1i(mTexture.mLocation, 0);
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
    if (mTexture.mLocation == -1) {
        GLuint location = glGetUniformLocation(mProgram, name);
        if (location != -1) {
            mTexture.mLocation = location;
            mTexture.mTexture = CreateTexture2DFromBMP(imagePath);
        }
    }
}