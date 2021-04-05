//
// Created by wjl on 21-2-21.
//

#include "StaticCube.h"

#include "VulkanCube.h"
#include "VulkanVertFragShader.h"
#include "UniformCamera.h"

BEGIN_NAMESPACE(VulkanEngine)

StaticCube::~StaticCube() {
    destroyObjects();
}

void StaticCube::prepareMyObjects() {
    m_zoom = -4.f;

    createCube();

    setDescriptorSet();
    createPipelines();
}

void StaticCube::buildMyObjects(VkCommandBuffer &cmd) {
    m_cube->build(cmd, m_cubeShader);
}

void StaticCube::render() {
    updateCamera();
    m_cubeUniform->update();
}

void StaticCube::setDescriptorSet() {
    m_vulkanDescriptorSet->addBinding(0, &(m_cubeUniform->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT,
                                      0);
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void StaticCube::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_cubeShader);
}

void StaticCube::createCube() {
    REGISTER_OBJECT<VulkanCube>(m_cube);
    m_cube->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_cubeShader);
    m_cubeShader->setShaderObjPath("shaders/StaticCube/staticcube.so.vert",
                                   "shaders/StaticCube/staticcube.so.frag");
    m_cubeShader->prepare();

    REGISTER_OBJECT<UniformCamera>(m_cubeUniform);
    m_cubeUniform->m_pCameraPos = &m_cameraPos;
    m_cubeUniform->m_pRotation = &m_rotation;
    m_cubeUniform->m_pZoom = &m_zoom;
    m_cubeUniform->prepare();
}

END_NAMESPACE(VulkanEngine)