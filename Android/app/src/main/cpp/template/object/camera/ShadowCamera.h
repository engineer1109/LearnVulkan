//
// Created by wjl on 21-5-24.
//

#ifndef LEARNVULKAN_SHADOWCAMERA_H
#define LEARNVULKAN_SHADOWCAMERA_H

#include "VulkanBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

class ShadowCamera : public VulkanBuffer {
public:
    struct ShadowMVP {
        glm::mat4 depthMVP;
    };
public:
    ShadowCamera() = default;
    ~ShadowCamera() = default;

    virtual void prepareUniformBuffers() override;
    virtual void updateUniformBuffers() override ;

public:
    ShadowMVP m_uboVS;
    float m_lightFOV = 45.f;
    float m_zNear = 0.001f;
    float m_zFar = 256.f;
    glm::vec3 m_lightPos = glm::vec3(0.f);
};

END_NAMESPACE(VulkanEngine)


#endif //LEARNVULKAN_SHADOWCAMERA_H
