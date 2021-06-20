//
// Created by wjl on 2021/6/20.
//

#include "AssimpModelSample.h"

int main(int argc, char** argv){
    VulkanEngine::AssimpModelSample* engine = new VulkanEngine::AssimpModelSample();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}

