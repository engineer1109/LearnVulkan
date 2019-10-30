#include "sky.h"
void Sky::create(){
    prepareTextures();
    prepareAssets();
    prepareUniformBuffers();
}

void Sky::prepareTextures(){
    std::vector<std::string> jpgPaths;
    jpgPaths.resize(6);
    jpgPaths[0]="../data/textures/skybox/back.jpg";
    jpgPaths[1]="../data/textures/skybox/front.jpg";
    jpgPaths[2]="../data/textures/skybox/bottom.jpg";
    jpgPaths[3]="../data/textures/skybox/top.jpg";
    jpgPaths[4]="../data/textures/skybox/right.jpg";
    jpgPaths[5]="../data/textures/skybox/left.jpg";
    m_textureSkybox.loadFromFile(jpgPaths,VK_FORMAT_R8G8B8A8_UNORM,m_vulkanDevice,m_queue);
}

void Sky::prepareAssets(){
    m_model.loadFromFile("../data/models/cube.obj",m_vertexLayout, 0.05f, m_vulkanDevice, m_queue);
}

void Sky::prepareUniformBuffers(){
    // Skybox vertex shader uniform buffer
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_uniformBuffers,
        sizeof(m_uboVS)));

    // Map persistent
    VK_CHECK_RESULT(m_uniformBuffers.map());
    updateUniformBuffers();
}

void Sky::updateUniformBuffers(){
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    m_uboVS.projection = glm::perspective(glm::radians(60.0f), float(*m_screenWidth) / float(*m_screenHeight), 0.001f, 256.0f);

    m_uboVS.model = glm::mat4(1.0f);
    m_uboVS.model = viewMatrix * glm::translate(m_uboVS.model, glm::vec3(0, 0, 0));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->z), glm::vec3(0.0f, 0.0f, 1.0f));

    memcpy(m_uniformBuffers.mapped, &m_uboVS, sizeof(m_uboVS));
}

