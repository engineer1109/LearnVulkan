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

BEGIN_NAMESPACE(VulkanEngine)

class VulkanBaseEngine : public VulkanBase {
public:
    VulkanBaseEngine() = default;

    virtual ~VulkanBaseEngine();

    virtual void prepare() override;

    virtual void render() override;

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
    void REGISTER_OBJECT(T* &obj){
        obj = VkObject::New<T>(m_context);
        m_objs.push_back((VkObject**)&obj);
    }

    void destroyObjects(){
        for (auto &obj : m_objs){
            delete_ptr(*obj);
        }
    }

protected:
    int m_maxSets = 1;

    VulkanDescriptorSet *m_vulkanDescriptorSet = nullptr;
    VulkanVertexDescriptions *m_vulkanVertexDescriptions = nullptr;
    VulkanPipelines *m_pipelines = nullptr;
    VulkanContext *m_context = nullptr;
    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

    std::vector<VkObject**> m_objs;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANBASEENGINE_H
