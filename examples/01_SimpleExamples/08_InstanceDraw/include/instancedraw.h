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
private:
    VkPipelineLayout m_pipelineLayout=VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descriptorSetLayout=VK_NULL_HANDLE;

    InstanceCube* m_cube=nullptr;
};

#endif
