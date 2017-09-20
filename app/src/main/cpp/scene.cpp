#include <glm/glm.hpp>
#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "ground.h"
GLuint vbo; // (vertex buffer object) 将CPU上的数据发送到GPU
GLuint ebo; // (element buffer object) 指定访问数据的顺序
GLuint program;
// attribute和uniform 在GPU上的插槽是分开的
GLint positionLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation, colorLocation;
GLint texcoordLocation, textureLocation;
GLuint texture;
// 没有设置时默认为单位矩阵
glm::mat4 modelMartix, viewMatrix, projectionMatrix;
Ground ground;
void Init() {
    // cpu 端的数据 C语言端写的任何数据都属于CUP端
    float data[] = {
            -0.2f, -0.2f, -0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.2f, -0.2f, -0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.2f, -0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f
    };

    // 1: 申请一个vbo
    // &vbo : vbo标示变量的地址 指向GPU中的某一个内存块
    glGenBuffers(1, &vbo);
    // 设置当前的GL_ARRAY_BUFFER为我们的vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // 将数据从CPU端发送到GPU端
    // GL_ARRAY_BUFFER: 我们要操作GL_ARRAY_BUFFER
    // sizeof(float) * 12: 标示要在GPU端开辟多大的内存空间
    // data : CPU端数据的地址
    // GL_STATIC_DRAW: 当数据放到显卡上就不会取修改它了
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 30, data, GL_STATIC_DRAW);
    // 设置当前的GL_ARRAY_BUFFER为我们的0
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 绘制点的顺序
    unsigned short indexes[] = {0,1,2};
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 3, indexes, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int fileSize = 0;
    unsigned char * vShaderCode = LoadFileContent("Res/test_vertex.glsl", fileSize);
    GLuint vsShader = CompileShader(GL_VERTEX_SHADER, (char*) vShaderCode);
    delete vShaderCode;
    unsigned char * fShaderCode = LoadFileContent("Res/test_fragment.glsl", fileSize);
    GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, (char*) fShaderCode);
    delete fShaderCode;
    program = CreateProgram(vsShader, fsShader);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);

    positionLocation = glGetAttribLocation(program, "position");
    colorLocation = glGetAttribLocation(program, "color");
    texcoordLocation = glGetAttribLocation(program, "texcoord");
    modelMatrixLocation = glGetUniformLocation(program, "ModelMatrix");
    viewMatrixLocation = glGetUniformLocation(program, "ViewMatrix");
    projectionMatrixLocation = glGetUniformLocation(program, "ProjectionMatrix");
    textureLocation = glGetUniformLocation(program, "U_Texture");
    modelMartix=glm::translate(0.0f, 0.0f, -0.6f);
    texture = CreateTexture2DFromBMP("Res/test.bmp");

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

    // 选择一个程序来执行接下来的绘制指令
    glUseProgram(program);
    // 为program设置Uniform变量
    // modelMatrixLocation: 插槽的位置
    // 1: 设置几个矩阵，因为一个插槽上可能对应的是一个矩阵数值
    // GL_FALSE: 矩阵从cpu到gpu是否需要转置
    // glm::value_ptr(modelMartix): 数据的起始地址
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMartix));
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // 设置当前纹理
    glBindTexture(GL_TEXTURE_2D, texture);
    // 纹理对象和纹理插槽关联起来
    glUniform1i(textureLocation, 0);

    // 设置当前的数据集为 vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // 启用position的插槽
    glEnableVertexAttribArray(positionLocation);
    // 告诉GPU如果遍历vbo的内存块
    // positionLocation : 插槽的位置
    // 4：插槽中数据有几个分量 我们设置的position为xyzw 所以传4
    // GL_FLOAT: 数据类型
    // GL_FALSE: 是否要归一化 类型的映射 我们这里使用的是float不要映射 eg.颜色值 int -> float 255映射为1.0, 05映射为0.0
    // sizeof(float) * 4: 紧挨着的两个点的起始地址相距多远
    // 0: 从vbo的什么位置开始取值，0代表从号位置开始取值
    //glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, 0);

    glEnableVertexAttribArray(colorLocation);
    //glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 4));
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)(sizeof(float) * 4));

    glEnableVertexAttribArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)(sizeof(float) * 8));

    // GL_TRIANGLES: 绘制三角形
    // 0: 从第几个点开始绘制
    // 3: 要绘制几个点
//     glDrawArrays(GL_TRIANGLES, 0, 3); // 没有ebo时这样绘制数据
    // 通过ebo绘制数据
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // 3: 画三个索引数据 取ebo中三个数据来绘制三角形
    // 0: 数据的起始位置
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 当前程序设置为0号程序
    glUseProgram(0);
}