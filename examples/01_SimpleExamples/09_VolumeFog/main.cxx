//
// Created by wjl on 2023/1/29.
//

#include "VolumeFog.h"

int main(int argc, char** argv){
    VulkanEngine::VolumeFog* engine = new VulkanEngine::VolumeFog();
    engine->initWindow();
    engine->initVulkan();
    engine->prepare();
    engine->renderLoop();
    return 1;
}
