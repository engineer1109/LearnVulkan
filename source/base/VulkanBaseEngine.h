//
// Created by wjl on 21-2-23.
//

#ifndef LEARNVULKAN_VULKANBASEENGINE_H
#define LEARNVULKAN_VULKANBASEENGINE_H

#include "VulkanBase.h"

#include "VulkanContext.h"
#include "VulkanDescriptorSet.h"
#include "VulkanVertexDescriptions.h"
#include "VulkanPipelines.h"
#ifndef __ANDROID__
#include "VulkanUIOverlay.h"
#endif

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API VulkanBaseEngine : public VulkanBase {
public:
    VulkanBaseEngine() = default;

    virtual ~VulkanBaseEngine();

    virtual void initWindow();

    virtual void prepare() override;

    virtual void render() override;

    virtual void updateOverlay() override;

    virtual void drawUI(const VkCommandBuffer commandBuffer);

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay) {}

    virtual void processPrepareCallback() {}

    virtual void updateCommand();

protected:
    void prepareIMGUI();

    void prepareDescriptorSets();

    void prepareVertexDescriptions();

    void prepareBasePipelines();

    void prepareContext();

    virtual void prepareMyObjects() {};

    virtual void buildCommandBuffersBeforeMainRenderPass(VkCommandBuffer &cmd) {}

    virtual void buildCommandBuffers() override;

    virtual void buildCommandBuffersAfterMainRenderPass(VkCommandBuffer &cmd) {}

    virtual void setViewPorts(VkCommandBuffer &cmd);

    virtual void buildMyObjects(VkCommandBuffer &cmd) {};

    template<class T>
    void REGISTER_OBJECT(T *&obj) {
        obj = VkObject::New<T>(m_context);
        m_objs.push_back((VkObject **) &obj);
    }

    void destroyObjects() {
        for (auto &obj : m_objs) {
            delete_ptr(*obj);
        }
    }

    VkPipelineShaderStageCreateInfo
    loadShader(const std::string &fileName, const VkShaderStageFlagBits &stage) {
        VkPipelineShaderStageCreateInfo shaderStage = {};
        shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStage.stage = stage;
#if __ANDROID__
        shaderStage.module = vks::tools::loadShader(m_context->m_asset, fileName.c_str(), m_context->getDevice());
#else
        shaderStage.module = vks::tools::loadShader(fileName.c_str(), m_context->getDevice());
#endif
        shaderStage.pName = "main"; // todo : make param
        assert(shaderStage.module != VK_NULL_HANDLE);
        m_shaderModules.push_back(shaderStage.module);
        return shaderStage;
    }

protected:
    int m_maxSets = 1;

    VulkanDescriptorSet *m_vulkanDescriptorSet = nullptr;
    VulkanVertexDescriptions *m_vulkanVertexDescriptions = nullptr;
    VulkanPipelines *m_pipelines = nullptr;
    VulkanContext *m_context = nullptr;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

#ifndef __ANDROID__
    vks::UIOverlay m_UIOverlay;
#endif

    std::vector<VkObject **> m_objs;

    struct Settings {
        bool overlay = true;
    } m_settings;

    bool m_rebuild = false;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANBASEENGINE_H
