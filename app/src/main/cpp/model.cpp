#include "model.h"
#include "utils.h"
Model::Model() {

}
void Model::Init(const char *modelPath) {
    // 存储obj模型中某一行的3个或2个浮点数
    struct FloatData {
        float v[3];
    };

    // 定义了某一个唯一的点
    struct VertexDefine {
        int posIndex;
        int texcoordIndex;
        int normalIndex;
    };

    int nFileSize = 0;
    unsigned char*fileContent = LoadFileContent(modelPath, nFileSize);
    if (fileContent == nullptr) {
        return;
    }
    // 三种数据容器
    std::vector<FloatData> positions, texcoords, normals;
    // 申请一个容器，存放所有的唯一的点
    std::vector<VertexDefine> vertexes;
    // 解码数据
    std::stringstream ssFileContent((char*)fileContent);
    std::string temp;
    char szOneLine[256];
    while (!ssFileContent.eof()) {
        memset(szOneLine, 0, 256);
        ssFileContent.getline(szOneLine, 256);
        if (strlen(szOneLine) > 0) {
            if (szOneLine[0] == 'v') {
                // 数据输入到一个流中
                std::stringstream ssOneLine(szOneLine);
                if (szOneLine[1] == 't') {
                    // 流的输出一个字符串(vt)到temp，输出到空格截止，空格不输出
                    ssOneLine >> temp;
                    FloatData floatData;
                    // 流的输出一个浮点数
                    ssOneLine >> floatData.v[0];
                    ssOneLine >> floatData.v[1];
                    // 放到容器里面
                    texcoords.push_back(floatData);
//                    printf("texcoords: %f,%f\n", floatData.v[0], floatData.v[1]);
                } else if (szOneLine[1] == 'n') {
                    // 输出一个字符串(vn)到temp
                    ssOneLine >> temp;
                    FloatData floatData;
                    ssOneLine >> floatData.v[0];
                    ssOneLine >> floatData.v[1];
                    ssOneLine >> floatData.v[2];
                    normals.push_back(floatData);
//                    printf("normal: %f,%f,%f\n", floatData.v[0], floatData.v[1], floatData.v[2]);
                } else {
                    // 输出一个字符串(v)到temp
                    ssOneLine >> temp;
                    FloatData floatData;
                    ssOneLine >> floatData.v[0];
                    ssOneLine >> floatData.v[1];
                    ssOneLine >> floatData.v[2];
                    positions.push_back(floatData);
//                    printf("position: %f,%f,%f\n", floatData.v[0], floatData.v[1], floatData.v[2]);
                }
            } else if (szOneLine[0] == 'f') { // 绘制指令
                // 因为是三角形所以有 1/1/1 2/2/2 3/3/3 三个数据
                // 1/1/1 三个数字是索引，分别代表：从顶点数据的哪个地方取位置，从纹理数据的哪个地方取纹理，从法线数据的哪个地方取法线
                // f 1/1/1 2/2/2 3/3/3
                // f 3/3/3 2/2/2 4/4/4
                std::stringstream ssOneLine(szOneLine);
                ssOneLine >> temp;
                std::string vertexStr;
                for (int i = 0; i < 3; i++) {
                    // eg. 将 1/1/1 输出到vertexStr 由于中间是由空格隔开的
                    ssOneLine >> vertexStr;
                    // 按照/分割 解出 索引字符串
                    size_t pos = vertexStr.find_first_of('/');
                    std::string posIndexStr = vertexStr.substr(0, pos);
                    size_t pos2 = vertexStr.find_first_of('/', pos + 1);
                    std::string texcoordIndexStr = vertexStr.substr(pos + 1, pos2 - 1 - pos);
                    std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);

                    VertexDefine vd;
                    // 索引字符串 转int
                    vd.posIndex = atoi(posIndexStr.c_str());
                    vd.texcoordIndex = atoi(texcoordIndexStr.c_str());
                    vd.normalIndex = atoi(normalIndexStr.c_str());
                    vertexes.push_back(vd);
                }
//                printf("draw command : %s\n", szOneLine);
            }
        }
    }
    delete fileContent;

    // 索引个数
    int vertexCount = (int)vertexes.size();
    mVertexBuffer = new VertexBuffer;
    mVertexBuffer->SetSize(vertexCount);
    // 到此 我们从数据从定义描述 转换到了 opengl可以直接拿来绘制的点的定义
    for (int i = 0; i < vertexCount; ++i) {
        float *temp = positions[vertexes[i].posIndex - 1].v;
        mVertexBuffer->SetPosition(i, temp[0], temp[1], temp[2]);
        temp = texcoords[vertexes[i].texcoordIndex - 1].v;
        mVertexBuffer->SetTexcoord(i, temp[0], temp[1]);
        temp = normals[vertexes[i].normalIndex - 1].v;
        mVertexBuffer->SetNormal(i, temp[0], temp[1], temp[2]);
    }

    mShader = new Shader;
    mShader -> Init("Res/model_vertex.glsl", "Res/model_fragment.glsl");
    // 设置光源位置
    mShader -> SetVec4("U_LightPos", 0.0f, 1.0f, 1.0f, 0.0f);
    // 设置光的 环境光 分量 白色
    mShader -> SetVec4("U_LightAmbient", 1.0f, 1.0f, 1.0f, 1.0f);
    // 设置光的 漫反射 分量 白色
    mShader -> SetVec4("U_LightDiffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    // 设置光的 镜面反射 分量 白色
    mShader -> SetVec4("U_LightSpecular", 1.0f, 1.0f, 1.0f, 1.0f);

    // 摄像机的位置
    mShader -> SetVec4("U_CameraPos", 0.0f, 0.0f, 0.0f, 1.0f);
    // x: 对镜面反射光照强度取32次幂, 是光照非线性，满足正太分布
    // w: 控制纹理与光照混合的方式
    mShader -> SetVec4("U_LightOpt", 32.0f, 0.0f, 0.0f, 0.0f);

    SetAmbientMaterial(0.1f, 0.1f, 0.1f, 1.0f);
    SetDiffuseMaterial(0.6f, 0.6f, 0.6f, 1.0f);
    SetSpecularMaterial(1.0f, 1.0f, 1.0f, 1.0);
}

