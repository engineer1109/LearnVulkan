/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* Skybox of cubemaps using my vulkanengine template.
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef SKYBOX_H
#define SKYBOX_H
#include "VulkanModel.hpp"
#include "vulkan_basicengine.h"
#include "vulkan_basicengine_texture.h"

class Skybox:public VulkanBasicEngine{
public:
    Skybox(bool debugLayer=false);
    ~Skybox();

    virtual void prepare();
    virtual void render();
    virtual void draw();

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);

private:
    void prepareTextures();
    void prepareAssets();
    void prepareUniformBuffers();
    void updateUniformBuffers();
    void setupDescriptorPool();
    void setupDescriptorSetLayout();
    void setupDescriptorSets();
    void preparePipelines();
    void buildCommandBuffers();

private:
    vks::Buffer m_uniformBuffers;

    struct UBOVS {
        glm::mat4 projection;
        glm::mat4 model;
    } m_uboVS;

    vks::VertexLayout m_vertexLayout = vks::VertexLayout({
        vks::VERTEX_COMPONENT_POSITION,
        vks::VERTEX_COMPONENT_NORMAL,
        vks::VERTEX_COMPONENT_UV,
    });
    std::vector<std::string> m_skyboxPath;
    vks::TextureCubeMapStbImage m_textureSkybox;
    vks::Model m_skyboxModel;
    VkDescriptorSet m_descriptorSet=VK_NULL_HANDLE;
    VkPipeline m_pipeline=VK_NULL_HANDLE;

    bool m_displaySkybox=true;
};


#endif
