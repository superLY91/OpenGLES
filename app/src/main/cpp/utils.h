#pragma once

#include "ggl.h"
unsigned char * LoadFileContent(const char * path, int & fileSize);
GLuint CompileShader(GLenum shaderType, const char * shaderCode);
GLuint CreateProgram(GLuint vsShader, GLuint fsShader);
float GetFrameTime();
unsigned char* DecodeBMP(unsigned char* bmpFileData, int& width, int& height);
// GLuint:返回生成的纹理对象的唯一标识符，通过这个唯一标识符使用这个纹理
// pixelData: 像素内存块的起始位置
// width height:图片宽高
// GLenum type:图片的像素类型 如 (r,g,b,a)或者(r,g,b)
GLuint CreateTexture2D(unsigned char*pixelData, int width, int height, GLenum type);
GLuint CreateTexture2DFromBMP(const char * bmpPath);
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data = nullptr);