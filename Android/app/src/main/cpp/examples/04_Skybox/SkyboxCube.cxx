//
// Created by wjl on 21-3-28.
//

#include "SkyboxCube.h"

#include "VulkanCube.h"
#include "VulkanVertFragShader.h"
#include "UniformCamera.h"
#include "VulkanTexture2D.h"
#include "VulkanTextureCubeMap.h"

BEGIN_NAMESPACE(VulkanEngine)

SkyboxCube::~SkyboxCube() {
    delete_ptr(m_cube);
    delete_ptr(m_cubeUniform);
    delete_ptr(m_cubeShader);
    delete_ptr(m_cubeTextureA);
    delete_ptr(m_cubeTextureB);
}

void SkyboxCube::prepareMyObjects() {
    m_zoom = -4.f;

    createCube();
    createSkybox();

    setDescriptorSet();
    createPipelines();
}

void SkyboxCube::buildMyObjects(VkCommandBuffer &cmd) {
    m_cube->build(cmd, m_cubeShader);
    m_sky->build(cmd, m_skyShader);
}

void SkyboxCube::render() {
    updateCamera();
    m_cubeUniform->update();
}

void SkyboxCube::setDescriptorSet() {
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
    m_vulkanDescriptorSet->addBinding(3, &(m_skyTexture->descriptor),
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      VK_SHADER_STAGE_FRAGMENT_BIT,
                                      0);
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void SkyboxCube::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_cubeShader);
    m_pipelines->createPipeline(m_skyShader);
}

void SkyboxCube::createCube() {
    m_cube = VkObject::New<VulkanCube>(m_context);
    m_cube->prepare();

    m_cubeShader = VkObject::New<VulkanVertFragShader>(m_context);
    m_cubeShader->setShaderObjPath("shaders/Texture2dCube/texture2d.so.vert",
                                   "shaders/Texture2dCube/texture2d.so.frag");
    m_cubeShader->setCullFlag(VK_CULL_MODE_BACK_BIT);
    m_cubeShader->prepare();

    m_cubeUniform = VkObject::New<UniformCamera>(m_context);
    m_cubeUniform->m_pCameraPos = &m_cameraPos;
    m_cubeUniform->m_pRotation = &m_rotation;
    m_cubeUniform->m_pZoom = &m_zoom;
    m_cubeUniform->prepare();

    m_cubeTextureA = VkObject::New<VulkanTexture2D>(m_context);
    m_cubeTextureA->loadFromFile("textures/awesomeface.png", VK_FORMAT_R8G8B8A8_UNORM);

    m_cubeTextureB = VkObject::New<VulkanTexture2D>(m_context);
    m_cubeTextureB->loadFromFile("textures/container.png", VK_FORMAT_R8G8B8A8_UNORM);
}

void SkyboxCube::createSkybox() {
    m_sky = VkObject::New<VulkanCube>(m_context);
    m_sky->prepare();

    m_skyShader = VkObject::New<VulkanVertFragShader>(m_context);
    m_skyShader->setShaderObjPath("shaders/Skybox/skybox.so.vert",
                                   "shaders/Skybox/skybox.so.frag");
    m_skyShader->setCullFlag(VK_CULL_MODE_FRONT_BIT);
    m_skyShader->prepare();

    std::vector<std::string> skyImages = {
            "textures/skybox/back.jpg",
            "textures/skybox/front.jpg",
            "textures/skybox/top.jpg",
            "textures/skybox/bottom.jpg",
            "textures/skybox/right.jpg",
            "textures/skybox/left.jpg",
    };
    m_skyTexture = VkObject::New<VulkanTextureCubeMap>(m_context);
    m_skyTexture->loadFromFile(skyImages, VK_FORMAT_R8G8B8A8_UNORM);

}

END_NAMESPACE(VulkanEngine)