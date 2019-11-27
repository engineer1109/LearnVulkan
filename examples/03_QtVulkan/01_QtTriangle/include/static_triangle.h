#ifndef STATIC_TRIANGLE_H
#define STATIC_TRIANGLE
#include "vulkan_basicengine.h"
class StaticTriangle:public VulkanBasicEngine{
public:
    StaticTriangle(bool debugLayer);
    ~StaticTriangle();
public:
    virtual void prepare();
    virtual void render();
    virtual void draw();

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);

    void enableAutoRotation(bool value=true){m_autoRotation=value;}
    void renderAsyncThread();
    void renderJoin();
    void rebuildCommandBuffers(float r,float g,float b);
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
    std::thread* m_thread;
    float m_backgroundRed=0.1f;
    float m_backgroundGreen=0.2f;
    float m_backgroundBlue=0.3f;
};

#endif
