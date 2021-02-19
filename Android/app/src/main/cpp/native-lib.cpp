#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jlong JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_createInstance(JNIEnv *env, jobject thiz,
                                                                    jint id) {

}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_initVulkan(JNIEnv *env, jobject thiz,
                                                               jlong instance) {
    // TODO: implement initVulkan()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_renderFrame(JNIEnv *env, jobject thiz,
                                                                jlong instance) {
    // TODO: implement renderFrame()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setSurface(JNIEnv *env, jobject thiz,
                                                               jlong instance, jobject surface) {
    // TODO: implement setSurface()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setAssetManager(JNIEnv *env, jobject thiz,
                                                                    jlong instance,
                                                                    jobject asset_manager) {
    // TODO: implement setAssetManager()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setTouchPos(JNIEnv *env, jobject thiz,
                                                                jlong instance, jfloat x,
                                                                jfloat y) {
    // TODO: implement setTouchPos()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setTouchPosSecond(JNIEnv *env, jobject thiz,
                                                                      jlong instance, jfloat x,
                                                                      jfloat y) {
    // TODO: implement setTouchPosSecond()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_setTouchMode(JNIEnv *env, jobject thiz,
                                                                 jlong instance, jint mode) {
    // TODO: implement setTouchMode()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_resetTouch(JNIEnv *env, jobject thiz,
                                                               jlong instance) {
    // TODO: implement resetTouch()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_engineer1109_learnvulkan_render_BaseRender_deleteVulkan(JNIEnv *env, jobject thiz,
                                                                 jlong instance, jint id) {
    // TODO: implement deleteVulkan()
}