//
// Created by wjl on 2021/4/7.
//

#include "SkyboxCube.h"

int main(int argc, char** argv){
    VulkanEngine::SkyboxCube* engine = new VulkanEngine::SkyboxCube();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}
