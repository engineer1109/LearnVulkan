#include "reflectsky.h"
#include "sky.h"
ReflectSky::ReflectSky(bool debugLayer):VulkanBasicEngine(debugLayer){
    title="LearnVulkan ReflectSky";
    settings.overlay=true;
}

ReflectSky::~ReflectSky(){

}

void ReflectSky::prepare(){
    VulkanBasicEngine::prepare();
    createObjects();
    prepared=true;
}

void ReflectSky::render(){
    if (!prepared)
        return;
    draw();

    if (frameCounter == 0)
    {
        vkDeviceWaitIdle(device);
    }
}

void ReflectSky::draw(){
    VulkanBasicEngine::prepareFrame();
    // Command buffer to be sumitted to the queue
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VulkanBasicEngine::submitFrame();
}

void ReflectSky::OnUpdateUIOverlay(vks::UIOverlay *overlay){

}

void ReflectSky::createObjects(){
    VulkanBaseObject::ObjectInfo objectinfo;
    objectinfo.vulkanDevice=vulkanDevice;
    objectinfo.instance=instance;
    objectinfo.cmdPool=cmdPool;
    objectinfo.pipelineLayout=m_pipelineLayout;
    objectinfo.pipelineCache=pipelineCache;
    objectinfo.renderPass=renderPass;
    objectinfo.queue=queue;
    objectinfo.screenWitdh=&width;
    objectinfo.screenHeight=&height;

    m_sky=new Sky();
    m_sky->setObjectInfo(objectinfo);
    m_sky->m_camera.rotation=&rotation;
    m_sky->create();
}
