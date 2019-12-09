/*
* LearnVulkan BasicEngine
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef VULKAN_BASICENGINE_VERTEXOBJECT_H
#define VULKAN_BASICENGINE_VERTEXOBJECT_H
#include "vulkan_basicengine_def.h"
#include "vulkan_basicengine_object.h"
BEGIN_NAMESPACE(VulkanTemplate)

class VK_BASICENGINE_EXPORT VertexObject:public VulkanBaseObject{
public:
    VertexObject()=default;
    virtual ~VertexObject();

    virtual void create();
    virtual void build(VkCommandBuffer cmd);
    virtual void update();
protected:
    virtual void generateVertex();
public:
    struct UboVS {
        glm::mat4 projection;
        glm::mat4 model;
        glm::vec4 viewPos;
        float lodBias = 10.0f;
    } m_uboVS;
    struct ObjectCamera{
        float* zoom=nullptr;
        glm::vec3* rotation=nullptr;
        glm::vec3* cameraPos=nullptr;
    }m_camera;
    vks::Buffer m_uniformBuffers;
    VkPipeline m_pipeline=VK_NULL_HANDLE;
    VkDescriptorSet m_descriptorSet=VK_NULL_HANDLE;
public:
    void setCamera(ObjectCamera camera){m_camera=camera;}
protected:
    vks::Buffer m_vertexBuffer;
    vks::Buffer m_indexBuffer;
    uint32_t m_indexCount=0;
};

END_NAMESPACE(VulkanTemplate)
#endif // VULKAN_BASICENGINE_VERTEXOBJECT_H
