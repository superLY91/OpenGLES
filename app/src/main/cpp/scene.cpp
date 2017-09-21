#include <glm/glm.hpp>
#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "ground.h"

GLuint texture;
// 没有设置时默认为单位矩阵
glm::mat4 modelMartix, viewMatrix, projectionMatrix;
Ground ground;
GLint texcoordLocation, textureLocation;
Shader *shader;
VertexBuffer *vertexBuffer;
void Init() {
    vertexBuffer = new VertexBuffer;
    vertexBuffer -> SetSize(3);
    vertexBuffer -> SetPosition(0, -0.2f, -0.2f, 0.0f);
    vertexBuffer -> SetTexcoord(0, 0.0f, 0.0f);
    vertexBuffer -> SetColor(0, 1.0f, 1.0f, 0.0f);
    vertexBuffer -> SetPosition(1, 0.2f, -0.2f, 0.0f);
    vertexBuffer -> SetTexcoord(1, 1.0f, 0.0f);
    vertexBuffer -> SetColor(1, 1.0f, 0.0f, 0.0f);
    vertexBuffer -> SetPosition(2, 0.0f, 0.2f, 0.0f);
    vertexBuffer -> SetTexcoord(2, 0.5f, 1.0f);
    vertexBuffer -> SetColor(2, 0.0f, 1.0f, 0.0f);
    shader = new Shader;
    shader -> Init("Res/test_vertex.glsl", "Res/test_fragment.glsl");
    shader -> SetTexture("U_Texture", "Res/test.bmp");
    modelMartix=glm::translate(0.0f, 0.0f, -0.6f);
    ground.Init();
}

void SetViewPortSize(float width, float height) {
    // 60.0f: 视角
    projectionMatrix = glm::perspective(60.0f, width / height, 0.1f, 1000.0f);
}

void Draw() {
    float frameTime = GetFrameTime();
    glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ground.Draw(viewMatrix, projectionMatrix);

    vertexBuffer -> Bind();
    shader -> Bind(glm::value_ptr(modelMartix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vertexBuffer -> Unbind();
}