/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "vulkan_basicengine_help.h"
#include "vulkan_basicengine_algorithm.h"
#include "texture3dsmoke.h"
Texture3dSmoke::Texture3dSmoke(uint32_t width,uint32_t height,uint32_t depth){
    m_width=width;
    m_height=height;
    m_depth=depth;
    m_imgData=new uint8_t[width*height*depth];
}

Texture3dSmoke::~Texture3dSmoke(){
    if(m_imgData){
        delete m_imgData;
        m_imgData=nullptr;
    }
}

void Texture3dSmoke::create(){
    generateVertex();
    prepareUniformBuffers();
    setupVertexDescriptions();
    generateNoise();
    prepareModelBuffer();
    prepareTransferFunctionImage();
}

void Texture3dSmoke::update(){
    updateModelBuffer();
    updateUniformBuffers();
}

void Texture3dSmoke::generateNoise(){
    VulkanTemplate::PerlinNoise<float> perlinNoise;
    VulkanTemplate::FractalNoise<float> fractalNoise(perlinNoise);

    const int32_t noiseType = rand() % 2;
    const float noiseScale = static_cast<float>(rand() % 10) + 4.0f;

    for (int32_t z = 0; z < m_width; z++)
    {
        for (uint32_t y = 0; y < m_height; y++)
        {
            for (int32_t x = 0; x < m_depth; x++)
            {
                float nx = (float)x / (float)m_width;
                float ny = (float)y / (float)m_height;
                float nz = (float)z / (float)m_depth;
#define FRACTAL
#ifdef FRACTAL
                float n = fractalNoise.noise(nx * noiseScale, ny * noiseScale, nz * noiseScale);
#else
                float n = 20.0 * perlinNoise.noise(nx, ny, nz);
#endif
                n = n - floor(n);

                m_imgData[x + y * m_width + z * m_height * m_depth] = static_cast<uint8_t>(floor(n * 255));
            }
        }
    }

    loadTexture3D(m_imgData,m_width,m_height,m_depth);
}

void Texture3dSmoke::prepareUniformBuffers(){
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &m_uniformBuffers,
        sizeof(m_uboVS),
        &m_uboVS));
    VK_CHECK_RESULT(m_uniformBuffers.map());
    updateUniformBuffers();
}

void Texture3dSmoke::updateUniformBuffers(){
    m_uboVS.projection = glm::perspective(glm::radians(60.0f), float(*m_screenWidth) / float(*m_screenHeight), 0.0f, 256.0f);
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, -sqrtf(3)/2.f));
    m_uboVS.model = viewMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_uboVS.model = glm::rotate(m_uboVS.model, glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_uboVS.viewPos = glm::vec4(0.0f, 0.0f, -sqrtf(3)/2.f, 0.0f);
    memcpy(m_uniformBuffers.mapped, &m_uboVS, sizeof(m_uboVS));
}

void Texture3dSmoke::prepareModelBuffer(){
    VK_CHECK_RESULT(m_vulkanDevice->createBuffer(
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            &m_modelVolumeBuffer,
            sizeof(m_modelVolume),
            &m_modelVolume));
    VK_CHECK_RESULT(m_modelVolumeBuffer.map());
    updateModelBuffer();
}

void Texture3dSmoke::updateModelBuffer(){
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, *m_camera.zoom));
    m_modelVolume.model = viewMatrix * glm::translate(glm::mat4(1.0f), *m_camera.cameraPos);
    m_modelVolume.model = glm::rotate(m_modelVolume.model, glm::radians(m_camera.rotation->x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_modelVolume.model = glm::rotate(m_modelVolume.model, glm::radians(m_camera.rotation->y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_modelVolume.model = glm::rotate(m_modelVolume.model, glm::radians(m_camera.rotation->z), glm::vec3(0.0f, 0.0f, 1.0f));
    memcpy(m_modelVolumeBuffer.mapped, &m_modelVolume, sizeof(m_modelVolume));
}

void Texture3dSmoke::prepareTransferFunctionImage(){
    std::vector<glm::vec4> sampleColorList=
    {
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.2, 0.2, 0.2, 1.0, },
        {  0.8, 0.8, 0.8, 1.0, },
        {  1.0, 1.0, 1.0, 1.0, },
        {  0.8, 0.8, 0.8, 1.0, },
        {  0.2, 0.2, 0.2, 1.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
        {  0.0, 0.0, 0.0, 0.0, },
    };
    std::vector<uint8_t> transferImg(sampleColorList.size()*4);
    for (size_t i=0;i<transferImg.size()/4;i++) {
        transferImg[i*4]=uint8_t(sampleColorList[i].x*255);
        transferImg[i*4+1]=uint8_t(sampleColorList[i].y*255);
        transferImg[i*4+2]=uint8_t(sampleColorList[i].z*255);
        transferImg[i*4+3]=uint8_t(sampleColorList[i].w*255);
    }
    m_texture2d.loadFromArray(transferImg.data(),30,1,VK_FORMAT_R8G8B8A8_UNORM,m_vulkanDevice,m_queue,
                                  VK_IMAGE_USAGE_SAMPLED_BIT,VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                  false,VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
}
