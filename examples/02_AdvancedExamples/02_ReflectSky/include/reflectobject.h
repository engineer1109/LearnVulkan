/*
* LearnVulkan Examples
*
* Copyright (C) by Jialiang Wang - https://github.com/engineer1109/LearnVulkan
* 1292846099@qq.com
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef REFLECTOBJECT_H
#define REFLECTOBJECT_H
#include "VulkanModel.hpp"
#include "vulkan_basicengine_object.h"
class ReflectObject:public VulkanBaseObject{
public:
    ReflectObject()=default;
    ~ReflectObject();
public:
    vks::Buffer m_uniformBuffers;
    VkPipeline m_pipeline=VK_NULL_HANDLE;
    VkDescriptorSet m_descriptorSet=VK_NULL_HANDLE;
    struct UboVS {
        glm::mat4 projection;
        glm::mat4 model;
        float lodBias = 0.0f;
    } m_uboVS;
    struct ObjectCamera{
        float* zoom=nullptr;
        glm::vec3* rotation=nullptr;
        glm::vec3* cameraPos=nullptr;
    }m_camera;
public:
    void create();
    void build(VkCommandBuffer cmd,VkPipelineLayout pipelineLayout);
    void update();

    void setCamera(ObjectCamera camera);
private:
    void loadModel();
    void prepareUniformBuffers();
    void updateUniformBuffers();
private:
    vks::Model m_model;
    vks::VertexLayout m_vertexLayout = vks::VertexLayout({
        vks::VERTEX_COMPONENT_POSITION,
        vks::VERTEX_COMPONENT_NORMAL,
        vks::VERTEX_COMPONENT_UV,
    });
};
#endif // REFLECTOBJECT_H
