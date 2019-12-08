#ifndef VULKAN_BASICENGINE_TEXTURE3DPLANE_H
#define VULKAN_BASICENGINE_TEXTURE3DPLANE_H
#include "vulkan_basicengine_texture3d.h"
#include "vulkan_basicengine_vertexobject.h"
BEGIN_NAMESPACE(VulkanTemplate)
class VK_BASICENGINE_EXPORT Texture3DPlane:public VertexObject{
public:
    Texture3DPlane()=default;
    ~Texture3DPlane();

    virtual void create();

    void loadTexture3D(uint8_t* imgData,uint32_t width,uint32_t height,uint32_t depth,VkFormat format=VK_FORMAT_R8_UNORM);

protected:
    void generateVertex();
    void setupVertexDescriptions();
public:
    struct Vertex {
        float pos[3];
        float uvw[3];
        float normal[3];
    };
    struct {
        VkPipelineVertexInputStateCreateInfo inputState;
        std::vector<VkVertexInputBindingDescription> inputBinding;
        std::vector<VkVertexInputAttributeDescription> inputAttributes;
    } m_vertices;

    vks::Texture3D m_texture3d;
};
END_NAMESPACE(VulkanTemplate)
#endif // VULKAN_BASICENGINE_TEXTURE3DPLANE_H
