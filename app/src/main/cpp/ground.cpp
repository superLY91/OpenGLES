#include "ground.h"
void Ground::Init() {
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
    mShader = new Shader;
    mShader -> Init("Res/ground_vertex.glsl", "Res/ground_fragment.glsl");
    // 设置光源位置
    mShader -> SetVec4("U_LightPos", 0.0f, 0.0f, 1.0f, 0.0f);
    // 设置光的 环境光 分量 白色
    mShader -> SetVec4("U_LightAmbient", 1.0f, 1.0f, 1.0f, 1.0f);
    // 设置光的 漫反射 分量 白色
    mShader -> SetVec4("U_LightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    // 设置模型表面对 环境光 的反射系数
    mShader -> SetVec4("U_AmbientMaterial", 0.1f, 0.1f, 0.1f, 1.0f);
    // 设置模型表面对 漫反射 的反射系数
    mShader -> SetVec4("U_DiffuseMaterial", 0.6f, 0.6f, 0.6f, 1.0f);

}

void Ground::Draw(glm::mat4 & viewMatrix, glm::mat4 & projectionMatrix) {
    glEnable(GL_DEPTH_TEST);

    // mVertexBuffer -> Bind() 需要在mShader -> Bind 之前调用，先上传数据，再设置属性
    mVertexBuffer -> Bind();
    mShader -> Bind(glm::value_ptr(mModeMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));

    for (int i = 0; i < 400; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }
    mVertexBuffer -> Unbind();
}