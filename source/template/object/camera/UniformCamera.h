//
// Created by wjl on 21-2-28.
//

#ifndef LEARNVULKAN_UNIFORMCAMERA_H
#define LEARNVULKAN_UNIFORMCAMERA_H

#include "VulkanBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API UniformCamera : public VulkanBuffer {
public:
    struct CameraMatrix{
        glm::mat4 projection;
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 normal;
        glm::vec4 lightpos;
    };

public:
    UniformCamera() = default;
    ~UniformCamera() = default;

    virtual void prepareUniformBuffers() override;
    virtual void updateUniformBuffers() override ;

public:
    CameraMatrix m_uboVS;

    glm::vec3 *m_pRotation = nullptr;
    glm::vec3 *m_pCameraPos = nullptr;
    float *m_pZoom = nullptr;

};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_UNIFORMCAMERA_H
