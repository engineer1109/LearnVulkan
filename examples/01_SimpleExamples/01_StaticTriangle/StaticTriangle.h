//
// Created by wjl on 21-2-21.
//

#ifndef LEARNVULKAN_STATICTRIANGLE_H
#define LEARNVULKAN_STATICTRIANGLE_H

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class Triangle;

class TriangleShader;

class TriangleUniform;

class StaticTriangle : public ThirdPersonEngine{
public:
    StaticTriangle() = default;

    virtual ~StaticTriangle();

public:

    void prepareMyObjects() override ;

    void buildMyObjects(VkCommandBuffer &cmd) override ;

    void render() override;

    void setDescriptorSet();

    void createPipelines();

    void createTriangle();

private:
    std::shared_ptr<Triangle> m_triangle = nullptr;
    std::shared_ptr<TriangleShader> m_triangleShader = nullptr;
    std::shared_ptr<TriangleUniform> m_triangleUniform = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_STATICTRIANGLE_H
