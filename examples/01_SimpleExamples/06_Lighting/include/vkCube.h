#ifndef VKCUBE_H
#define VKCUBE_H
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "VulkanDevice.hpp"
#include <vulkan_basicengine_texture.h>
class VkCube{
public:
    VkCube();
    ~VkCube();
public:
    struct ObjectInfo{
        vks::VulkanDevice* vulkanDevice=nullptr;
        VkInstance instance=VK_NULL_HANDLE;
        VkCommandPool cmdPool=VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout=VK_NULL_HANDLE;
        VkPipelineCache pipelineCache=VK_NULL_HANDLE;
        VkRenderPass renderPass=VK_NULL_HANDLE;
        VkQueue queue=VK_NULL_HANDLE;
        uint32_t* screenWitdh=nullptr;
        uint32_t* screenHeight=nullptr;
    }m_objectInfo;
    struct ObjectCamera{
        float* zoom=nullptr;
        glm::vec3* rotation=nullptr;
        glm::vec3* cameraPos=nullptr;
    }m_camera;
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

    VkPipeline m_pipeline;
    vks::Buffer m_uniformBufferVS;
    vks::Texture2DStbImage m_textureA;
    vks::Texture2DStbImage m_textureB;
public:
    void setObjectInfo(ObjectInfo info);
    void setCamera(ObjectCamera camera);
    void setLocation(float x,float y,float z);
    void setSize(float size);
    void create();
    void build(VkCommandBuffer cmd);
    void update();
private:
    void generateVertex();
    void setupVertexDescriptions();
    void loadTexture2D();
    void prepareUniformBuffers();
    void updateUniformBuffers(bool viewchanged=false);
private:
    vks::VulkanDevice* m_vulkanDevice=nullptr;
    VkDevice m_device=VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice=VK_NULL_HANDLE;
    VkInstance m_instance=VK_NULL_HANDLE;
    VkCommandPool m_cmdPool=VK_NULL_HANDLE;
    VkPipelineLayout m_pipelineLayout=VK_NULL_HANDLE;
    VkPipelineCache m_pipelineCache=VK_NULL_HANDLE;
    VkRenderPass m_renderPass=VK_NULL_HANDLE;
    VkQueue m_queue=VK_NULL_HANDLE;
    uint32_t* m_screenWidth=nullptr;
    uint32_t* m_screenHeight=nullptr;
    vks::Buffer m_vertexBuffer;
    vks::Buffer m_indexBuffer;
    uint32_t m_indexCount=0;
    float m_size=1.f;
    float m_x=0.f;
    float m_y=0.f;
    float m_z=0.f;
};
#endif
