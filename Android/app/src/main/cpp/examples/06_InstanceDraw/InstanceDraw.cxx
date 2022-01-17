//
// Created by wjl on 21-4-11.
//

#include "InstanceDraw.h"

#include "DefaultInstanceDescription.h"

#include "VulkanCube.h"
#include "VulkanVertFragShader.h"
#include "UniformCamera.h"
#include "VulkanTexture2D.h"
#include "VulkanTextureCubeMap.h"
#include "InstanceCube.h"
#include "SpeedBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

InstanceDraw::~InstanceDraw() {}

void InstanceDraw::prepareFunctions() {}

void InstanceDraw::prepareMyObjects() {
    createInstanceDescription();

    createSkybox();

    createInstanceCube();

    createSpeedBuffer();

    setDescriptorSet();

    createPipelines();
}

void InstanceDraw::buildMyObjects(VkCommandBuffer &cmd) {
    m_instanceCube->build(cmd, m_instanceCubeShader);
    m_sky->build(cmd, m_skyShader);
}

void InstanceDraw::render() {
    updateCamera();
    m_cubeUniform->update();
    m_speedBuffer->m_speedData.speed[0] += m_frameTimer * 0.35f;
    m_speedBuffer->m_speedData.speed[1] += m_frameTimer * 0.15f;
    m_speedBuffer->update();
}

void InstanceDraw::createInstanceDescription() {
    m_instanceDescription = std::make_shared<DefaultInstanceDescription>();
    m_instanceDescription->GenerateInstanceDescriptions();
}

void InstanceDraw::setDescriptorSet() {
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
    m_vulkanDescriptorSet->addBinding(4, &(m_speedBuffer->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                      VK_SHADER_STAGE_VERTEX_BIT,
                                      0);
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void InstanceDraw::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_skyShader);
    LOGI("InstanceShader\n");
    m_pipelines->createPipeline(m_instanceCubeShader);
}

void InstanceDraw::createInstanceCube() {
    REGISTER_OBJECT<InstanceCube>(m_instanceCube);
    m_instanceCube->setInstanceCount(100);
    m_instanceCube->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_instanceCubeShader);
    m_instanceCubeShader->setShaderObjPath(FS::getPath("shaders/InstanceDraw/instancedraw.so.vert"),
                                           FS::getPath(
                                                   "shaders/InstanceDraw/instancedraw.so.frag"));
    m_instanceCubeShader->setVertexInputState(m_instanceDescription->m_inputState);
    m_instanceCubeShader->prepare();

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

void InstanceDraw::createSkybox() {
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

void InstanceDraw::createSpeedBuffer() {
    REGISTER_OBJECT<SpeedBuffer>(m_speedBuffer);
    m_speedBuffer->prepare();
}

END_NAMESPACE(VulkanEngine)