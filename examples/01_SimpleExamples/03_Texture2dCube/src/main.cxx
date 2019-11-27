#include "texture2d_cube.h"
int main(int argc,char** argv){
    bool debug=false;
    Texture2dCube* texture2dCubePtr=new Texture2dCube(debug);
    texture2dCubePtr->initVulkan();
    texture2dCubePtr->initWindow();
    texture2dCubePtr->prepare();
    texture2dCubePtr->renderLoop();
    delete texture2dCubePtr;
    return 1;
}
