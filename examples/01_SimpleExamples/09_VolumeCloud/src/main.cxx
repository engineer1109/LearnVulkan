/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "volumecloud.h"
int main(int argc,char** argv){
    bool debug=false;
    VolumeCloud* pVulkan=new VolumeCloud(debug);
    pVulkan->initVulkan();
    pVulkan->initWindow();
    pVulkan->prepare();
    pVulkan->renderLoop();
    delete pVulkan;
    pVulkan=nullptr;
    return 1;
}
