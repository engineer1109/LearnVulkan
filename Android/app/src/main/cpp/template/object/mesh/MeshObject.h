//
// Created by wjl on 21-2-28.
//

#ifndef LEARNVULKAN_MESHOBJECT_H
#define LEARNVULKAN_MESHOBJECT_H

#include "VkObject.h"
#include "VulkanContext.h"
#include "VulkanBuffer.hpp"
#include "VulkanShader.h"

BEGIN_NAMESPACE(VulkanEngine)

class MeshObject : public VkObject {
public:
    MeshObject() = default;
    virtual ~MeshObject();

    virtual void prepare() override;

    virtual void update() override;

    virtual void generateVertex() = 0;

    virtual void updateVertex() = 0;

    void build(VkCommandBuffer &cmdBuffer, VulkanShader* vulkanShader);

public:
    vks::Buffer m_vertexBuffer;
    vks::Buffer m_indexBuffer;
    uint32_t m_indexCount = 0;

};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_MESHOBJECT_H
