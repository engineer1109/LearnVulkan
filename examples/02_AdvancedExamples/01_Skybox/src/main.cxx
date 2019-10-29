#include "skybox.h"
int main(int argc,char** argv){
    bool debug=false;
    Skybox* skyboxPtr=new Skybox(debug);
    skyboxPtr->initVulkan();
    skyboxPtr->initWindow();
    skyboxPtr->prepare();
    skyboxPtr->renderLoop();
    delete skyboxPtr;
    return 1;
}
