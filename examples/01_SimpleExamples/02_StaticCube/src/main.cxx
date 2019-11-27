#include "static_cube.h"
int main(int argc,char** argv){
    bool debug=false;
    StaticCube* staticCubePtr=new StaticCube(debug);
    staticCubePtr->initVulkan();
    staticCubePtr->initWindow();
    staticCubePtr->prepare();
    staticCubePtr->renderLoop();
    delete staticCubePtr;
    return 1;
}
