//
// Created by wjl on 21-3-14.
//

#ifndef LEARNVULKAN_VULKANCUBE_H
#define LEARNVULKAN_VULKANCUBE_H

#include "vertex_struct.h"
#include "MeshObject.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanCube : public MeshObject {
public:
    VulkanCube() = default;
    virtual ~VulkanCube();

    void generateVertex() override;

    void updateVertex() override{}
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANCUBE_H
