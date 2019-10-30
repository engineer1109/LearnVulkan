#include "reflectsky.h"
int main(int argc,char** argv){
    bool debug=false;
    ReflectSky* reflectSkyPtr=new ReflectSky(debug);
    reflectSkyPtr->initVulkan();
    reflectSkyPtr->initWindow();
    reflectSkyPtr->prepare();
    reflectSkyPtr->renderLoop();
    delete reflectSkyPtr;
    return 1;
}
