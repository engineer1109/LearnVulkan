//
// Created by wjl on 21-2-27.
//

#ifndef LEARNVULKAN_VULKANSHADER_H
#define LEARNVULKAN_VULKANSHADER_H

#include "VkObject.h"
#include "VulkanContext.h"

BEGIN_NAMESPACE(VulkanEngine)

class VulkanShader : public VkObject {
public:
    VulkanShader() = default;

    virtual ~VulkanShader();

    virtual void prepare() override;

    virtual void update() override;

    virtual void prepareShaders() = 0;

    VkPipeline &getPipeLine() { return m_pipeline; }

    std::vector<VkPipelineShaderStageCreateInfo> &getShaderStages() {return m_shaderStages;}

    void setCullFlag(VkCullModeFlags flag) { m_cullFlag = flag; }

    VkCullModeFlags getCullFlag() const { return m_cullFlag;}

protected:
    VkPipelineShaderStageCreateInfo
    loadShader(const std::string &fileName, const VkShaderStageFlagBits &stage);

protected:
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
    std::vector<VkShaderModule> m_shaderModules;

    VkCullModeFlags m_cullFlag = VK_CULL_MODE_NONE;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANSHADER_H
