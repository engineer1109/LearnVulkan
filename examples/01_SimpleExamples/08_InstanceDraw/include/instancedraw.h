/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef INSTANCEDRAW_H
#define INSTANCEDRAW_H
#include "vulkan_basicengine.h"
class InstanceCube;
class InstanceDraw:public VulkanBasicEngine{
public:
    InstanceDraw(bool debug=false);
    ~InstanceDraw();

    virtual void prepare();
    virtual void render();
    virtual void draw();

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);
    virtual void getEnabledFeatures();
private:
    void createObjects();
    void setupDescriptorPool();
    void setupDescriptorSetLayout();
    void setupDescriptorSet();
    void preparePipelines();
    void buildCommandBuffers();
private:
    VkPipelineLayout m_pipelineLayout=VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descriptorSetLayout=VK_NULL_HANDLE;

    InstanceCube* m_cube=nullptr;
};

#endif
