//
// Created by wjl on 21-3-14.
//

#ifndef LEARNVULKAN_VULKANCUBE_H
#define LEARNVULKAN_VULKANCUBE_H

#include "vertex_struct.h"
#include "MeshObject.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API VULKANENGINE_EXPORT_API VulkanCube : public MeshObject {
public:
    VulkanCube() = default;
    virtual ~VulkanCube();

    void generateVertex() override;

    void updateVertex() override{}

    void setSize(float a, float b, float c){
        m_a = a;
        m_b = b;
        m_c = c;
    }

protected:
    float m_a = 1.f;
    float m_b = 1.f;
    float m_c = 1.f;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANCUBE_H
