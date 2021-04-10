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
    destroyObjects();
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
    REGISTER_OBJECT<VulkanCube>(m_cube);
    m_cube->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_cubeShader);
    m_cubeShader->setShaderObjPath(FS::getPath("shaders/Texture2dCube/texture2d.so.vert"),
                                   FS::getPath("shaders/Texture2dCube/texture2d.so.frag"));
    m_cubeShader->setCullFlag(VK_CULL_MODE_BACK_BIT);
    m_cubeShader->prepare();

    REGISTER_OBJECT<UniformCamera>(m_cubeUniform);
    m_cubeUniform->m_pCameraPos = &m_cameraPos;
    m_cubeUniform->m_pRotation = &m_rotation;
    m_cubeUniform->m_pZoom = &m_zoom;
    m_cubeUniform->prepare();

    REGISTER_OBJECT<VulkanTexture2D>(m_cubeTextureA);
    m_cubeTextureA->loadFromFile(FS::getPath("textures/awesomeface.png"), VK_FORMAT_R8G8B8A8_UNORM);

    REGISTER_OBJECT<VulkanTexture2D>(m_cubeTextureB);
    m_cubeTextureB->loadFromFile(FS::getPath("textures/container.png"), VK_FORMAT_R8G8B8A8_UNORM);
}

void SkyboxCube::createSkybox() {
    REGISTER_OBJECT<VulkanCube>(m_sky);
    m_sky->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_skyShader);
    m_skyShader->setShaderObjPath(FS::getPath("shaders/Skybox/skybox.so.vert"),
                                  FS::getPath("shaders/Skybox/skybox.so.frag"));
    m_skyShader->setCullFlag(VK_CULL_MODE_FRONT_BIT);
    m_skyShader->prepare();

    std::vector<std::string> skyImages = {
            FS::getPath("textures/skybox/back.jpg"),
            FS::getPath("textures/skybox/front.jpg"),
            FS::getPath("textures/skybox/top.jpg"),
            FS::getPath("textures/skybox/bottom.jpg"),
            FS::getPath("textures/skybox/right.jpg"),
            FS::getPath("textures/skybox/left.jpg"),
    };
    REGISTER_OBJECT<VulkanTextureCubeMap>(m_skyTexture);
    m_skyTexture->loadFromFile(skyImages, VK_FORMAT_R8G8B8A8_UNORM);

}

END_NAMESPACE(VulkanEngine)