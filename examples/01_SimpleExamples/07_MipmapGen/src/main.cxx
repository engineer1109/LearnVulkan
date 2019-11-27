#include "mipmapgen.h"
int main(int argc,char** argv){
    bool debug=false;
    MipmapGen* pVulkan=new MipmapGen(debug);
    pVulkan->initVulkan();
    pVulkan->initWindow();
    pVulkan->prepare();
    pVulkan->renderLoop();
    delete pVulkan;
    pVulkan=nullptr;
    return 1;
}
