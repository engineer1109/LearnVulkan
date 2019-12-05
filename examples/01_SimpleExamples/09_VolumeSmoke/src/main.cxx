/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "volumesmoke.h"
int main(int argc,char** argv){
    bool debug=false;
    VolumeSmoke* pVulkan=new VolumeSmoke(debug);
    pVulkan->initVulkan();
    pVulkan->initWindow();
    pVulkan->prepare();
    pVulkan->renderLoop();
    delete pVulkan;
    pVulkan=nullptr;
    return 1;
}
