#include "multiobjects.h"
int main(int argc,char** argv){
    bool debug=false;
    MultiImageSampler* pMultiImageSampler=new MultiImageSampler(debug);
    pMultiImageSampler->initVulkan();
    pMultiImageSampler->initWindow();
    pMultiImageSampler->prepare();
    pMultiImageSampler->renderLoop();
    delete pMultiImageSampler;
    return 1;
}
