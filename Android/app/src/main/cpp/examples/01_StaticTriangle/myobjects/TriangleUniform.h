//
// Created by wjl on 21-2-28.
//

#ifndef LEARNVULKAN_TRIANGLEUNIFORM_H
#define LEARNVULKAN_TRIANGLEUNIFORM_H

#include "UniformCamera.h"

BEGIN_NAMESPACE(VulkanEngine)

class TriangleUniform : public UniformCamera {
public:
    TriangleUniform() = default;
    ~TriangleUniform() = default;

    virtual void updateUniformBuffers() override ;

public:
    glm::vec3 *m_pRotation = nullptr;
    glm::vec3 *m_pCameraPos = nullptr;
    float *m_pZoom = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_TRIANGLEUNIFORM_H
