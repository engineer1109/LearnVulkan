//
// Created by wjl on 2021/4/7.
//

#include "PhongLighting.h"

int main(int argc, char** argv){
    VulkanEngine::PhongLighting* engine = new VulkanEngine::PhongLighting();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}
