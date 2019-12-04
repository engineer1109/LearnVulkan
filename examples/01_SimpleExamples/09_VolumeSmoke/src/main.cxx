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
