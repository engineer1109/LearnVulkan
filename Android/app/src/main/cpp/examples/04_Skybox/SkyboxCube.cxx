//
// Created by wjl on 21-3-28.
//

#include "SkyboxCube.h"

BEGIN_NAMESPACE(VulkanEngine)

SkyboxCube::~SkyboxCube() {}

void SkyboxCube::prepareMyObjects() {
    m_zoom = -4.f;

    createCube();
    createSkybox();

    setDescriptorSet();
    createPipelines();
}

void SkyboxCube::buildMyObjects(VkCommandBuffer &cmd) {

}

void SkyboxCube::render() {

}

void SkyboxCube::setDescriptorSet() {

}

void SkyboxCube::createPipelines() {

}

void SkyboxCube::createCube() {

}

void SkyboxCube::createSkybox() {

}

END_NAMESPACE(VulkanEngine)