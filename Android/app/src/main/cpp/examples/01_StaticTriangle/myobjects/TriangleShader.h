//
// Created by wjl on 21-2-28.
//

#ifndef LEARNVULKAN_TRIANGLESHADER_H
#define LEARNVULKAN_TRIANGLESHADER_H

#include "VulkanShader.h"

BEGIN_NAMESPACE(VulkanEngine)

class TriangleShader : public VulkanShader {
public:
    TriangleShader() = default;
    ~TriangleShader() = default;

    virtual void prepareShaders() override;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_TRIANGLESHADER_H
