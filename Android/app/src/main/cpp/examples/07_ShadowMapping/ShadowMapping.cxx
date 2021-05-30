//
// Created by wjl on 21-5-17.
//

#include "ShadowMapping.h"

#include "VulkanCube.h"
#include "VulkanVertFragShader.h"
#include "UniformCamera.h"
#include "VulkanTexture2D.h"
#include "VulkanTextureCubeMap.h"
#include "VulkanPlane.h"

#include "VulkanFrameBuffer.h"
#include "ShadowCamera.h"

BEGIN_NAMESPACE(VulkanEngine)

ShadowMapping::~ShadowMapping() noexcept {
    delete_ptr(m_frameBuffer);
    destroyObjects();
}

void ShadowMapping::prepareFunctions() {}

void ShadowMapping::prepareMyObjects() {
    m_zoom = -4.f;

    createCube();
    createSkybox();
    createPlane();
    createShadowFrameBuffer();

    setDescriptorSet();
    createPipelines();
}

void ShadowMapping::buildMyObjects(VkCommandBuffer &cmd) {
    m_cube->build(cmd, m_cubeShader);
    m_sky->build(cmd, m_skyShader);
    m_plane->build(cmd, m_planeShader);
}

void ShadowMapping::render() {
    updateCamera();
    m_cubeUniform->update();
}

void ShadowMapping::setDescriptorSet() {
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
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void ShadowMapping::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_cubeShader);
    m_pipelines->createPipeline(m_skyShader);
    m_pipelines->createPipeline(m_planeShader);
    m_pipelines->createPipeline(m_shadowShader);
}

void ShadowMapping::createCube() {
    REGISTER_OBJECT<VulkanCube>(m_cube);
    m_cube->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_cubeShader);
    m_cubeShader->setShaderObjPath(FS::getPath("shaders/ShadowMapping/scene.so.vert"),
                                   FS::getPath("shaders/ShadowMapping/scene.so.frag"));
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

void ShadowMapping::createSkybox() {
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

void ShadowMapping::createPlane() {
    REGISTER_OBJECT(m_plane);
    m_plane->setSize(5.f, 5.f);
    m_plane->setPosOffset({0.f, 1.f, 0.f});
    m_plane->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_planeShader);
    m_planeShader->setShaderObjPath(FS::getPath("shaders/ShadowMapping/plane.so.vert"),
                                    FS::getPath("shaders/ShadowMapping/plane.so.frag"));
    m_planeShader->setCullFlag(VK_CULL_MODE_NONE);
    m_planeShader->prepare();

    REGISTER_OBJECT<VulkanTexture2D>(m_planeTexture);
    m_planeTexture->loadFromFile(FS::getPath("textures/wood.png"), VK_FORMAT_R8G8B8A8_UNORM);
}

void ShadowMapping::createShadowFrameBuffer() {
    m_frameBuffer = new VulkanFrameBuffer();
    m_frameBuffer->setVulkanDevice(m_vulkanDevice);
    m_frameBuffer->setFormat(VK_FORMAT_D16_UNORM);
    m_frameBuffer->setSize(2048, 2048);
    m_frameBuffer->create();

    REGISTER_OBJECT<VulkanVertFragShader>(m_shadowShader);
    m_shadowShader->setShaderObjPath(FS::getPath("shaders/ShadowMapping/shadow.so.vert"),
                                    FS::getPath("shaders/ShadowMapping/shadow.so.frag"));
    m_shadowShader->setCullFlag(VK_CULL_MODE_NONE);
    m_shadowShader->prepare();

    REGISTER_OBJECT<ShadowCamera>(m_shadowCamera);
    m_shadowCamera->prepare();
}

void ShadowMapping::buildCommandBuffers() {
    VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

    for (size_t i = 0; i < m_drawCmdBuffers.size(); ++i) {

        VK_CHECK_RESULT(vkBeginCommandBuffer(m_drawCmdBuffers[i], &cmdBufInfo));

        {
            VkClearValue clearValues[2];
            clearValues[0].depthStencil = { 1.0f, 0 };

            VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
            renderPassBeginInfo.renderPass = m_frameBuffer->getRenderPass()->get();
            renderPassBeginInfo.framebuffer = m_frameBuffer->get();
            renderPassBeginInfo.renderArea.extent.width = m_frameBuffer->getWidth();
            renderPassBeginInfo.renderArea.extent.height = m_frameBuffer->getHeight();
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues = clearValues;

            vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            auto viewport = vks::initializers::viewport((float)m_frameBuffer->getWidth(), (float)m_frameBuffer->getHeight(), 0.0f, 1.0f);
            vkCmdSetViewport(m_drawCmdBuffers[i], 0, 1, &viewport);

            auto scissor = vks::initializers::rect2D(m_frameBuffer->getWidth(), m_frameBuffer->getHeight(), 0, 0);
            vkCmdSetScissor(m_drawCmdBuffers[i], 0, 1, &scissor);

            // Set depth bias (aka "Polygon offset")
            // Required to avoid shadow mapping artifacts

            // Constant depth bias factor (always applied)
            float depthBiasConstant = 1.25f;
            // Slope depth bias factor, applied depending on polygon's slope
            float depthBiasSlope = 1.75f;

            vkCmdSetDepthBias(
                    m_drawCmdBuffers[i],
                    depthBiasConstant,
                    0.0f,
                    depthBiasSlope);

            //vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.offscreen);
            VkDeviceSize offsets[1] = {0};
            vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1,
                                    &(m_vulkanDescriptorSet->get(0)), 0, NULL);
            //scenes[sceneIndex].draw(m_drawCmdBuffers[i]);
            m_cube->build(m_drawCmdBuffers[i], m_shadowShader);
            m_plane->build(m_drawCmdBuffers[i], m_shadowShader);

            vkCmdEndRenderPass(m_drawCmdBuffers[i]);
        }

        {
            VkClearValue clearValues[2];
            clearValues[0].color = {{0.1f, 0.2f, 0.3f, 1.0f}};
            clearValues[1].depthStencil = {1.0f, 0};

            VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
            renderPassBeginInfo.renderPass = m_renderPass;
            renderPassBeginInfo.renderArea.offset.x = 0;
            renderPassBeginInfo.renderArea.offset.y = 0;
            renderPassBeginInfo.renderArea.extent.width = m_width;
            renderPassBeginInfo.renderArea.extent.height = m_height;
            renderPassBeginInfo.clearValueCount = 2;
            renderPassBeginInfo.pClearValues = clearValues;

            renderPassBeginInfo.framebuffer = m_frameBuffers[i];

            vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkDeviceSize offsets[1] = {0};
            vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1,
                                    &(m_vulkanDescriptorSet->get(0)), 0, NULL);

            setViewPorts(m_drawCmdBuffers[i]);
            buildMyObjects(m_drawCmdBuffers[i]);

            vkCmdEndRenderPass(m_drawCmdBuffers[i]);
        }
    }
    vkQueueWaitIdle(m_queue);
}

END_NAMESPACE(VulkanEngine)