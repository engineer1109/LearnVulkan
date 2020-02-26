/*
 * LearnVulkan Examples
 *
 * This is the header file of engine class.
 *
 * Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef BASEPROJECT_H
#define BASEPROJECT_H
#include "vulkan_basicengine.h"
/**
 * @brief BaseProject is the main engine class which inherits from the parent class VulkanBasicEngine.\n
 */
class BaseProject : public VulkanBasicEngine { // VulkanBasicEngine is the parent class.
public:
    /**
     * @brief Construtor.
     * @param[in] debug   Display debug layer info
     */
    BaseProject(bool debug = false);
    ~BaseProject();

    /**
     * @brief Virtual function for your prepare render.
     */
    virtual void prepare();
    /**
     * @brief Renderloop function. Every time you render a frame, this function executes once.
     */
    virtual void render();
    /**
     * @brief Draw rendering image to display devices. It should be executed in the 'render()'.
     */
    virtual void draw();

    /**
     * @brief Design IMGUI.
     */
    virtual void OnUpdateUIOverlay(vks::UIOverlay *overlay);
    /**
     * @brief Set Vulkan other features.
     */
    virtual void getEnabledFeatures();

private:
    /**
     * @brief Your objects can be contructed and prepared here.
     */
    void createObjects();
    /**
     * @brief setupDescriptorPool for vulkan
     */
    void setupDescriptorPool();
    /**
     * @brief setupDescriptorSetLayout for vulkan
     */
    void setupDescriptorSetLayout();
    /**
     * @brief setupDescriptorSet for vulkan
     */
    void setupDescriptorSet();
    /**
     * @brief main class pipeline
     */
    void preparePipelines();
    /**
     * @brief main class command buffers.
     */
    void buildCommandBuffers();

private:
    // You can define your object class here. Like cube, sphere, and so on.
};
#endif
