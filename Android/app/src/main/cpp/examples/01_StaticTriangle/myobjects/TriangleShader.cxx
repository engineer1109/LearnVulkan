//
// Created by wjl on 21-2-28.
//

#include "TriangleShader.h"

BEGIN_NAMESPACE(VulkanEngine)

void TriangleShader::prepareShaders() {
    m_shaderStages.push_back(
            loadShader(FS::getPath("shaders/StaticTriangle/statictriangle.so.vert"), VK_SHADER_STAGE_VERTEX_BIT));
    m_shaderStages.push_back(
            loadShader(FS::getPath("shaders/StaticTriangle/statictriangle.so.frag"), VK_SHADER_STAGE_FRAGMENT_BIT));
}

END_NAMESPACE(VulkanEngine)
