/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef REFLECTSKY_H
#define REFLECTSKY_H
#include "VulkanModel.hpp"
#include "vulkan_basicengine.h"
#include "vulkan_basicengine_texture.h"
class Sky;
class ReflectObject;

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
    void setupDescriptorPool();
    void setupDescriptorSetLayout();
    void setupDescriptorSets();
    void preparePipelines();
    void buildCommandBuffers();
private:
    struct{
        VkDescriptorSet sky;
        VkDescriptorSet reflectObject;
    }m_descriptorSets;
    vks::VertexLayout m_vertexLayout = vks::VertexLayout({
        vks::VERTEX_COMPONENT_POSITION,
        vks::VERTEX_COMPONENT_NORMAL,
        vks::VERTEX_COMPONENT_UV,
    });
    Sky* m_sky=nullptr;
    ReflectObject* m_reflectObject=nullptr;
};

#endif
