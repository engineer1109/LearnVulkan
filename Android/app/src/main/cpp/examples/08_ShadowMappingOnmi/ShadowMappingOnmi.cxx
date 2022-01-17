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
#include "OnmiCamera.h"

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
    createDebugQuad();

    setDescriptorSet();
    createPipelines();
}

void ShadowMappingOnmi::buildMyObjects(VkCommandBuffer &cmd) {
    for (auto &cube: m_cubes) {
        cube->build(cmd, m_cubeShader);
    }
    m_sky->build(cmd, m_skyShader);
    m_room->build(cmd, m_roomShader);
    if(m_displayDebugQuad) {
        m_dubugPlane->build(cmd, m_debugShader);
    }
}

void ShadowMappingOnmi::render() {
    updateCamera();
    m_cubeUniform->update();
    m_onmiCamera->update();
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
    m_vulkanDescriptorSet->addBinding(8, &(m_shadowMapTexture->descriptor),
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      VK_SHADER_STAGE_FRAGMENT_BIT,
                                      0);
    m_vulkanDescriptorSet->addBinding(9, &(m_frameBuffer->getDescriptor()),
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      VK_SHADER_STAGE_FRAGMENT_BIT,
                                      0);
    m_vulkanDescriptorSet->addBinding(10, &(m_onmiCamera->m_uniformBuffer.descriptor),
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
    m_pipelines->createPipeline(m_debugShader);
    m_pipelines->createPipeline(m_shadowShader);
}

void ShadowMappingOnmi::createCube() {
    m_cubes.resize(m_cubeNum);
    int index = 0;
    for (auto &cube : m_cubes) {
        REGISTER_OBJECT<VulkanCube>(cube);
        cube->setPosOffset({-3 + index * 2, -1 - index * 2 , 0 + index * 2 });
        cube->prepare();
        index++;
    }

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
    m_skyShader->setCullFlag(VK_CULL_MODE_NONE);
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

    m_frameBuffer = std::make_shared<VulkanFrameBuffer>();
    m_frameBuffer->setContext(m_context);
    m_frameBuffer->setFormat(VK_FORMAT_R32_SFLOAT);
    m_frameBuffer->setSize(1024, 1024);
    m_frameBuffer->createWithColorDepth();

    REGISTER_OBJECT<VulkanVertFragShader>(m_shadowShader);
    m_shadowShader->setShaderObjPath(FS::getPath("shaders/ShadowMappingOnmi/offscreen.so.vert"),
                                     FS::getPath("shaders/ShadowMappingOnmi/offscreen.so.frag"));
    m_shadowShader->setCullFlag(VK_CULL_MODE_NONE);
    m_shadowShader->prepare();

    REGISTER_OBJECT<OnmiCamera>(m_onmiCamera);
    m_onmiCamera->m_mvp.lightpos = {0, 0, 0, 1};
    m_onmiCamera->prepare();
}

void ShadowMappingOnmi::createDebugQuad() {
    REGISTER_OBJECT(m_dubugPlane);
    m_dubugPlane->setSize(0.5, 0.5);
    m_dubugPlane->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_debugShader);
    m_debugShader->setShaderObjPath(FS::getPath("shaders/ShadowMappingOnmi/cubemapdisplay.so.vert"),
                                    FS::getPath("shaders/ShadowMappingOnmi/cubemapdisplay.so.frag"));
    m_debugShader->setCullFlag(VK_CULL_MODE_NONE);
    m_debugShader->prepare();
}

