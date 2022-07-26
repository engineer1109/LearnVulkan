//
// Created by wjl on 21-3-14.
//

#ifndef LEARNVULKAN_VULKANVERTFRAGSHADER_H
#define LEARNVULKAN_VULKANVERTFRAGSHADER_H

#include "VulkanShader.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API VulkanVertFragShader : public VulkanShader {
public:
    VulkanVertFragShader() = default;
    ~VulkanVertFragShader() = default;

    void setShaderObjPath(const std::string &vertPath, const std::string &fragPath){
        m_vertPath = vertPath;
        m_fragPath = fragPath;
    }

    virtual void prepareShaders() override{
        m_shaderStages.push_back(
                loadShader(m_vertPath.c_str(), VK_SHADER_STAGE_VERTEX_BIT));
        m_shaderStages.push_back(
                loadShader(m_fragPath.c_str(), VK_SHADER_STAGE_FRAGMENT_BIT));
    }

protected:
    std::string m_vertPath;
    std::string m_fragPath;

};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VULKANVERTFRAGSHADER_H
