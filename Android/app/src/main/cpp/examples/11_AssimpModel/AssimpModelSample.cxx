//
// Created by wjl on 2021/6/20.
//

#include "AssimpModelSample.h"

#include "VulkanCube.h"
#include "VulkanVertFragShader.h"
#include "UniformCamera.h"
#include "VulkanTexture2D.h"
#include "VulkanTextureCubeMap.h"
#include "VulkanPlane.h"
#include "AssimpObject.h"

#include "VulkanFrameBuffer.h"
#include "ShadowCamera.h"
#include "ReflectParaBuffer.h"

BEGIN_NAMESPACE(VulkanEngine)

AssimpModelSample::~AssimpModelSample() {}

void AssimpModelSample::prepareFunctions() {
    m_functions.emplace_back([this] { seeDebugQuad(); });
}

void AssimpModelSample::prepareMyObjects() {
    m_zoom = -4.f;

    createCube();
    createSkybox();
    createPlane();
    createShadowFrameBuffer();
    createDebugQuad();
    createReflectParaBuffer();

    setDescriptorSet();
    createPipelines();
}

void AssimpModelSample::buildMyObjects(VkCommandBuffer &cmd) {
    m_assimpObject->build(cmd, m_cubeShader);
    m_sky->build(cmd, m_skyShader);
    m_plane->build(cmd, m_planeShader);
    if(m_seeDebug){
        m_dubugPlane->build(cmd, m_debugShader);
    }
}

void AssimpModelSample::render() {
    updateCamera();
    m_cubeUniform->update();
    m_shadowCamera->update();
}

void AssimpModelSample::setDescriptorSet() {
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
    m_vulkanDescriptorSet->addBinding(4, &(m_planeTexture->descriptor),
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      VK_SHADER_STAGE_FRAGMENT_BIT,
                                      0);
    m_vulkanDescriptorSet->addBinding(5, &(m_frameBuffer->getDescriptor()),
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      VK_SHADER_STAGE_FRAGMENT_BIT,
                                      0);
    m_vulkanDescriptorSet->addBinding(6, &(m_shadowCamera->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                      VK_SHADER_STAGE_VERTEX_BIT,
                                      0);
    m_vulkanDescriptorSet->addBinding(7, &(m_reflectParaBuffer->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                      VK_SHADER_STAGE_VERTEX_BIT,
                                      0);
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void AssimpModelSample::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_cubeShader);
    m_pipelines->createPipeline(m_skyShader);
    m_pipelines->createPipeline(m_planeShader);
    m_pipelines->createPipeline(m_debugShader);
    m_pipelines->createPipeline(m_shadowShader, m_frameBuffer->getRenderPass()->get());
}

void AssimpModelSample::createCube() {
    REGISTER_OBJECT<AssimpObject>(m_assimpObject);
    m_assimpObject->setModelPath(FS::getPath("models/sphere.obj"));
    m_assimpObject->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_cubeShader);
    m_cubeShader->setShaderObjPath(FS::getPath("shaders/ShadowMapping/scene.so.vert"),
                                   FS::getPath("shaders/ShadowMapping/scene.so.frag"));
    m_cubeShader->setCullFlag(VK_CULL_MODE_BACK_BIT);
    m_cubeShader->setFrontFace(VK_FRONT_FACE_CLOCKWISE);
    m_cubeShader->prepare();

    REGISTER_OBJECT<UniformCamera>(m_cubeUniform);
    m_cubeUniform->m_uboVS.lightpos = glm::vec4(10.0f, -10.0f, 10.0f, 1.0f);
    m_cubeUniform->m_pCameraPos = &m_cameraPos;
    m_cubeUniform->m_pRotation = &m_rotation;
    m_cubeUniform->m_pZoom = &m_zoom;
    m_cubeUniform->prepare();

    REGISTER_OBJECT<VulkanTexture2D>(m_cubeTextureA);
    m_cubeTextureA->loadFromFile(FS::getPath("textures/awesomeface.png"), VK_FORMAT_R8G8B8A8_UNORM);

    REGISTER_OBJECT<VulkanTexture2D>(m_cubeTextureB);
    m_cubeTextureB->loadFromFile(FS::getPath("textures/container.png"), VK_FORMAT_R8G8B8A8_UNORM);
}

