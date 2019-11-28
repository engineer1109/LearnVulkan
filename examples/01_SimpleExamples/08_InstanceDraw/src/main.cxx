#include "instancedraw.h"
int main(int argc,char** argv){
    bool debug=false;
    InstanceDraw* pVulkan=new InstanceDraw(debug);
    pVulkan->initVulkan();
    pVulkan->initWindow();
    pVulkan->prepare();
    pVulkan->renderLoop();
    delete pVulkan;
    pVulkan=nullptr;
    return 1;
}
