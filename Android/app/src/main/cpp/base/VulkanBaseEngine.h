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

#ifdef __ANDROID__
namespace vks{
class UIOverlay;
}
#else
#include "VulkanUIOverlay.h"
#endif

BEGIN_NAMESPACE(VulkanEngine)

class VulkanBaseEngine : public VulkanBase {
public:
    VulkanBaseEngine() = default;

    virtual ~VulkanBaseEngine();

    virtual void prepare() override;

    virtual void render() override;

    virtual void updateCommand() override;

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay) {}

    virtual void processPrepareCallback(){}

protected:
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
    void REGISTER_OBJECT(std::shared_ptr<T> &obj) {
        obj = VkObject::New<T>(m_context);
        m_objs.emplace_back(obj);
    }

    void destroyObjects(){
        for (auto &obj : m_objs){
            obj = nullptr;
        }
    }

protected:
    int m_maxSets = 1;

    VulkanDescriptorSet *m_vulkanDescriptorSet = nullptr;
    VulkanVertexDescriptions *m_vulkanVertexDescriptions = nullptr;
    VulkanPipelines *m_pipelines = nullptr;
    VulkanContext *m_context = nullptr;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

    std::vector<std::shared_ptr<VkObject>> m_objs;

    bool m_rebuild = false;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANBASEENGINE_H
