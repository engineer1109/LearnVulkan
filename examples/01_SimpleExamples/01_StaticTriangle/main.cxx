//
// Created by wjl on 2021/4/7.
//

#include "StaticTriangle.h"

int main(int argc, char** argv){
    VulkanEngine::StaticTriangle* engine = new VulkanEngine::StaticTriangle();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}
