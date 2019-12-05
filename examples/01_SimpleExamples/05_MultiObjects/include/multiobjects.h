/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* Multi render objects sample. Many cubes.
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef MULTIOBJECTS_H
#define MULTIOBJECTS_H
#ifdef WIN32
    #define gli glm
#endif
#include "vulkan_basicengine.h"
#include "vulkan_basicengine_texture.h"
class VkCube;

class MultiImageSampler:public VulkanBasicEngine{
public:
    MultiImageSampler(bool debugLayer);
    ~MultiImageSampler();

    virtual void prepare();
    virtual void render();
    virtual void draw();

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);
private:
    void createObjects();
    void setupVertexDescriptions();
    void setupDescriptorSetLayout();
    void preparePipelines();
    void setupDescriptorPool();
    void setupDescriptorSet();
    void buildCommandBuffers();
    void startAutoRotation();
public:
    struct Vertex {
        float pos[3];
        float uv[2];
        float normal[3];
    };
    struct {
        VkPipelineVertexInputStateCreateInfo inputState;
        std::vector<VkVertexInputBindingDescription> inputBinding;
        std::vector<VkVertexInputAttributeDescription> inputAttributes;
    } m_vertices;
private:
    VkDescriptorSet m_descriptorSet=VK_NULL_HANDLE;
    bool m_autoRotation=false;
    std::vector<VkCube*> m_vkCubeList;
};

#endif
