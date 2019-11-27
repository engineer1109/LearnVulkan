#include "static_triangle.h"
int main(int argc,char** argv){
    bool debug=false;
    StaticTriangle* statictrianglePtr=new StaticTriangle(debug);
    statictrianglePtr->initVulkan();
    statictrianglePtr->initWindow();
    statictrianglePtr->prepare();
    statictrianglePtr->renderLoop();
    delete statictrianglePtr;
    return 1;
}
