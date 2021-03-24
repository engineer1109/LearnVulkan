//
// Created by wjl on 21-3-22.
//

#include "Texture2dCube.h"

#include "VulkanCube.h"
#include "VulkanVertFragShader.h"
#include "UniformCamera.h"
#include "VulkanTexture2D.h"

BEGIN_NAMESPACE(VulkanEngine)

Texture2dCube::~Texture2dCube() {
    delete_ptr(m_cube);
    delete_ptr(m_cubeUniform);
    delete_ptr(m_cubeShader);
    delete_ptr(m_cubeTextureA);
    delete_ptr(m_cubeTextureB);
}

void Texture2dCube::prepareMyObjects() {
    m_zoom = -4.f;

    createCube();

    setDescriptorSet();
    createPipelines();
}

void Texture2dCube::buildMyObjects(VkCommandBuffer &cmd) {
    m_cube->build(cmd, m_cubeShader);
}

void Texture2dCube::render() {
    updateCamera();
    m_cubeUniform->update();
}

void Texture2dCube::setDescriptorSet() {
    m_vulkanDescriptorSet->addBinding(0, &(m_cubeUniform->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT,
                                      0);
    m_vulkanDescriptorSet->addBinding(1, &(m_cubeTextureA->descriptor),
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      VK_SHADER_STAGE_FRAGMENT_BIT,
                                      0);
    m_vulkanDescriptorSet->addBinding(2, &(m_cubeTextureB->descriptor),
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      VK_SHADER_STAGE_FRAGMENT_BIT,
                                      0);
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void Texture2dCube::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_cubeShader);
}

void Texture2dCube::createCube() {
    m_cube = VkObject::New<VulkanCube>(m_context);
    m_cube->prepare();

    m_cubeShader = VkObject::New<VulkanVertFragShader>(m_context);
    m_cubeShader->setShaderObjPath("shaders/Texture2dCube/texture2d.so.vert",
                                   "shaders/Texture2dCube/texture2d.so.frag");
    m_cubeShader->prepare();

    m_cubeUniform = VkObject::New<UniformCamera>(m_context);
    m_cubeUniform->m_pCameraPos = &m_cameraPos;
    m_cubeUniform->m_pRotation = &m_rotation;
    m_cubeUniform->m_pZoom = &m_zoom;
    m_cubeUniform->prepare();

    m_cubeTextureA = new VulkanTexture2D();
    m_cubeTextureA->loadFromFile("textures/awesomeface.png", m_asset, VK_FORMAT_R8G8B8A8_UNORM,
                                 m_vulkanDevice, m_queue);

    m_cubeTextureB = new VulkanTexture2D();
    m_cubeTextureB->loadFromFile("textures/container.png", m_asset, VK_FORMAT_R8G8B8A8_UNORM,
                                 m_vulkanDevice, m_queue);
}

END_NAMESPACE(VulkanEngine)