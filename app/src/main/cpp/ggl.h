#pragma once

#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <map>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <android/log.h>

#define printf(...) __android_log_print(ANDROID_LOG_DEBUG, "LY", __VA_ARGS__);
