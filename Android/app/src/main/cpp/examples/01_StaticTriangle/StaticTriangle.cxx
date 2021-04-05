//
// Created by wjl on 21-2-21.
//

#include "StaticTriangle.h"

#include "myobjects/Triangle.h"
#include "myobjects/TriangleShader.h"
#include "myobjects/TriangleUniform.h"

BEGIN_NAMESPACE(VulkanEngine)

//StaticTriangle::StaticTriangle(){}

StaticTriangle::~StaticTriangle() {
    destroyObjects();
}

void StaticTriangle::prepareMyObjects() {
    createTriangle();

    setDescriptorSet();
    createPipelines();
}

void StaticTriangle::buildMyObjects(VkCommandBuffer &cmd) {
    m_triangle->build(cmd, m_triangleShader);
}

void StaticTriangle::render() {
    updateCamera();
    m_triangleUniform->update();
}

void StaticTriangle::createTriangle() {
    REGISTER_OBJECT<Triangle>(m_triangle);
    m_triangle->prepare();

    REGISTER_OBJECT<TriangleShader>(m_triangleShader);
    m_triangleShader->prepare();

    REGISTER_OBJECT<TriangleUniform>(m_triangleUniform);
    m_triangleUniform->m_pCameraPos = &m_cameraPos;
    m_triangleUniform->m_pRotation = &m_rotation;
    m_triangleUniform->m_pZoom = &m_zoom;
    m_triangleUniform->prepare();
}

void StaticTriangle::setDescriptorSet() {
    m_vulkanDescriptorSet->addBinding(0, &(m_triangleUniform->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT,
                                      0);
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void StaticTriangle::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_triangleShader);
}

END_NAMESPACE(VulkanEngine)
