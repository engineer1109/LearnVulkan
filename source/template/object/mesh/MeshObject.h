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

    virtual void build(VkCommandBuffer &cmdBuffer, VulkanShader *vulkanShader);

    virtual void build(VkCommandBuffer &cmdBuffer, std::shared_ptr<VulkanShader> vulkanShader) {
        this->build(cmdBuffer, vulkanShader.get());
    }

    void setPosOffset(const glm::vec3 &offset) { m_posOffset = offset; }

    template<class T>
    void staticMove(std::vector<T> &vertexs) {
        for (auto &vertex : vertexs) {
            vertex.pos[0] += m_posOffset.x;
            vertex.pos[1] += m_posOffset.y;
            vertex.pos[2] += m_posOffset.z;
        }
    }

public:
    vks::Buffer m_vertexBuffer;
    vks::Buffer m_indexBuffer;
    uint32_t m_indexCount = 0;
    glm::vec3 m_posOffset = glm::vec3(0.f);

};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_MESHOBJECT_H
