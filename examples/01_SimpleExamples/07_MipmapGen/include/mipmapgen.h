/*
* LearnVulkan Examples
*
* Copyright (C) by Jialiang Wang - https://github.com/engineer1109/LearnVulkan
* 1292846099@qq.com
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef MIPMAPGEN_H
#define MIPMAPGEN_H
#include "vulkan_basicengine.h"
class CubeTunnel;

class MipmapGen:public VulkanBasicEngine{
public:
    MipmapGen(bool debug=false);
    ~MipmapGen();

    virtual void prepare();
    virtual void render();
    virtual void draw();

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);
private:
    void createObjects();
    void setupVertexDescriptions();
    void setupDescriptorPool();
    void setupDescriptorSetLayout();
    void setupDescriptorSet();
    void preparePipelines();
    void buildCommandBuffers();
private:
    struct {
        VkPipelineVertexInputStateCreateInfo inputState;
        std::vector<VkVertexInputBindingDescription> bindingDescriptions;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    } m_vertices;
    struct Vertex {
        float pos[3];
        float uv[2];
        float normal[3];
    };
private:
    CubeTunnel* m_cubeTunnel=nullptr;
};
#endif
