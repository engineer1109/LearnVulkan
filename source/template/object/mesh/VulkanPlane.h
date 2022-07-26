//
// Created by wjl on 21-5-17.
//

#ifndef LEARNVULKAN_VULKANPLANE_H
#define LEARNVULKAN_VULKANPLANE_H

#include "vertex_struct.h"
#include "MeshObject.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API VulkanPlane : public MeshObject {
public:
    VulkanPlane() = default;

    ~VulkanPlane();

    void generateVertex() override;

    void updateVertex() override {}

    void setSize(float a, float b) {
        m_a = a;
        m_b = b;
    }

protected:
    float m_a = 1.f;
    float m_b = 1.f;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANPLANE_H
