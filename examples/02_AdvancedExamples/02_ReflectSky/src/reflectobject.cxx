#include "reflectobject.h"
#define VERTEX_BUFFER_BIND_ID 0
ReflectObject::~ReflectObject(){
    vkDestroyPipeline(m_device, m_pipeline, nullptr);
    m_model.destroy();
    m_uniformBuffers.destroy();
}

void ReflectObject::create(){
    loadModel();
    prepareUniformBuffers();
}

void ReflectObject::build(VkCommandBuffer cmd){
    VkDeviceSize offsets[1] = { 0 };
    //vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, NULL);
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    vkCmdBindVertexBuffers(cmd, VERTEX_BUFFER_BIND_ID, 1,&m_model.vertices.buffer, offsets);
    vkCmdBindIndexBuffer(cmd, m_model.indices.buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd, m_model.indexCount, 1, 0, 0, 0);
}

void ReflectObject::update(){
    updateUniformBuffers();
}

void ReflectObject::loadModel(){
    m_model.loadFromFile("../data/models/sphere.obj",m_vertexLayout,0.05f,m_vulkanDevice,m_queue);
}

void ReflectObject::prepareUniformBuffers(){
    // Objact vertex shader uniform buffer
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_uniformBuffers,
        sizeof(m_uboVS)));
    VK_CHECK_RESULT(m_uniformBuffers.map());
    updateUniformBuffers();
}

void ReflectObject::updateUniformBuffers(){
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    m_uboVS.projection = glm::perspective(glm::radians(60.0f), float(*m_screenWidth) / float(*m_screenHeight), 0.001f, 256.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, *m_camera.zoom));

    m_uboVS.model = glm::mat4(1.0f);
    m_uboVS.model = viewMatrix * glm::translate(m_uboVS.model, *m_camera.cameraPos);
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(m_camera.rotation->z), glm::vec3(0.0f, 0.0f, 1.0f));

    memcpy(m_uniformBuffers.mapped, &m_uboVS, sizeof(m_uboVS));
}
