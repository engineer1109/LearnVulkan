#include "texture3dsmoke.h"
#include "volumesmoke.h"
VolumeSmoke::VolumeSmoke(bool debug):VulkanBasicEngine (debug){
    title="InstanceDraw";
    settings.overlay = true;
}

VolumeSmoke::~VolumeSmoke(){
    if(m_cube!=nullptr){
        delete m_cube;
        m_cube=nullptr;
    }
}

void VolumeSmoke::prepare(){
    VulkanBasicEngine::prepare();
    createObjects();
    prepared=true;
}

void VolumeSmoke::render(){
    if (!prepared)
        return;
    draw();

    if (frameCounter == 0)
    {
        vkDeviceWaitIdle(device);
    }
}

void VolumeSmoke::draw(){
    VulkanBasicEngine::prepareFrame();
    // Command buffer to be sumitted to the queue
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VulkanBasicEngine::submitFrame();
}

void VolumeSmoke::OnUpdateUIOverlay(vks::UIOverlay *overlay){

}

void VolumeSmoke::getEnabledFeatures()
{
    // Enable anisotropic filtering if supported
    if (deviceFeatures.samplerAnisotropy) {
        enabledFeatures.samplerAnisotropy = VK_TRUE;
    }
}

void VolumeSmoke::createObjects(){
    VulkanBaseObject::ObjectInfo objectinfo;
    objectinfo.vulkanDevice=vulkanDevice;
    objectinfo.instance=instance;
    objectinfo.cmdPool=cmdPool;
    objectinfo.pipelineCache=pipelineCache;
    objectinfo.renderPass=renderPass;
    objectinfo.queue=queue;
    objectinfo.screenWitdh=&width;
    objectinfo.screenHeight=&height;

    m_cube=new Texture3dSmoke();
    m_cube->setObjectInfo(objectinfo);
    m_cube->create();
}
