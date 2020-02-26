#include "planarmirror.h"
int main(int argc, char **argv) {
    bool debug = false;
    PlanarMirror *pVulkan = new PlanarMirror(debug);
    pVulkan->initVulkan();
    pVulkan->initWindow();
    pVulkan->prepare();
    pVulkan->renderLoop();
    delete pVulkan;
    pVulkan = nullptr;
    return 1;
}
