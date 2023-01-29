//
// Created by wjl on 2023/1/29.
//

#include "VolumeFog.h"

#include "VulkanPlane.h"
#include "VulkanTexture3D.h"
#include "VulkanTexture2D.h"
#include "PerlinNoise.h"
#include "UniformCamera.h"
#include "VolumeModelUniform.h"
#include "GraphicsAlgorithm.h"
#include "VulkanVertFragShader.h"

BEGIN_NAMESPACE(VulkanEngine)

VolumeFog::~VolumeFog() {}

void VolumeFog::prepareFunctions() {

}

void VolumeFog::prepareMyObjects() {
    createCameraUniform();
    createVolumeRenderObject();
    createVolumeTexture();
    createTransferColor();
    createModelUniform();

    setDescriptorSet();
    createPipelines();
}

void VolumeFog::buildMyObjects(VkCommandBuffer &cmd) {
    m_volumeRenderObject->build(cmd, m_volumeShader);
}

void VolumeFog::render() {
    updateCamera();
    m_cubeUniform->update();

    m_volumeModelUniform->m_model.model = glm::inverse(m_cubeUniform->m_uboVS.view * m_cubeUniform->m_uboVS.model);
    m_volumeModelUniform->update();
}

void VolumeFog::setDescriptorSet() {
    m_vulkanDescriptorSet->addBinding(0, &(m_cubeUniform->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS,
                                      0);
    m_vulkanDescriptorSet->addBinding(1, &(m_volumeModelUniform->m_uniformBuffer.descriptor),
                                      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS,
                                      0);
    m_vulkanDescriptorSet->addBinding(2, &(m_volumeTexture->descriptor),
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      VK_SHADER_STAGE_FRAGMENT_BIT,
                                      0);
    m_vulkanDescriptorSet->addBinding(3, &(m_transferColor->descriptor),
                                      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                      VK_SHADER_STAGE_FRAGMENT_BIT,
                                      0);
    m_vulkanDescriptorSet->GenPipelineLayout(&m_pipelineLayout);
}

void VolumeFog::createPipelines() {
    m_pipelines->createBasePipelineInfo(m_pipelineLayout, m_renderPass);
    m_pipelines->createPipeline(m_volumeShader);
}

void VolumeFog::createCameraUniform() {
    REGISTER_OBJECT<UniformCamera>(m_cubeUniform);
    m_cubeUniform->m_uboVS.lightpos = glm::vec4(0.0f, -0.0f, 0.0f, 1.0f);
    m_cubeUniform->m_pCameraPos = &m_cameraPos;
    m_cubeUniform->m_pRotation = &m_rotation;
    m_cubeUniform->m_pZoom = &m_zoom;
    m_cubeUniform->prepare();
}

void VolumeFog::createVolumeRenderObject() {
    REGISTER_OBJECT(m_volumeRenderObject);
    m_volumeRenderObject->setDirection(VulkanPlane::XY);
    m_volumeRenderObject->prepare();

    REGISTER_OBJECT<VulkanVertFragShader>(m_volumeShader);
    m_volumeShader->setShaderObjPath(FS::getPath("shaders/VolumeFog/volumefog.so.vert"),
                                   FS::getPath("shaders/VolumeFog/volumefog.so.frag"));
    m_volumeShader->setCullFlag(VK_CULL_MODE_NONE);
    m_volumeShader->prepare();
}

void VolumeFog::createVolumeTexture() {
    int width = 512;
    int height = 512;
    int depth = 512;

    std::vector<uint8_t> m_volumeData(width * height * depth);

    VulkanEngine::PerlinNoise<float> perlinNoise;
    VulkanEngine::FractalNoise<float> fractalNoise(perlinNoise);

    const int32_t noiseType = rand() % 2;
    const float noiseScale = static_cast<float>(rand() % 10) + 4.0f;

#pragma omp parallel for num_threads(8)
    for (int32_t z = 0; z < width; z++) {
        for (uint32_t y = 0; y < height; y++) {
            for (int32_t x = 0; x < depth; x++) {
                float nx = (float) x / (float) width;
                float ny = (float) y / (float) height;
                float nz = (float) z / (float) depth;
#define FRACTAL
#ifdef FRACTAL
                float n = fractalNoise.noise(nx * noiseScale, ny * noiseScale, nz * noiseScale);
#else
                float n = 20.0 * perlinNoise.noise(nx, ny, nz);
#endif
                n = n - floor(n);

                m_volumeData[x + y * width + z * height * depth] = static_cast<uint8_t>(floor(n * 255));
            }
        }
    }

    REGISTER_OBJECT(m_volumeTexture);
    m_volumeTexture->setAddressMode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER);
    m_volumeTexture->loadFromData(m_volumeData.data(), width, height, depth, VK_FORMAT_R8_UNORM);
}

void VolumeFog::createTransferColor() {
    std::vector<glm::vec4> sampleColorList =
            {
                    {0.0, 0.0, 0.0, 0.0,},
                    //{  0.2, 0.2, 0.2, 1.0, },
                    {0.8, 0.8, 0.8, 1.0,},
                    {1.0, 1.0, 1.0, 1.0,},
                    {0.8, 0.8, 0.8, 1.0,},
                    //{  0.2, 0.2, 0.2, 1.0, },
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
                    {0.0, 0.0, 0.0, 0.0,},
            };
    std::vector<uint8_t> transferImg(sampleColorList.size() * 4);
    for (size_t i = 0; i < transferImg.size() / 4; i++) {
        transferImg[i * 4] = uint8_t(sampleColorList[i].x * 255);
        transferImg[i * 4 + 1] = uint8_t(sampleColorList[i].y * 255);
        transferImg[i * 4 + 2] = uint8_t(sampleColorList[i].z * 255);
        transferImg[i * 4 + 3] = uint8_t(sampleColorList[i].w * 255);
    }
    REGISTER_OBJECT(m_transferColor);
    m_transferColor->setAddressMode(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
    m_transferColor->loadFromData(transferImg.data(), sampleColorList.size(), 1, 4, VK_FORMAT_R8G8B8A8_UNORM);
}

void VolumeFog::createModelUniform() {
    REGISTER_OBJECT(m_volumeModelUniform);
    m_volumeModelUniform->prepare();
}

END_NAMESPACE(VulkanEngine)