//
// Created by wjl on 2021/4/7.
//

#include "StaticCube.h"

int main(int argc, char** argv){
    VulkanEngine::StaticCube* engine = new VulkanEngine::StaticCube();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}
