#include "ground.h"
void Ground::Init() {
//    Vertex vertex[1600];
//    for (int z = 0; z < 20; ++z) {
//        float zStart = 100.0f - z * 10.0f;
//        for (int x = 0; x < 20; ++x) {
//            int offset = (x + z * 20) * 4;
//            float xStart = x * 10.0f - 100.0f;
//            // TriangleStrip
//            vertex[offset].Position[0] = xStart;
//            vertex[offset].Position[1] = -1.0f;
//            vertex[offset].Position[2] = zStart;
//            vertex[offset].Position[3] = 1.0f;
//
//            vertex[offset + 1].Position[0] = xStart + 10.0f;
//            vertex[offset + 1].Position[1] = -1.0f;
//            vertex[offset + 1].Position[2] = zStart;
//            vertex[offset + 1].Position[3] = 1.0f;
//
//            vertex[offset + 2].Position[0] = xStart ;
//            vertex[offset + 2].Position[1] = -1.0f;
//            vertex[offset + 2].Position[2] = zStart - 10.0f;
//            vertex[offset + 2].Position[3] = 1.0f;
//
//            vertex[offset + 3].Position[0] = xStart + 10.0f;
//            vertex[offset + 3].Position[1] = -1.0f;
//            vertex[offset + 3].Position[2] = zStart - 10.0f;
//            vertex[offset + 3].Position[3] = 1.0f;
//        }
//    }
//    mVBO = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(Vertex) * 1600, GL_STATIC_DRAW, vertex);

    // 封装VertexBuffer后改造
    mVertexBuffer = new VertexBuffer;
    mVertexBuffer -> SetSize(1600);
    for (int z = 0; z < 20; ++z) {
       float zStart = 100.0f - z * 10.0f;
        for (int x = 0; x < 20; ++x) {
            int offset = (x + z * 20) * 4;
            float xStart = x * 10.0f - 100.0f;
            mVertexBuffer -> SetPosition(offset, xStart, -1.0f, zStart);
            mVertexBuffer -> SetPosition(offset + 1, xStart + 10.0f, -1.0f, zStart);
            mVertexBuffer -> SetPosition(offset + 2, xStart, -1.0f, zStart - 10.0f);
            mVertexBuffer -> SetPosition(offset + 3, xStart + 10.0f, -1.0f, zStart - 10.0f);
            mVertexBuffer -> SetNormal(offset, 0.0f, 1.0f, 0.0f);
            mVertexBuffer -> SetNormal(offset + 1, 0.0f, 1.0f, 0.0f);
            mVertexBuffer -> SetNormal(offset + 2, 0.0f, 1.0f, 0.0f);
            mVertexBuffer -> SetNormal(offset + 3, 0.0f, 1.0f, 0.0f);
            if ((z % 2) ^ (x % 2)) {
                mVertexBuffer -> SetColor(offset, 0.1f, 0.1f, 0.1f);
                mVertexBuffer -> SetColor(offset + 1, 0.1f, 0.1f, 0.1f);
                mVertexBuffer -> SetColor(offset + 2, 0.1f, 0.1f, 0.1f);
                mVertexBuffer -> SetColor(offset + 3, 0.1f, 0.1f, 0.1f);
            } else {
                mVertexBuffer -> SetColor(offset, 0.8f, 0.8f, 0.8f);
                mVertexBuffer -> SetColor(offset + 1, 0.8f, 0.8f, 0.8f);
                mVertexBuffer -> SetColor(offset + 2, 0.8f, 0.8f, 0.8f);
                mVertexBuffer -> SetColor(offset + 3, 0.8f, 0.8f, 0.8f);
            }
        }
    }
    // ------------------------------封装VBO封装到VertexBuffer后注释掉----------------------------------
    // mVBO = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertexBuffer -> mVertexCount, GL_STATIC_DRAW, mVertexBuffer -> mVertexes);
    // ------------------------------封装VBO封装到VertexBuffer后注释掉----------------------------------
    mShader = new Shader;
    mShader -> Init("Res/ground_vertex.glsl", "Res/ground_fragment.glsl");

    // ------------------------------封装Shader类后注释掉----------------------------------
//    int fileSize = 0;
//    unsigned char * vShaderCode = LoadFileContent("Res/ground_vertex.glsl", fileSize);
//    GLuint vsShader = CompileShader(GL_VERTEX_SHADER, (char*) vShaderCode);
//    delete vShaderCode;
//    unsigned char * fShaderCode = LoadFileContent("Res/ground_fragment.glsl", fileSize);
//    GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, (char*) fShaderCode);
//    delete fShaderCode;
//    mProgram = CreateProgram(vsShader, fsShader);
//    glDeleteShader(vsShader);
//    glDeleteShader(fsShader);
//    mPositionLocation = glGetAttribLocation(mProgram, "position");
//    mColorLocation = glGetAttribLocation(mProgram, "color");
//    mNormalLocation = glGetAttribLocation(mProgram, "normal");
//    mModelMatrixLocation = glGetUniformLocation(mProgram, "ModelMatrix");
//    mViewMatrixLocation = glGetUniformLocation(mProgram, "ViewMatrix");
//    mProjectionMatrixLocation = glGetUniformLocation(mProgram, "ProjectionMatrix");
    // ------------------------------封装Shader类后注释掉----------------------------------
}

void Ground::Draw(glm::mat4 & viewMatrix, glm::mat4 & projectionMatrix) {
    glEnable(GL_DEPTH_TEST);
    // ------------------------------封装VBO封装到VertexBuffer后注释掉----------------------------------
    // glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    // ------------------------------封装VBO封装到VertexBuffer后注释掉----------------------------------

    //   mVertexBuffer -> Bind() 需要在mShader -> Bind 之前调用，先上传数据，再设置属性
    mVertexBuffer -> Bind();
    mShader -> Bind(glm::value_ptr(mModeMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));

    for (int i = 0; i < 400; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }
    mVertexBuffer -> Unbind();
    // ------------------------------封装VBO封装到VertexBuffer后注释掉----------------------------------
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // ------------------------------封装VBO封装到VertexBuffer后注释掉----------------------------------

}