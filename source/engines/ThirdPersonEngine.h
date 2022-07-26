//
// Created by wjl on 21-2-28.
//

#ifndef LEARNVULKAN_THIRDPERSONENGINE_H
#define LEARNVULKAN_THIRDPERSONENGINE_H

#include "VulkanBaseEngine.h"
#include "ThirdPersonCamera.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API ThirdPersonEngine : public VulkanBaseEngine , public ThirdPersonCamera {
public:
    ThirdPersonEngine() = default;
    virtual ~ThirdPersonEngine() {}

    void updateCamera();
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_THIRDPERSONENGINE_H
