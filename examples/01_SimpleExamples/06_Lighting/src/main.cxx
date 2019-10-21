#include "lighting.h"
int main(int argc,char** argv){
    bool debug=false;
    Lighting* pVulkan=new Lighting(debug);
    pVulkan->initVulkan();
    pVulkan->initWindow();
    pVulkan->prepare();
    pVulkan->renderLoop();
    delete pVulkan;
    return 1;
}
