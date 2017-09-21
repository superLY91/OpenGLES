#include <glm/glm.hpp>
#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "ground.h"
#include "model.h"
// 没有设置时默认为单位矩阵
glm::mat4 modelMartix, viewMatrix, projectionMatrix;
Ground ground;
Model model;
void Init() {
    ground.Init();
    model.Init("Res/Sphere.obj");
    model.SetTexture("Res/earth.bmp");
    model.SetPosition(0.0f, 0.0f, -5.0f);
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
    model.Draw(viewMatrix, projectionMatrix);
}