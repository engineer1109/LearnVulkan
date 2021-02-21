//
// Created by wjl on 20-11-22.
//

#ifndef LEARNVULKAN_VERTEX_STRUCT_H
#define LEARNVULKAN_VERTEX_STRUCT_H

#include "render_common.h"

BEGIN_NAMESPACE(OpenGLESEngine)

struct Vertex {
    float pos[3];
    float uv[2];
    float normal[3];
};

struct VertexUVW {
    float pos[3];
    float uv[3];
    float normal[3];
};

END_NAMESPACE(OpenGLESEngine)

#endif //LEARNVULKAN_VERTEX_STRUCT_H
