//
// Created by wjl on 21-4-15.
//

#ifndef LEARNVULKAN_INSTANCECUBE_H
#define LEARNVULKAN_INSTANCECUBE_H

#include "VulkanCube.h"
#include "DefaultInstanceDescription.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API InstanceCube : public VulkanCube {
public:
    InstanceCube() = default;

    virtual ~InstanceCube();

    void setInstanceCount(int count) { m_instanceCount = count; }

    virtual void prepare() override;

    virtual void createInstanceData();

    template<class T>
    void createInstanceBuffer(const std::vector<T> &instanceData);

    virtual void build(VkCommandBuffer &cmdBuffer, VulkanShader *vulkanShader) override;

    virtual void build(VkCommandBuffer &cmdBuffer, std::shared_ptr<VulkanShader> vulkanShader) override {
        this->build(cmdBuffer, vulkanShader.get());
    }

public:
    struct InstanceBuffer {
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        size_t size = 0;
        VkDescriptorBufferInfo descriptor;
    } m_instanceBuffer;

protected:
    int m_instanceCount = 128;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_INSTANCECUBE_H
