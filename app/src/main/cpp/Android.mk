LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:=libmygles

#LOCAL_SRC_FILES:=opengl.cpp
FILE_LIST:=$(wildcard $(LOCAL_PATH)/*.cpp)
LOCAL_SRC_FILES:= $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS:=-llog -lGLESv2 -landroid
# os
include $(BUILD_SHARED_LIBRARY)


