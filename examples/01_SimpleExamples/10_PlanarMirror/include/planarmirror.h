/*
 * LearnVulkan Examples
 *
 * Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef PLANARMIRROR_H
#define PLANARMIRROR_H
#include "vulkan_basicengine.h"
class PlanarMirror : public VulkanBasicEngine {
public:
    PlanarMirror(bool debug = false);
    ~PlanarMirror();

    virtual void prepare();
    virtual void render();
    virtual void draw();

    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);
    virtual void getEnabledFeatures();

private:
    void createObjects();
    void setupDescriptorPool();
    void setupDescriptorSetLayout();
    void setupDescriptorSet();
    void preparePipelines();
    void buildCommandBuffers();

private:
    //
};
#endif