void AssimpModelSample::createSkybox() {
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

void AssimpModelSample::createPlane() {
    REGISTER_OBJECT(m_plane);
    m_plane->setSize(10.f, 10.f);
    m_plane->setPosOffset({0.f, 1.f, 0.f});
    m_plane->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_planeShader);
    m_planeShader->setShaderObjPath(FS::getPath("shaders/ShadowMapping/plane.so.vert"),
                                    FS::getPath("shaders/ShadowMapping/plane.so.frag"));
    m_planeShader->setCullFlag(VK_CULL_MODE_BACK_BIT);
    m_planeShader->prepare();

    REGISTER_OBJECT<VulkanTexture2D>(m_planeTexture);
    m_planeTexture->loadFromFile(FS::getPath("textures/wood.png"), VK_FORMAT_R8G8B8A8_UNORM);
}

void AssimpModelSample::createShadowFrameBuffer() {
    m_frameBuffer = std::make_shared<VulkanFrameBuffer>();
    m_frameBuffer->setVulkanDevice(m_vulkanDevice);
    m_frameBuffer->setFormat(VK_FORMAT_D16_UNORM);
    m_frameBuffer->setSize(4096, 4096);
    m_frameBuffer->createWithDepth();

    REGISTER_OBJECT<VulkanVertFragShader>(m_shadowShader);
    m_shadowShader->setShaderObjPath(FS::getPath("shaders/ShadowMapping/shadow.so.vert"),
                                     FS::getPath("shaders/ShadowMapping/shadow.so.frag"));
    m_shadowShader->setCullFlag(VK_CULL_MODE_FRONT_BIT);
    m_shadowShader->setDepthBiasEnable(true);
    m_shadowShader->setOneStage(true);
    m_shadowShader->prepare();

    REGISTER_OBJECT<ShadowCamera>(m_shadowCamera);
    m_shadowCamera->m_lightPos = m_cubeUniform->m_uboVS.lightpos;
    m_shadowCamera->prepare();
}

void AssimpModelSample::createDebugQuad() {
    REGISTER_OBJECT(m_dubugPlane);
    m_dubugPlane->setSize(0.5, 0.5);
    m_dubugPlane->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_debugShader);
    m_debugShader->setShaderObjPath(FS::getPath("shaders/ShadowMapping/quad.so.vert"),
                                    FS::getPath("shaders/ShadowMapping/quad.so.frag"));
    m_debugShader->setCullFlag(VK_CULL_MODE_NONE);
    m_debugShader->prepare();
}

void AssimpModelSample::createReflectParaBuffer() {
    REGISTER_OBJECT(m_reflectParaBuffer);
    m_reflectParaBuffer->m_para.para[0] = 1.f;
    m_reflectParaBuffer->prepare();
}

void AssimpModelSample::buildCommandBuffersBeforeMainRenderPass(VkCommandBuffer &cmd) {
    VkClearValue clearValues[2];
    clearValues[0].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
    renderPassBeginInfo.renderPass = m_frameBuffer->getRenderPass()->get();
    renderPassBeginInfo.framebuffer = m_frameBuffer->get();
    renderPassBeginInfo.renderArea.extent.width = m_frameBuffer->getWidth();
    renderPassBeginInfo.renderArea.extent.height = m_frameBuffer->getHeight();
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = clearValues;

    vkCmdBeginRenderPass(cmd, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    auto viewport = vks::initializers::viewport((float)m_frameBuffer->getWidth(), (float)m_frameBuffer->getHeight(), 0.0f, 1.0f);
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    auto scissor = vks::initializers::rect2D(m_frameBuffer->getWidth(), m_frameBuffer->getHeight(), 0, 0);
    vkCmdSetScissor(cmd, 0, 1, &scissor);

    // Set depth bias (aka "Polygon offset")
    // Required to avoid shadow mapping artifacts

    // Constant depth bias factor (always applied)
    float depthBiasConstant = 1.25f;
    // Slope depth bias factor, applied depending on polygon's slope
    float depthBiasSlope = 1.75f;

    vkCmdSetDepthBias(
            cmd,
            depthBiasConstant,
            0.0f,
            depthBiasSlope);

    //vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.offscreen);
    VkDeviceSize offsets[1] = {0};
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1,
                            &(m_vulkanDescriptorSet->get(0)), 0, NULL);

    m_assimpObject->build(cmd, m_shadowShader);
    m_plane->build(cmd, m_shadowShader);

    vkCmdEndRenderPass(cmd);
}

void AssimpModelSample::seeDebugQuad() {
    m_seeDebug = !m_seeDebug;
    m_rebuild = true;
}

#ifndef __ANDROID__
void AssimpModelSample::OnUpdateUIOverlay(vks::UIOverlay *overlay) {
    if(m_UIOverlay.button("seeDebugQuad")){
        seeDebugQuad();
    }
    if(m_UIOverlay.sliderFloat("reflect", &m_reflectParaBuffer->m_para.para[0], 0.f, 1.f)){
        m_reflectParaBuffer->update();
    }
}
#endif

END_NAMESPACE(VulkanEngine)