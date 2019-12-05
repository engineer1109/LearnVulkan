/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef INSTANCECUBE_H
#define INSTANCECUBE_H
#include "vulkan_basicengine_object.h"
#include "vulkan_basicengine_texture.h"

class InstanceCube:public VulkanTemplate::VulkanBaseObject{
public:
    InstanceCube()=default;
    ~InstanceCube();

    void create();
    void build(VkCommandBuffer cmd,VkPipelineLayout pipelineLayout);
    void update();
public:
    struct InstanceBuffer {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        size_t size = 0;
        VkDescriptorBufferInfo descriptor;
    } m_instanceBuffer;

    struct UBOVS {
        glm::mat4 projection;
        glm::mat4 view;
        glm::vec4 lightPos = glm::vec4(0.0f, -5.0f, 0.0f, 1.0f);
        float locSpeed = 0.0f;
        float globSpeed = 0.0f;
    } m_uboVS;

    struct InstanceData {
        glm::vec3 pos;
        glm::vec3 rot;
        float scale;
        uint32_t texIndex;
    };

    struct {
        VkPipelineVertexInputStateCreateInfo inputState;
        std::vector<VkVertexInputBindingDescription> bindingDescriptions;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    } m_vertices;

    VkDescriptorSet m_descriptorSet;
    VkPipeline m_pipeline;
    vks::Buffer m_uniformBufferVS;
    vks::Texture2DStbImage m_texture;
private:
    void setupVertexDescriptions();
    void generateVertex();
    void loadTextures();
private:
    vks::Buffer m_vertexBuffer;
    vks::Buffer m_indexBuffer;
    uint32_t m_indexCount=0;
    struct Vertex {
        float pos[3];
        float uv[2];
        float normal[3];
        float color[3];
    };

    float m_size=1.f;
    float m_x=0.f;
    float m_y=0.f;
    float m_z=0.f;
};

#endif // INSTANCECUBE_H
