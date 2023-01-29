//
// Created by wjl on 2023/1/29.
//

#ifndef LEARNVULKAN_VOLUMEMODELUNIFORM_H
#define LEARNVULKAN_VOLUMEMODELUNIFORM_H

#include "VulkanBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

class VolumeModelUniform : public VulkanBuffer {
public:
    struct ReflectPara{
        glm::mat4 model;
    } m_model;

    virtual void prepareUniformBuffers() override;

    virtual void updateUniformBuffers() override;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VOLUMEMODELUNIFORM_H
