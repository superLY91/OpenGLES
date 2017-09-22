#include "particlesystem.h"
#include "utils.h"
void ParticleSystem::Init(float x, float y, float z) {
    mModelMatrix = glm::translate(x, y, z);
    mVertexBuffer = new VertexBuffer;
    int particleCount = 180;
    mVertexBuffer -> SetSize(particleCount);
    for (int i = 0; i < particleCount; ++i) {
        // 每两个点之间相隔8度
        mVertexBuffer -> SetPosition(i, 2.0f * cosf(float(i) * 8.0f * 3.14f / 180.0f), 0.0f, 2.0f * sinf(float(i) * 8.0f * 3.14f / 180.0f));
        mVertexBuffer -> SetColor(i, 0.1f, 0.4f, 0.6f);
    }
    mShader = new Shader;
    mShader -> Init("Res/particle_vertex.glsl", "Res/particle_fragment.glsl");
    mShader -> SetTexture("U_texture", CreateProcedureTexture(128));
}
void ParticleSystem::Draw(glm::mat4 &V, glm::mat4 &P) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    mVertexBuffer -> Bind();
    mShader -> Bind(glm::value_ptr(mModelMatrix), glm::value_ptr(V), glm::value_ptr(P));
    glDrawArrays(GL_POINTS, 0, mVertexBuffer -> mVertexCount);
    mVertexBuffer -> Unbind();
    glDisable(GL_BLEND);
}

void ParticleSystem::Update(float deltaTime) {
    // 旋转角度
    static float angle = 0.0f;
    angle += deltaTime * 10.0f;
    mModelMatrix = glm::rotate(angle, 0.0f, 1.0f, 0.0f);
    // 增加向上的偏移
    for (int i = 0; i < mVertexBuffer -> mVertexCount; ++i) {
        Vertex &vertex = mVertexBuffer -> Get(i);
        vertex.Normal[1] = 0.1f * i;
        if (i > 90) {
            break;
        }
    }
}

