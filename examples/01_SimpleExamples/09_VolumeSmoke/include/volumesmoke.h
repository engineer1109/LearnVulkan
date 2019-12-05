/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef INSTANCEDRAW_H
#define INSTANCEDRAW_H
#include "vulkan_basicengine.h"
class Texture3dSmoke;
class VolumeSmoke:public VulkanBasicEngine{
public:
    VolumeSmoke(bool debug=false);
    ~VolumeSmoke();

    virtual void prepare();
    virtual void render();
    virtual void draw();

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);
    virtual void getEnabledFeatures();
private:
    void createObjects();
private:
    Texture3dSmoke* m_cube=nullptr;
};

#endif
