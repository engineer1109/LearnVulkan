//
// Created by wjl on 2023/1/29.
//

#include "VertexFactory.h"

BEGIN_NAMESPACE(VulkanEngine)

#define DIRECTION_XZ 0
#define DIRECTION_XY 1

std::vector<VertexTexVec4> VertexFactory::GenPlaneVertex(int direction, glm::vec2 size) {
    std::vector<VertexTexVec4> vertices{};
    if (direction == DIRECTION_XZ) {
        vertices =
                {

                        {{size.x,  -0.0f, size.y},  {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
                        {{-size.x, -0.0f, size.y},  {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
                        {{-size.x, -0.0f, -size.y}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
                        {{size.x,  -0.0f, -size.y}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},

                };
    } else if (direction == DIRECTION_XY) {
        vertices =
                {

                        {{size.x,  size.y,  0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
                        {{-size.x, size.y,  0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
                        {{-size.x, -size.y, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
                        {{size.x,  -size.y, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},

                };
    }
    return vertices;
}

END_NAMESPACE(VulkanEngine)