/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef CUBETUNNEL_H
#define CUBETUNNEL_H
#include "camera.hpp"
#include "vulkan_basicengine_object.h"
#include "vulkan_basicengine_texture.h"
class CubeTunnel:public VulkanTemplate::VulkanBaseObject{
public:
    CubeTunnel()=default;
    ~CubeTunnel();

    void setCamera(Camera* camera){m_camera=camera;}
    void setTimer(float* timer){m_timer=timer;}
    void setLocation(float x,float y,float z){m_x=x;m_y=y;m_z=z;}
    void setSize(float size){m_size=size;}

    void create();
    void build(VkCommandBuffer cmd,VkPipelineLayout pipelineLayout);
    void update();
public:
    struct uboVS {
        glm::mat4 projection;
        glm::mat4 view;
        glm::mat4 model;
        glm::vec4 viewPos;
        float lodBias = 0.0f;
        int32_t samplerIndex = 2;
    } m_uboVS;
    VkDescriptorSet m_descriptorSet;
    VkPipeline m_pipeline;
    vks::Buffer m_uniformBufferVS;
    vks::Texture2DStbImage m_texture;
private:
    void generateVertex();
    void loadTextures();
    void prepareUniformBuffers();
    void updateUniformBuffers();

private:
    vks::Buffer m_vertexBuffer;
    vks::Buffer m_indexBuffer;
    uint32_t m_indexCount=0;
    Camera* m_camera=nullptr;
    float* m_timer=nullptr;
    struct Vertex {
        float pos[3];
        float uv[2];
        float normal[3];
    };
    float m_size=1.f;
    float m_x=0.f;
    float m_y=0.f;
    float m_z=0.f;
};
#endif // CUBETUNNEL_H
