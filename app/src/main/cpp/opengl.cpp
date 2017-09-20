#include "ggl.h"
#include "scene.h"
#include "utils.h"
#include <sys/time.h>
AAssetManager *sAssetManager = nullptr;
unsigned char * LoadFileContent(const char * path, int & fileSize) {
    unsigned char * fileContent = nullptr;
    fileSize = 0;
    AAsset * asset = AAssetManager_open(sAssetManager, path, AASSET_MODE_UNKNOWN);
    if (asset == nullptr) {
        return nullptr;
    }
    fileSize = AAsset_getLength(asset);
    fileContent = new unsigned char[fileSize + 1];
    AAsset_read(asset, fileContent, fileSize);
    fileContent[fileSize] = '\0';
    AAsset_close(asset);
    return fileContent;
}

/**
 * 计算每一帧消耗的时间
 * @return
 */
float GetFrameTime() {
    // lastTime：上次的时间 timeCurrent：这次的时间
    static unsigned long long lastTime = 0, timeCurrent = 0;
    timeval current;
    // 获取当前的时间
    gettimeofday(&current, nullptr);
    // current.tv_sec * 1000: 秒转化为毫秒
    //  current.tv_usec / 1000: 微秒转化为毫秒
    timeCurrent = current.tv_sec * 1000 + current.tv_usec / 1000;
    unsigned long long frameTime = lastTime == 0 ? 0 : timeCurrent - lastTime;
    lastTime = timeCurrent;
    // 毫秒 -> 秒
    return float(frameTime) / 1000.0f;
}

extern "C" JNIEXPORT void Java_com_liyang_opengles_Native_InitAssetManager(JNIEnv * env, jobject obj, jobject assetManager) {
    sAssetManager = AAssetManager_fromJava(env, assetManager);
}

extern "C" JNIEXPORT void Java_com_liyang_opengles_Native_InitOpenGL(JNIEnv * env, jobject obj) {
    Init();
}

extern "C" JNIEXPORT void Java_com_liyang_opengles_Native_OnViewportChanged(JNIEnv * env, jobject obj, jfloat width, jfloat height) {
    SetViewPortSize(width, height);
}

extern "C" JNIEXPORT void Java_com_liyang_opengles_Native_RenderOneFrame(JNIEnv * env, jobject obj) {
    Draw();
}