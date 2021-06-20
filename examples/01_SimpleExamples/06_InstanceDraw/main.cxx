//
// Created by wjl on 2021/4/7.
//

#include "InstanceDraw.h"

int main(int argc, char** argv){
    VulkanEngine::InstanceDraw* engine = new VulkanEngine::InstanceDraw();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}
