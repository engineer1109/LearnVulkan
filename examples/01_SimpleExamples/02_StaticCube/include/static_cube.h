#ifndef STATIC_TRIANGLE_H
#define STATIC_TRIANGLE
#include "vulkan_basicengine.h"
class StaticCube:public VulkanBasicEngine{
public:
    StaticCube(bool debugLayer);
    ~StaticCube();

    virtual void prepare();
    virtual void render();
    virtual void draw();

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);
private:
    void generateVertex();
    void setupVertexDescriptions();
    void prepareUniformBuffers();
    void updateUniformBuffers(bool viewchanged=false);
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
    struct UboVS {
        glm::mat4 projection;
        glm::mat4 model;
    } m_uboVS;
    struct {
        VkPipelineVertexInputStateCreateInfo inputState;
        std::vector<VkVertexInputBindingDescription> inputBinding;
        std::vector<VkVertexInputAttributeDescription> inputAttributes;
    } m_vertices;
private:
    vks::Buffer m_vertexBuffer;
    vks::Buffer m_indexBuffer;
    vks::Buffer m_uniformBufferVS;
    VkPipelineLayout m_pipelineLayout=VK_NULL_HANDLE;
    VkDescriptorSet m_descriptorSet=VK_NULL_HANDLE;
    VkDescriptorSetLayout m_descriptorSetLayout=VK_NULL_HANDLE;
    VkPipeline m_pipeline=VK_NULL_HANDLE;
    uint32_t m_indexCount=0;
    bool m_autoRotation=false;
};

#endif
