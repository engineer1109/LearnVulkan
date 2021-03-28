//
// Created by wjl on 21-2-26.
//

#include "VulkanPipelines.h"

BEGIN_NAMESPACE(VulkanEngine)

VulkanPipelines::VulkanPipelines(VkDevice &device) {
    m_device = device;
}

void VulkanPipelines::createBasePipelineInfo(const VkPipelineLayout &pipelineLayout,
                                             const VkRenderPass &renderPass) {
    m_inputAssemblyState =
            vks::initializers::pipelineInputAssemblyStateCreateInfo(
                    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);

    m_rasterizationState = vks::initializers::pipelineRasterizationStateCreateInfo(
            VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, 0);

    m_blendAttachmentState = vks::initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);

    m_colorBlendState = vks::initializers::pipelineColorBlendStateCreateInfo(1,
                                                                             &m_blendAttachmentState);

    m_depthStencilState =
            vks::initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE,
                                                                   VK_COMPARE_OP_LESS_OR_EQUAL);

    m_viewportState = vks::initializers::pipelineViewportStateCreateInfo(5, 5, 0);

    m_multisampleState = vks::initializers::pipelineMultisampleStateCreateInfo(
            VK_SAMPLE_COUNT_1_BIT, 0);
    m_multisampleState.sampleShadingEnable = VK_TRUE;
    m_multisampleState.minSampleShading = 0.2f;

    m_dynamicState = vks::initializers::pipelineDynamicStateCreateInfo(
            m_dynamicStateEnables.data(), static_cast<uint32_t>(m_dynamicStateEnables.size()), 0);

    m_pipelineCreateInfo = vks::initializers::pipelineCreateInfo(pipelineLayout, renderPass, 0);

    m_pipelineCreateInfo.pVertexInputState = &m_vertexInputState;
    m_pipelineCreateInfo.pInputAssemblyState = &m_inputAssemblyState;
    m_pipelineCreateInfo.pRasterizationState = &m_rasterizationState;
    m_pipelineCreateInfo.pColorBlendState = &m_colorBlendState;
    m_pipelineCreateInfo.pMultisampleState = &m_multisampleState;
    m_pipelineCreateInfo.pViewportState = &m_viewportState;
    m_pipelineCreateInfo.pDepthStencilState = &m_depthStencilState;
    m_pipelineCreateInfo.pDynamicState = &m_dynamicState;
    m_pipelineCreateInfo.stageCount = static_cast<uint32_t>(m_shaderStages.size());
    m_pipelineCreateInfo.pStages = m_shaderStages.data();

    m_blendAttachmentState.blendEnable = VK_TRUE;
    m_blendAttachmentState.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
    m_blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    m_blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    m_blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    m_blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    m_blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    m_blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
}

void VulkanPipelines::createPipeline(VulkanShader *shader, VkPolygonMode mode) {
    if (shader) {
        if (shader->getPipeLine()) {
            vkDestroyPipeline(m_device, shader->getPipeLine(), nullptr);
        }
        m_rasterizationState.polygonMode = mode;
        m_rasterizationState.cullMode = shader->getCullFlag();
        m_shaderStages[0] = shader->getShaderStages()[0];
        m_shaderStages[1] = shader->getShaderStages()[1];
        VK_CHECK_RESULT(
                vkCreateGraphicsPipelines(m_device, m_pipelineCache, 1, &m_pipelineCreateInfo,
                                          nullptr,
                                          &(shader->getPipeLine())));
    }
}

END_NAMESPACE(VulkanEngine)
