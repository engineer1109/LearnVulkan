//
// Created by wjl on 2021/4/7.
//

#include "Texture2dCube.h"

int main(int argc, char** argv){
    VulkanEngine::Texture2dCube* engine = new VulkanEngine::Texture2dCube();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}
