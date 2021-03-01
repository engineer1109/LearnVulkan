//
// Created by wjl on 21-2-26.
//

#ifndef LEARNVULKAN_VKOBJECT_H
#define LEARNVULKAN_VKOBJECT_H

#include "vulkan_macro.h"
#include "vulkan_template.h"
#include "VulkanDevice.hpp"
#include "VulkanContext.h"

BEGIN_NAMESPACE(VulkanEngine)

class VkObject{
public:
    VkObject() = default;

    virtual ~VkObject() = default;

    template<class T>
    static T* New(VulkanContext* context){
        T* object = new T();
        object->setContext(context);
        return object;
    }

    virtual void setContext(VulkanContext* context){
        m_context = context;
    }

    virtual void prepare() = 0;

    virtual void update() = 0;

protected:
    VulkanContext* m_context = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_VKOBJECT_H
