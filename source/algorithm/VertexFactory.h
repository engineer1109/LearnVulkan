//
// Created by wjl on 2023/1/29.
//

#ifndef LEARNVULKAN_VERTEXFACTORY_H
#define LEARNVULKAN_VERTEXFACTORY_H

#include "base_template.h"
#include "vertex_struct.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API VertexFactory {
public:
    static std::vector<VertexTexVec4> GenPlaneVertex(int direction, glm::vec2 size);
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VERTEXFACTORY_H
