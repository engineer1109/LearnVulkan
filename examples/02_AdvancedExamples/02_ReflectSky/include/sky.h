#ifndef SKY_H
#define SKY_H
#include "VulkanModel.hpp"
#include "vulkan_basicengine_object.h"
#include "vulkan_basicengine_texture.h"
class Sky:public VulkanBaseObject{
public:
    Sky()=default;
    ~Sky()=default;
public:
    vks::Buffer m_uniformBuffers;
    VkPipeline m_pipeline=VK_NULL_HANDLE;
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

    void setCamera(ObjectCamera camera){m_camera=camera;}
private:
    void prepareTextures();
    void prepareAssets();
    void prepareUniformBuffers();
    void updateUniformBuffers();
private:
    vks::TextureCubeMapStbImage m_textureSkybox;
    vks::Model m_model;
    vks::VertexLayout m_vertexLayout = vks::VertexLayout({
        vks::VERTEX_COMPONENT_POSITION,
        vks::VERTEX_COMPONENT_NORMAL,
        vks::VERTEX_COMPONENT_UV,
    });
};
#endif // SKY_H
