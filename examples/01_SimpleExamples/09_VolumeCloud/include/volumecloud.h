/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef VOLUMECLOUD_H
#define VOLUMECLOUD_H
#include "vulkan_basicengine.h"
class Texture3dCloud;
class VolumeCloud:public VulkanBasicEngine{
public:
    VolumeCloud(bool debug=false);
    ~VolumeCloud();

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
    Texture3dCloud* m_volumeCloud=nullptr;
};

#endif
