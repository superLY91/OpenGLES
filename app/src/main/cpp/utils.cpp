#include "utils.h"
/**
 *
 * @param shaderType VertexShader or FragmentShader
 * @param shaderCode shader的代码
 * @return
 */
GLuint CompileShader(GLenum shaderType, const char * shaderCode) {
    // 创建shader对象 标识符
    GLuint shader = glCreateShader(shaderType);
    // shader: shader对象
    // 1: 代表有多少几句shader代码，因为将shader的代码都存在了一个shaderCode中，所以传1
    // &shaderCode: shader的代码
    // nullptr: 代表每一句shader代码的长度，一句传nullptr，多句传一个数字分布对应每一句的长度
    glShaderSource(shader, 1, &shaderCode, nullptr);
    // 显卡驱动对shader编译
    glCompileShader(shader);

    // 编译结果
    GLint compileResult = GL_TRUE;
    // shader: 获得那一个shader的编译状态
    // GL_COMPILE_STATUS: 获取编译状态
    // compileResult: 编译结果
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (compileResult == GL_FALSE) {
        // 1024大小的缓冲区
        char szLog[1024] = { 0 };
        // 存储实际的错误日志有多长
        GLsizei logLen = 0;
        // 1024: 错误日志最多可以存储多少个字符，
        // &logLen: logLen的地址，最终错误日志的长度会写入logLen中
        glGetShaderInfoLog(shader, 1024, &logLen, szLog);
        printf("Compile Shader fail error log : %s \nshader code: \n%s\n", szLog, shaderCode);
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

/**
 * program将VertexShader和FragmentShader连接在一起，program是用来画图的程序，跑在GPU上
 * @param vsShader 编译后的VertexShader
 * @param fsShader 编译后的FragmentShader
 * @return
 */
GLuint CreateProgram(GLuint vsShader, GLuint fsShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vsShader);
    glAttachShader(program, fsShader);
    glLinkProgram(program);
    glDetachShader(program, vsShader);
    glDetachShader(program, fsShader);
    // link结果
    GLint nResult=0;
    glGetProgramiv(program, GL_LINK_STATUS, &nResult);
    if (nResult == GL_FALSE) {
        char log[1024] = { 0 };
        GLsizei writed = 0;
        glGetProgramInfoLog(program, 1024, &writed, log);
        printf("create gpu program fail, link error : %s\n", log);
        glDeleteProgram(program);
        program = 0;
    }
    return program;
}

unsigned char* DecodeBMP(unsigned char* bmpFileData, int& width, int& height) {
    // 判断是否是位图，文件内存块首部取出一个unsigned short判断是否等于0x4D42
    if (0x4D42 == *((unsigned short*)bmpFileData)) {
        // 取像素内存块在整个文件中的偏移，文件内存块偏移10个字节的地方取出一个整形
        int pixelDataOffset = *((int*)(bmpFileData + 10));
        // 取位图的宽
        width = *((int*)(bmpFileData + 18));
        // 取位图的高
        height = *((int*)(bmpFileData + 22));
        // 像素内存块的起始位置
        unsigned char*pixelData = bmpFileData + pixelDataOffset;
        // 对位图进行转码 因为位图是(b,g,r) 转为opengl可以识别的(r,g,b)
        for (int i = 0; i < width*height * 3; i += 3) {
            unsigned char temp = pixelData[i];
            pixelData[i] = pixelData[i + 2];
            pixelData[i + 2] = temp;
        }
        // 返回像素内存块的起始位置
        return pixelData;
    }
    return nullptr;
}

void setTexParamGL_CLAMP() {
    // 设置纹理坐标过滤参数：当我们设置的几何形体超过1.0的时候，应该怎么采集纹理，
    // GL_TEXTURE_WRAP_S GL_TEXTURE_WRAP_T UV坐标
    // GL_CLAMP 超过1.0的部分，纹理坐标的像素数据取纹理的边界的像素(纹理坐标等于1.0的地方)
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // 解决天空盒接缝黑线问题
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setTexParamGL_REPEAT() {
    // 设置纹理坐标过滤参数：当我们设置的几何形体超过1.0的时候，应该怎么采集纹理，
    // GL_CLAMP 超过1.0的部分，取纹理坐标减1.0位置的像素
    // 例如 超过部分纹理坐标1.1取的纹理为 1.1 - 1 = 0.1 的像素
    // 例如 超过部分纹理坐标1.2取的纹理为 1.2 - 1 = 0.2 的像素
    // 例如 超过部分纹理坐标2.1取的纹理为 2.1 - 1 = 1.1 的像素 也就等于取0.1的像素
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void setTexParamGL_LINEAR() {
    // GL_LINEAR 使用线性过滤 纹理更加平滑
    // 在给多边形某一点进行着色的时候，他的颜色，是根据周围的一些点，进行莫一种线性的差值计算出来的

    // GL_TEXTURE_MAG_FILTER 当纹理被放大的时候，应该使用怎样的算法采集像素对像素块着色
    // 比如：一个较小的纹理(168*168) 贴在一个较大的像素块(256*256)时，怎样对多边形进行着色
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // GL_TEXTURE_MIN_FILTER 当纹理被缩小的时候，应该使用怎样的算法采集像素对像素块着色
    // 比如：一个较大的纹理(256*256) 贴在一个较小的像素块(168*168)时，怎样对多边形进行着色
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void setTexParamGL_NEAREST() {
    // GL_NEAREST
    // 在给多边形某一点进行着色的时候，他的颜色，取里当前点最近的点的颜色，因为像素点是离散的

    // GL_TEXTURE_MAG_FILTER 当纹理被放大的时候，应该使用怎样的算法采集像素对像素块着色
    // 比如：一个较小的纹理(168*168) 贴在一个较大的像素块(256*256)时，怎样对多边形进行着色
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // GL_TEXTURE_MIN_FILTER 当纹理被缩小的时候，应该使用怎样的算法采集像素对像素块着色
    // 比如：一个较大的纹理(256*256) 贴在一个较小的像素块(168*168)时，怎样对多边形进行着色
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

GLuint CreateTexture2D(unsigned char*pixelData, int width, int height, GLenum type) {
    GLuint texture;
    // 1:表示申请一个纹理对象
    // texture: 纹理对象的唯一标识
    glGenTextures(1, &texture);
    // 设置texture为当前的纹理对象,后面所有对纹理的操作都会应用到当前的纹理对象上
    // GL_TEXTURE_2D 表示设置的是2D纹理
    glBindTexture(GL_TEXTURE_2D, texture);

    setTexParamGL_LINEAR();
//    setTexParamGL_NEAREST();

    setTexParamGL_CLAMP();
//    setTexParamGL_REPEAT();

    // 将像素数据上传到显卡上，内存上传到显存
    // GL_TEXTURE_2D: 2D纹理
    // 0: BitmapLevel级别 太占内存
    // type: internalformat 纹理数据在显卡上是什么样的格式
    // width height 像素数据宽高
    // border:  必须为0
    // type: format 纹理数据在内存上是什么样的格式
    // GL_UNSIGNED_BYTE: 在像素数据中每一个分量是一个什么类型
    // pixelData 像素数据 将这个数据拷贝到显卡上去
    glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixelData);
    // 将当前纹理设置为0号纹理，避免后续操作对此纹理进行修改
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

GLuint CreateTexture2DFromBMP(const char * bmpPath) {
    // 加载bmp文件加载到内存中来
    int nFileSize = 0;
    unsigned char * bmpFileContent = LoadFileContent(bmpPath, nFileSize);
    if (bmpFileContent == nullptr) {
        return 0; // 加载bmp文件失败返回opengl默认的纹理单元 0号纹理单元（一般为黑色）
    }
    // 解码bmp文件
    int bmpWidth = 0; int bmpHeight = 0;
    unsigned char *pixelData = DecodeBMP(bmpFileContent, bmpWidth, bmpHeight);
    if (bmpWidth==0) {
        delete bmpFileContent;
        return 0; // 解码失败
    }

    // 使用解码数据，创建opengl中的纹理对象 GL_RGB: bmp文件没有alpha通道
    GLuint texture = CreateTexture2D(pixelData, bmpWidth, bmpHeight, GL_RGB);
    delete bmpFileContent;
    return texture;
}

/**
 * 创建BufferObject  vbo or ebo
 * @param bufferType ARRAY_BUFFER(vbo), ELEMENT_ARRAY_BUFFER(ebo)
 * @param size buffer大小
 * @param usage GL_STATIC_DRAW, GL_DYNAMIC_DRAW
 * @param data 具体数据
 * @return
 */
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data) {
    GLuint object;
    glGenBuffers(1, &object);
    glBindBuffer(bufferType, object);
    glBufferData(bufferType, size, data, usage);
    glBindBuffer(bufferType, 0);
    return object;
}

// size 代表 长宽都是相等的大小size, halfSize = size / 2
// 纹理方块中心点的坐标为（halfSize, halfSize）
GLuint CreateProcedureTexture(int size) {
    // rgba 每个像素占用一个unsigned char，rgba分别占用一个unsigned char
    unsigned char *imageData = new unsigned char[size*size * 4];
    float halfSize = (float) size / 2.0f;
    // 方块上离中心点最园的距离，对角线的一半
    float maxDistance = sqrtf(halfSize*halfSize + halfSize*halfSize);
    float centerX = halfSize;
    float centerY = halfSize;
    for(int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int currentPixelOffset = (x + y * size) * 4;
            imageData[currentPixelOffset] = 255;
            imageData[currentPixelOffset + 1] = 255;
            imageData[currentPixelOffset + 2] = 255;
            float deltaX = (float)x - centerX;
            float deltaY = (float)y - centerY;
            // 当前正在遍历的点距离中心的距离
            float distance = sqrtf(deltaX*deltaX + deltaY*deltaY);
            // 离中心点越远 alpha越小 线性
            // float alpha = 1.0f - distance / maxDistance;
            // 非线性 取幂 正太分布 8次幂
            float alpha = powf(1.0f - distance / maxDistance, 8.0f);
            alpha = alpha > 1.0f ? 1.0f : alpha;
            imageData[currentPixelOffset + 3] = (unsigned char)(alpha * 255);
        }
    }
    GLuint texture = CreateTexture2D(imageData,size,size,GL_RGBA);
    delete imageData;
    return texture;
}