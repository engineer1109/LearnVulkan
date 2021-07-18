//
// Created by wjl on 2021/4/7.
//

#include "ShadowMappingOnmi.h"

int main(int argc, char** argv){
    VulkanEngine::ShadowMappingOnmi* engine = new VulkanEngine::ShadowMappingOnmi();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}
