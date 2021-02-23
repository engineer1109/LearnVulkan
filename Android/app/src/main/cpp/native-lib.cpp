#include <jni.h>
#include <string>

#include "VulkanBase.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_createInstance(JNIEnv *env, jobject thiz,
                                                                   jint id) {
    VulkanEngine::VulkanBase *engine = new VulkanEngine::VulkanBase();
    return (jlong) engine;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_initVulkan(JNIEnv *env, jobject thiz,
                                                               jlong instance) {
    // TODO: implement initVulkan()
    if (instance){
        VulkanEngine::VulkanBase *engine = reinterpret_cast<VulkanEngine::VulkanBase*>(instance);
        engine->initVulkan();
        engine->prepare();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_renderFrame(JNIEnv *env, jobject thiz,
                                                                jlong instance) {
    // TODO: implement renderFrame()
    if (instance){
        VulkanEngine::VulkanBase *engine = reinterpret_cast<VulkanEngine::VulkanBase*>(instance);
        engine->renderFrame();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setSurface(JNIEnv *env, jobject thiz,
                                                               jlong instance, jobject surface) {
    // TODO: implement setSurface()
    auto window = ANativeWindow_fromSurface(env, surface);

    if (instance) {
        VulkanEngine::VulkanBase *engine = (VulkanEngine::VulkanBase *) instance;
        engine->setWindow(window);
        uint32_t width = static_cast<uint32_t >(ANativeWindow_getWidth(window));
        uint32_t height = static_cast<uint32_t >(ANativeWindow_getHeight(window));
        engine->setWidth(width);
        engine->setHeight(height);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setAssetManager(JNIEnv *env, jobject thiz,
                                                                    jlong instance,
                                                                    jobject asset_manager) {
    // TODO: implement setAssetManager()
    auto asset = AAssetManager_fromJava(env, asset_manager);
    if (instance) {
        VulkanEngine::VulkanBase *engine = reinterpret_cast<VulkanEngine::VulkanBase *>(instance);
        engine->setAssetManager(asset);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setTouchPos(JNIEnv *env, jobject thiz,
                                                                jlong instance, jfloat x,
                                                                jfloat y) {
    // TODO: implement setTouchPos()
    if (instance) {
        VulkanEngine::VulkanBase *engine = reinterpret_cast<VulkanEngine::VulkanBase *>(instance);
        engine->setTouchPos(x, y);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setTouchPosSecond(JNIEnv *env, jobject thiz,
                                                                      jlong instance, jfloat x,
                                                                      jfloat y) {
    // TODO: implement setTouchPosSecond()
    if (instance) {
        VulkanEngine::VulkanBase *engine = reinterpret_cast<VulkanEngine::VulkanBase *>(instance);
        engine->setTouchPosSecond(x, y);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setTouchMode(JNIEnv *env, jobject thiz,
                                                                 jlong instance, jint mode) {
    // TODO: implement setTouchMode()
    if (instance) {
        VulkanEngine::VulkanBase *engine = reinterpret_cast<VulkanEngine::VulkanBase *>(instance);
        engine->setTouchMode(VulkanEngine::VulkanBase::TouchMode(mode));
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_resetTouch(JNIEnv *env, jobject thiz,
                                                               jlong instance) {
    // TODO: implement resetTouch()
    if (instance) {
        VulkanEngine::VulkanBase *engine = reinterpret_cast<VulkanEngine::VulkanBase *>(instance);
        engine->resetTouch();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_deleteVulkan(JNIEnv *env, jobject thiz,
                                                                 jlong instance, jint id) {
    // TODO: implement deleteVulkan()
    if (instance) {
//        if (id == 0) {
//        } else if (id == 1) {
//        }
//        else if (id == 2) {
//        }
//        else {
            VulkanEngine::VulkanBase *engine = reinterpret_cast<VulkanEngine::VulkanBase *>(instance);
            delete (engine);
//        }
    }
}