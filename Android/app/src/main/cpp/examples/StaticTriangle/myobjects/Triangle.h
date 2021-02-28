//
// Created by wjl on 21-2-28.
//

#ifndef LEARNVULKAN_TRIANGLE_H
#define LEARNVULKAN_TRIANGLE_H

#include "object/MeshObject.h"
#include "vertex_struct.h"

BEGIN_NAMESPACE(VulkanEngine)

class Triangle : public MeshObject {
public:
    Triangle() = default;
    virtual ~Triangle();

    void generateVertex() override;

    void updateVertex() override{}

};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_TRIANGLE_H
