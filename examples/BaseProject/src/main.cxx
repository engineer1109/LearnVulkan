/*
 * LearnVulkan Examples
 *
 * This is the main function of the base project with native window vulkan application.
 *
 * Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "baseproject.h"
int main(int argc, char **argv) {
    bool debug = false;
    BaseProject *pVulkan = new BaseProject(debug); // new an engine
    pVulkan->initVulkan();                         // init vulkan
    pVulkan->initWindow();                         // init native window
    pVulkan->prepare();                            // prepare render pipeline
    pVulkan->renderLoop();                         // start render loop until it stops
    delete pVulkan;                                // delete engine
    pVulkan = nullptr;
    return 1;
}
