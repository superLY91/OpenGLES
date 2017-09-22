#include <glm/glm.hpp>
#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "ground.h"
#include "model.h"
#include "skybox.h"
#include "particlesystem.h"
// 没有设置时默认为单位矩阵
glm::mat4 modelMartix, viewMatrix, projectionMatrix;
glm::vec3 cameraPos(10.0f, 2.0f, 10.0f);
Ground ground;
Model model, niutou;
SkyBox skyBox;
ParticleSystem ps;
void Init() {
    viewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ground.Init();
    model.Init("Res/Sphere.obj");
    model.SetTexture("Res/earth.bmp");
    model.SetPosition(0.0f, 0.0f, 0.0f);
    skyBox.Init("Res/");
    niutou.Init("Res/niutou.obj");
//    niutou.SetTexture("Res/niutou.bmp");
    niutou.mModelMatrix = glm::translate(-5.0f, 0.0f, 4.0f) * glm::scale(0.05f, 0.05f, 0.05f);
    ps.Init(0.0f, 0.0f, 0.0f);
}

void SetViewPortSize(float width, float height) {
    // 60.0f: 视角
    projectionMatrix = glm::perspective(60.0f, width / height, 0.1f, 1000.0f);
}

void Draw() {
    float frameTime = GetFrameTime();
//    glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
    // 背景设置为黑色 方便观察地面光照
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 在所以物体绘制之前绘制天空盒，因为为了保证天空盒在最远处，skyBox.Draw关闭了深度测试
    skyBox.Draw(viewMatrix, projectionMatrix, cameraPos.x, cameraPos.y, cameraPos.z);

    ground.Draw(viewMatrix, projectionMatrix);
    model.Draw(viewMatrix, projectionMatrix, cameraPos.x, cameraPos.y, cameraPos.z);
    niutou.Draw(viewMatrix, projectionMatrix, cameraPos.x, cameraPos.y, cameraPos.z);
    ps.Update(frameTime);
    ps.Draw(viewMatrix, projectionMatrix);
}