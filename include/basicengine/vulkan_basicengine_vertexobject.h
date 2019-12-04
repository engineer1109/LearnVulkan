#ifndef VULKAN_BASICENGINE_VERTEXOBJECT_H
#define VULKAN_BASICENGINE_VERTEXOBJECT_H
#include "vulkan_basicengine_def.h"
#include "vulkan_basicengine_object.h"
namespace VulkanTemplate {

class VK_BASICENGINE_EXPORT VertexObject:public VulkanBaseObject{
public:
    VertexObject()=default;
    virtual ~VertexObject();

    virtual void create();
    virtual void update();
protected:
    virtual void generateVertex();
    virtual void prepareUniformBuffers();
    virtual void updateUniformBuffers();
public:
    struct UboVS {
        glm::mat4 projection;
        glm::mat4 model;
        glm::vec4 viewPos;
        float lodBias = 10.0f;
    } m_uboVS;
    vks::Buffer m_uniformBuffers;
    VkPipeline m_pipeline=VK_NULL_HANDLE;
    VkDescriptorSet m_descriptorSet=VK_NULL_HANDLE;
protected:
    vks::Buffer m_vertexBuffer;
    vks::Buffer m_indexBuffer;
    uint32_t m_indexCount=0;
};

}
#endif // VULKAN_BASICENGINE_VERTEXOBJECT_H
