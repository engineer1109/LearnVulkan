//
// Created by wjl on 21-2-27.
//

#include "VulkanShader.h"

BEGIN_NAMESPACE(VulkanEngine)

VulkanShader::~VulkanShader() {
    VK_SAFE_DELETE(m_pipeline, vkDestroyPipeline(m_context->getDevice(), m_pipeline, nullptr));
    for (auto &shaderModule : m_shaderModules) {
        VK_SAFE_DELETE(shaderModule,
                       vkDestroyShaderModule(m_context->getDevice(), shaderModule, nullptr));
    }
}

void VulkanShader::prepare() {
    prepareShaders();
}

void VulkanShader::update() {}

VkPipelineShaderStageCreateInfo
VulkanShader::loadShader(const std::string &fileName, const VkShaderStageFlagBits &stage) {
    VkPipelineShaderStageCreateInfo shaderStage = {};
    shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.stage = stage;
    shaderStage.module = vks::tools::loadShader(m_context->m_asset, fileName.c_str(), m_context->getDevice());
    shaderStage.pName = "main"; // todo : make param
    assert(shaderStage.module != VK_NULL_HANDLE);
    m_shaderModules.push_back(shaderStage.module);
    return shaderStage;
}

END_NAMESPACE()
