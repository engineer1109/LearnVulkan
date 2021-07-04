//
// Created by wjl on 21-4-10.
//

#ifndef LEARNVULKAN_REFLECTPARABUFFER_H
#define LEARNVULKAN_REFLECTPARABUFFER_H

#include "VulkanBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

BEGIN_NAMESPACE(AssimpSample)

class ReflectParaBuffer : public VulkanBuffer {
public:
    struct ReflectPara{
        glm::vec4 para;
    } m_para;

public:
    ReflectParaBuffer() = default;
    ~ReflectParaBuffer() = default;

    void setReflect(float value) { m_para.para[0] = value; }

    float getReflect() { return m_para.para[0];}

    void reflect();

    virtual void prepareUniformBuffers() override;
    virtual void updateUniformBuffers() override ;
};

END_NAMESPACE(AssimpSample)

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_REFLECTPARABUFFER_H
