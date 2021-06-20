//
// Created by wjl on 2021/4/7.
//

#include "ShadowMapping.h"

int main(int argc, char** argv){
    VulkanEngine::ShadowMapping* engine = new VulkanEngine::ShadowMapping();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}
