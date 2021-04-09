//
// Created by wjl on 21-4-5.
//

#include "PhongLighting.h"

#include "VulkanCube.h"
#include "VulkanVertFragShader.h"
#include "UniformCamera.h"
#include "VulkanTexture2D.h"
#include "VulkanTextureCubeMap.h"
#include "ReflectParaBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

PhongLighting::~PhongLighting() {
    destroyObjects();
}

void PhongLighting::prepareFunctions() {
    m_functions.emplace_back([this] { changeReflectMode(); });
}

void PhongLighting::prepareMyObjects() {
    m_zoom = -4.f;

    createCube();
    createSkybox();
    createReflectParaBuffer();

    setDescriptorSet();
    createPipelines();
}

void PhongLighting::buildMyObjects(VkCommandBuffer &cmd) {
    m_cube->build(cmd, m_cubeShader);
    m_sky->build(cmd, m_skyShader);
}

void PhongLighting::render() {
    updateCamera();
    m_cubeUniform->update();
}

void PhongLighting::setDescriptorSet() {
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
    m_vulkanDescriptorSet->addBinding(4, &(m_reflectParaBuffer->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                      VK_SHADER_STAGE_VERTEX_BIT,
                                      0);
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void PhongLighting::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_cubeShader);
    m_pipelines->createPipeline(m_skyShader);
}

void PhongLighting::createCube() {
    REGISTER_OBJECT<VulkanCube>(m_cube);
    m_cube->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_cubeShader);
    m_cubeShader->setShaderObjPath("shaders/PhongLighting/phonglighting.so.vert",
                                   "shaders/PhongLighting/phonglighting.so.frag");
    m_cubeShader->setCullFlag(VK_CULL_MODE_BACK_BIT);
    m_cubeShader->prepare();

    REGISTER_OBJECT<UniformCamera>(m_cubeUniform);
    m_cubeUniform->m_pCameraPos = &m_cameraPos;
    m_cubeUniform->m_pRotation = &m_rotation;
    m_cubeUniform->m_pZoom = &m_zoom;
    m_cubeUniform->prepare();

    REGISTER_OBJECT<VulkanTexture2D>(m_cubeTextureA);
    m_cubeTextureA->loadFromFile("textures/awesomeface.png", VK_FORMAT_R8G8B8A8_UNORM);

    REGISTER_OBJECT<VulkanTexture2D>(m_cubeTextureB);
    m_cubeTextureB->loadFromFile("textures/container.png", VK_FORMAT_R8G8B8A8_UNORM);
}

void PhongLighting::createSkybox() {
    REGISTER_OBJECT<VulkanCube>(m_sky);
    m_sky->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_skyShader);
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
    REGISTER_OBJECT<VulkanTextureCubeMap>(m_skyTexture);
    m_skyTexture->loadFromFile(skyImages, VK_FORMAT_R8G8B8A8_UNORM);

}

void PhongLighting::createReflectParaBuffer() {
    REGISTER_OBJECT<ReflectParaBuffer>(m_reflectParaBuffer);
    m_reflectParaBuffer->prepare();
}

void PhongLighting::changeReflectMode() {
    LOGI("changeReflect\n");
    m_reflectParaBuffer->reflect();
    m_reflectParaBuffer->update();
}

END_NAMESPACE(VulkanEngine)