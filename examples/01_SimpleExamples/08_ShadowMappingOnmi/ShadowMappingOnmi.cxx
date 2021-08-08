//
// Created by wjl on 2021/7/18.
//

#include "ShadowMappingOnmi.h"

#include "VulkanCube.h"
#include "VulkanVertFragShader.h"
#include "UniformCamera.h"
#include "VulkanTexture2D.h"
#include "VulkanTextureCubeMap.h"
#include "VulkanPlane.h"

#include "VulkanFrameBuffer.h"
#include "ReflectParaBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

ShadowMappingOnmi::~ShadowMappingOnmi() {}

void ShadowMappingOnmi::prepareFunctions() {}

void ShadowMappingOnmi::prepareMyObjects() {
    m_zoom = -4.f;

    createCube();
    createSkybox();
    createReflectParaBuffer();
    createRoom();
    createOnmiShadowFrameBuffer();

    setDescriptorSet();
    createPipelines();
}

void ShadowMappingOnmi::buildMyObjects(VkCommandBuffer &cmd) {
    m_cube->build(cmd, m_cubeShader);
    m_sky->build(cmd, m_skyShader);
    m_room->build(cmd, m_roomShader);
}

void ShadowMappingOnmi::render() {
    updateCamera();
    m_cubeUniform->update();
}

void ShadowMappingOnmi::setDescriptorSet() {
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
    m_vulkanDescriptorSet->addBinding(7, &(m_reflectParaBuffer->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                      VK_SHADER_STAGE_VERTEX_BIT,
                                      0);
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void ShadowMappingOnmi::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_cubeShader);
    m_pipelines->createPipeline(m_skyShader);
    m_pipelines->createPipeline(m_roomShader);
}

void ShadowMappingOnmi::createCube() {
    REGISTER_OBJECT<VulkanCube>(m_cube);
    m_cube->setPosOffset({-2, -2, 0});
    m_cube->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_cubeShader);
    m_cubeShader->setShaderObjPath(FS::getPath("shaders/ShadowMappingOnmi/scene.so.vert"),
                                   FS::getPath("shaders/ShadowMappingOnmi/scene.so.frag"));
    m_cubeShader->setCullFlag(VK_CULL_MODE_BACK_BIT);
    m_cubeShader->prepare();

    REGISTER_OBJECT<UniformCamera>(m_cubeUniform);
    m_cubeUniform->m_uboVS.lightpos = glm::vec4(0.0f, -0.0f, 0.0f, 1.0f);
    m_cubeUniform->m_pCameraPos = &m_cameraPos;
    m_cubeUniform->m_pRotation = &m_rotation;
    m_cubeUniform->m_pZoom = &m_zoom;
    m_cubeUniform->prepare();

    REGISTER_OBJECT<VulkanTexture2D>(m_cubeTextureA);
    m_cubeTextureA->loadFromFile(FS::getPath("textures/awesomeface.png"), VK_FORMAT_R8G8B8A8_UNORM);

    REGISTER_OBJECT<VulkanTexture2D>(m_cubeTextureB);
    m_cubeTextureB->loadFromFile(FS::getPath("textures/container.png"), VK_FORMAT_R8G8B8A8_UNORM);
}

void ShadowMappingOnmi::createSkybox() {
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

void ShadowMappingOnmi::createReflectParaBuffer() {
    REGISTER_OBJECT<ReflectParaBuffer>(m_reflectParaBuffer);
    m_reflectParaBuffer->prepare();
}

void ShadowMappingOnmi::createRoom() {
    REGISTER_OBJECT<VulkanCube>(m_room);
    m_room->setSize(5, 5, 5);
    m_room->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_roomShader);
    m_roomShader->setShaderObjPath(FS::getPath("shaders/ShadowMappingOnmi/room.so.vert"),
                                  FS::getPath("shaders/ShadowMappingOnmi/room.so.frag"));
    m_roomShader->setCullFlag(VK_CULL_MODE_NONE);
    m_roomShader->prepare();
}

void ShadowMappingOnmi::createOnmiShadowFrameBuffer() {
    REGISTER_OBJECT<VulkanTextureCubeMap>(m_shadowMapTexture);
    m_shadowMapTexture->defaultAddressMode = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    m_shadowMapTexture->allocate(VK_FORMAT_R32_SFLOAT, 1024, 1024, 1);

    m_frameBuffer = new VulkanFrameBuffer();
    m_frameBuffer->setContext(m_context);
    m_frameBuffer->setFormat(VK_FORMAT_R32_SFLOAT);
    m_frameBuffer->setSize(1024, 1024);
    m_frameBuffer->createWithColorDepth();
}

END_NAMESPACE(VulkanEngine)