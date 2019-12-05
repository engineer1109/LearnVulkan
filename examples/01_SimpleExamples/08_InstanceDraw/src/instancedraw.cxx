/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "instancecube.h"
#include "instancedraw.h"
#define VERTEX_BUFFER_BIND_ID 0
#define INSTANCE_BUFFER_BIND_ID 1
InstanceDraw::InstanceDraw(bool debug):VulkanBasicEngine (debug){
    title="InstanceDraw";
    settings.overlay = true;
}

InstanceDraw::~InstanceDraw(){
    if(m_cube!=nullptr){
        delete m_cube;
        m_cube=nullptr;
    }
    vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
}

void InstanceDraw::prepare(){
    VulkanBasicEngine::prepare();
    createObjects();
    prepared=true;
}

void InstanceDraw::render(){
    if (!prepared)
        return;
    draw();

    if (frameCounter == 0)
    {
        vkDeviceWaitIdle(device);
    }
}

void InstanceDraw::draw(){
    VulkanBasicEngine::prepareFrame();
    // Command buffer to be sumitted to the queue
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &drawCmdBuffers[currentBuffer];
    VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
    VulkanBasicEngine::submitFrame();
}

void InstanceDraw::OnUpdateUIOverlay(vks::UIOverlay *overlay){

}

void InstanceDraw::getEnabledFeatures()
{
    // Enable anisotropic filtering if supported
    if (deviceFeatures.samplerAnisotropy) {
        enabledFeatures.samplerAnisotropy = VK_TRUE;
    }
}

void InstanceDraw::createObjects(){
    VulkanTemplate::VulkanBaseObject::ObjectInfo objectinfo;
    objectinfo.vulkanDevice=vulkanDevice;
    objectinfo.instance=instance;
    objectinfo.cmdPool=cmdPool;
    objectinfo.pipelineCache=pipelineCache;
    objectinfo.renderPass=renderPass;
    objectinfo.queue=queue;
    objectinfo.screenWitdh=&width;
    objectinfo.screenHeight=&height;

    m_cube=new InstanceCube();
    m_cube->setObjectInfo(objectinfo);
    m_cube->create();
}
