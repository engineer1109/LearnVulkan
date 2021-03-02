#include <jni.h>
#include <string>

#include "VulkanBase.h"
#include "StaticTriangle.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_createInstance(JNIEnv *env, jobject thiz,
                                                                   jint id) {
    auto *engine = new VulkanEngine::StaticTriangle();
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

    assert(window);

    if (instance) {
        VulkanEngine::VulkanBase *engine = (VulkanEngine::VulkanBase *) instance;
        LOGI("New Surface %ld", window);
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
            VulkanEngine::StaticTriangle *engine = reinterpret_cast<VulkanEngine::StaticTriangle *>(instance);
            delete (engine);
//        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_destroySurface(JNIEnv *env, jobject thiz,
                                                                   jlong instance) {
    // TODO: implement destroySurface()
    if(instance){
        auto *engine = reinterpret_cast<VulkanEngine::VulkanBaseEngine *>(instance);
        engine->destroySurface();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_rebuildSurface(JNIEnv *env, jobject thiz,
                                                                   jlong instance) {
    // TODO: implement rebuildSurface()
    if(instance){
        auto *engine = reinterpret_cast<VulkanEngine::VulkanBaseEngine *>(instance);
        engine->rebuildSurface();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_pause(JNIEnv *env, jobject thiz,
                                                          jlong instance) {
    // TODO: implement pause()
    if(instance){
        auto *engine = reinterpret_cast<VulkanEngine::VulkanBaseEngine *>(instance);
        engine->pause();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_resume(JNIEnv *env, jobject thiz,
                                                           jlong instance) {
    // TODO: implement resume()
    if(instance){
        auto *engine = reinterpret_cast<VulkanEngine::VulkanBaseEngine *>(instance);
        engine->resume();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_waitCurrentFrameComplete(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jlong instance) {
    // TODO: implement waitCurrentFrameComplete()
    if(instance){
        auto *engine = reinterpret_cast<VulkanEngine::VulkanBaseEngine *>(instance);
        engine->waitForCurrentFrameComplete();
        LOGI("waitForCurrentFrameComplete");
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_destroyANativeWindow(JNIEnv *env, jobject thiz,
                                                                         jlong instance) {
    // TODO: implement destroyANativeWindow()
    if(instance){
        auto *engine = reinterpret_cast<VulkanEngine::VulkanBaseEngine *>(instance);
        engine->destroyANativeWindow();
        LOGI("destroyANativeWindow");
    }
}