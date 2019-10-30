#ifndef REFLECTSKY_H
#define REFLECTSKY_H
#include "VulkanModel.hpp"
#include "vulkan_basicengine.h"
#include "vulkan_basicengine_texture.h"
class Sky;

class ReflectSky:public VulkanBasicEngine{
public:
    ReflectSky(bool debugLayer=false);
    ~ReflectSky();

    virtual void prepare();
    virtual void render();
    virtual void draw();

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);
private:
    void createObjects();
private:
    VkPipelineLayout m_pipelineLayout=VK_NULL_HANDLE;
    VkDescriptorSet m_descriptorSet=VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descriptorSetLayout=VK_NULL_HANDLE;
    Sky* m_sky=nullptr;
};

#endif
