//
// Created by wjl on 21-2-26.
//

#ifndef LEARNVULKAN_VULKANPIPELINES_H
#define LEARNVULKAN_VULKANPIPELINES_H

#include "render_common.h"

#include "object/VulkanShader.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API VulkanPipelines {
public:
    VulkanPipelines(VkDevice &device);
    ~VulkanPipelines() = default;

    void createBasePipelineInfo(const VkPipelineLayout &pipelineLayout, const VkRenderPass &renderPass);

    void createPipeline(VulkanShader* shader, VkPolygonMode mode= VK_POLYGON_MODE_FILL);

    void createPipeline(VulkanShader* shader, VkRenderPass renderPass, VkPolygonMode mode= VK_POLYGON_MODE_FILL);

    void createPipeline(std::shared_ptr<VulkanShader> shader, VkPolygonMode mode= VK_POLYGON_MODE_FILL) {
        this->createPipeline(shader.get(), mode);
    }

    void createPipeline(std::shared_ptr<VulkanShader> shader, VkRenderPass renderPass, VkPolygonMode mode= VK_POLYGON_MODE_FILL) {
        this->createPipeline(shader.get(), renderPass, mode);
    }

public:
    VkDevice m_device;
    VkGraphicsPipelineCreateInfo m_pipelineCreateInfo;
    VkPipelineVertexInputStateCreateInfo m_vertexInputState;
    VkPipelineCache m_pipelineCache;

    VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
    VkPipelineRasterizationStateCreateInfo m_rasterizationState;
    VkPipelineColorBlendAttachmentState m_blendAttachmentState;
    VkPipelineColorBlendStateCreateInfo m_colorBlendState;
    VkPipelineDepthStencilStateCreateInfo m_depthStencilState;
    VkPipelineViewportStateCreateInfo m_viewportState;
    VkPipelineMultisampleStateCreateInfo m_multisampleState;
    std::vector<VkDynamicState> m_dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR,
                                                         VK_DYNAMIC_STATE_LINE_WIDTH};
    VkPipelineDynamicStateCreateInfo m_dynamicState;

    std::array<VkPipelineShaderStageCreateInfo, 2> m_shaderStages;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANPIPELINES_H