void Model::Draw(glm::mat4 &viewMatrix, glm::mat4 projectionMatrix, float x, float y, float z) {
    mShader -> SetVec4("U_CameraPos", x, y, z, 1.0);
    glEnable(GL_DEPTH_TEST);
    mVertexBuffer -> Bind();
    glm::mat4 it = glm::inverseTranspose(mModelMatrix);
    mShader -> Bind(glm::value_ptr(mModelMatrix), glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(mShader -> mProgram, "IT_ModelMatrix"), 1, GL_FALSE, glm::value_ptr(it));
    glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer -> mVertexCount);
    mVertexBuffer -> Unbind();
}

void Model::SetPosition(float x, float y, float z) {
    mModelMatrix = glm::translate(x, y, z);
}

void Model::SetAmbientMaterial(float r, float g, float b, float a) {
    // 设置模型表面对 环境光 的反射系数
    mShader -> SetVec4("U_AmbientMaterial", r, g, b, a);
}

void Model::SetDiffuseMaterial(float r, float g, float b, float a) {
    // 设置模型表面对 漫反射 的反射系数
    mShader -> SetVec4("U_DiffuseMaterial", r, g, b, a);
}

void Model::SetSpecularMaterial(float r, float g, float b, float a) {
    // 设置模型表面对 镜面反射 的反射系数
    mShader -> SetVec4("U_SpecularMaterial", r, g, b, a);
}

void Model::SetTexture(const char *imagePath) {
    mShader -> SetTexture("U_Texture", imagePath);
}