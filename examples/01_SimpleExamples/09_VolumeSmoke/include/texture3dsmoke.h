/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef TEXTURE3DSMOKE_H
#define TEXTURE3DSMOKE_H
#include "vulkan_basicengine_texture.h"
#include "vulkan_basicengine_texture3dplane.h"

class Texture3dSmoke:public VulkanTemplate::Texture3DPlane{
public:
    Texture3dSmoke(uint32_t width,uint32_t height,uint32_t depth);
    ~Texture3dSmoke();

    void create();
    void update();
public:
    glm::mat4 m_viewMat;
    struct{
        glm::mat4 model;
    }m_modelVolume;
    vks::Buffer m_modelVolumeBuffer;
    vks::Texture2DStbImage m_texture2d;
protected:
    void generateNoise();
    void prepareUniformBuffers();
    void updateUniformBuffers();
    void prepareModelBuffer();
    void updateModelBuffer();
    void prepareTransferFunctionImage();
private:
    uint8_t* m_imgData=nullptr;
    uint32_t m_width=0;
    uint32_t m_height=0;
    uint32_t m_depth=0;
};

#endif // INSTANCECUBE_H
