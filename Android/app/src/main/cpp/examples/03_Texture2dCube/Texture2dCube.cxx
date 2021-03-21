//
// Created by wjl on 21-3-22.
//

#include "Texture2dCube.h"

#include "VulkanCube.h"

BEGIN_NAMESPACE(VulkanEngine)

Texture2dCube::~Texture2dCube(){

}

void Texture2dCube::prepareMyObjects() {}

void Texture2dCube::buildMyObjects(VkCommandBuffer &cmd) {}

void Texture2dCube::render() {}

void Texture2dCube::setDescriptorSet() {}

void Texture2dCube::createPipelines() {}

void Texture2dCube::createCube() {
    m_cube = VkObject::New<VulkanCube>(m_context);
    m_cube->prepare();
}

END_NAMESPACE(VulkanEngine)