void ShadowMappingOnmi::updateCubeFace(uint32_t faceIndex, VkCommandBuffer &commandBuffer) {
    VkClearValue clearValues[2];
    clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
    clearValues[1].depthStencil = { 1.0f, 0 };

    VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
    // Reuse render pass from example pass
    renderPassBeginInfo.renderPass = m_frameBuffer->m_pRenderPass;
    renderPassBeginInfo.framebuffer = m_frameBuffer->get();
    renderPassBeginInfo.renderArea.extent.width = m_frameBuffer->getWidth();
    renderPassBeginInfo.renderArea.extent.height = m_frameBuffer->getHeight();
    renderPassBeginInfo.clearValueCount = 2;
    renderPassBeginInfo.pClearValues = clearValues;

    auto viewport = vks::initializers::viewport((float)m_frameBuffer->getWidth(), (float)m_frameBuffer->getHeight(), 0.0f, 1.0f);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    auto scissor = vks::initializers::rect2D(m_frameBuffer->getWidth(), m_frameBuffer->getHeight(), 0, 0);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // Update view matrix via push constant

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    switch (faceIndex)
    {
        case 0: // POSITIVE_X
            viewMatrix = glm::rotate(viewMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            viewMatrix = glm::rotate(viewMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 1:	// NEGATIVE_X
            viewMatrix = glm::rotate(viewMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            viewMatrix = glm::rotate(viewMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 2:	// POSITIVE_Y
            viewMatrix = glm::rotate(viewMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 3:	// NEGATIVE_Y
            viewMatrix = glm::rotate(viewMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 4:	// POSITIVE_Z
            viewMatrix = glm::rotate(viewMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 5:	// NEGATIVE_Z
            viewMatrix = glm::rotate(viewMatrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            break;
    }

    // Render scene from cube face's point of view
    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Update shader push constant block
    // Contains current face view matrix
    vkCmdPushConstants(
            commandBuffer,
            m_pipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT,
            0,
            sizeof(glm::mat4),
            &viewMatrix);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &(m_vulkanDescriptorSet->get(0)), 0, NULL);
    for(auto &cube : m_cubes) {
        cube->build(commandBuffer, m_shadowShader);
    }
    m_room->build(commandBuffer, m_shadowShader);

    vkCmdEndRenderPass(commandBuffer);
//    // Make sure color writes to the framebuffer are finished before using it as transfer source
    vks::tools::setImageLayout(
            commandBuffer,
            m_frameBuffer->getColorAttachment().image,
            VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

    VkImageSubresourceRange cubeFaceSubresourceRange = {};
    cubeFaceSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    cubeFaceSubresourceRange.baseMipLevel = 0;
    cubeFaceSubresourceRange.levelCount = 1;
    cubeFaceSubresourceRange.baseArrayLayer = faceIndex;
    cubeFaceSubresourceRange.layerCount = 1;

    // Change image layout of one cubemap face to transfer destination
    vks::tools::setImageLayout(
            commandBuffer,
            m_shadowMapTexture->image,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            cubeFaceSubresourceRange);

    // Copy region for transfer from framebuffer to cube face
    VkImageCopy copyRegion = {};

    copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.srcSubresource.baseArrayLayer = 0;
    copyRegion.srcSubresource.mipLevel = 0;
    copyRegion.srcSubresource.layerCount = 1;
    copyRegion.srcOffset = { 0, 0, 0 };

    copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.dstSubresource.baseArrayLayer = faceIndex;
    copyRegion.dstSubresource.mipLevel = 0;
    copyRegion.dstSubresource.layerCount = 1;
    copyRegion.dstOffset = { 0, 0, 0 };

    copyRegion.extent.width = m_shadowMapTexture->width;
    copyRegion.extent.height = m_shadowMapTexture->height;
    copyRegion.extent.depth = 1;

    // Put image copy into command buffer
    vkCmdCopyImage(
            commandBuffer,
            m_frameBuffer->getColorAttachment().image,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            m_shadowMapTexture->image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &copyRegion);

    // Transform framebuffer color attachment back
    vks::tools::setImageLayout(
            commandBuffer,
            m_frameBuffer->getColorAttachment().image,
            VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

    // Change image layout of copied face to shader read
    vks::tools::setImageLayout(
            commandBuffer,
            m_shadowMapTexture->image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            cubeFaceSubresourceRange);
}

void ShadowMappingOnmi::buildCommandBuffersBeforeMainRenderPass(VkCommandBuffer &cmd) {

    VkViewport viewport = vks::initializers::viewport(1024, 1024, 0.0f, 1.0f);
    vkCmdSetViewport(cmd, 0, 1, &viewport);

    VkRect2D scissor = vks::initializers::rect2D(1024, 1024, 0, 0);
    vkCmdSetScissor(cmd, 0, 1, &scissor);
    for (uint32_t face = 0; face < 6; face++) {
        updateCubeFace(face, cmd);
    }
//    updateCubeFace(1, cmd);
}

void ShadowMappingOnmi::seeDebugQuad() {
    m_displayDebugQuad = !m_displayDebugQuad;
    m_rebuild = true;
}

void ShadowMappingOnmi::OnUpdateUIOverlay(vks::UIOverlay *overlay) {
#ifndef __ANDROID__
    if(m_UIOverlay.button("seeDebugQuad")){
        seeDebugQuad();
    }
    if(m_UIOverlay.sliderFloat("reflect", &m_reflectParaBuffer->m_para.para[0], 0.f, 1.f)){
        m_reflectParaBuffer->update();
    }
#endif
}

END_NAMESPACE(VulkanEngine)