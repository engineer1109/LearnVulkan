#ifndef VULKAN_BASICENGINE_ALGORITHM_GRAPHICS_H
#define VULKAN_BASICENGINE_ALGORITHM_GRAPHICS_H
#include "vulkan_basicengine_help.h"
#include "vulkan_basicengine_export.h"

BEGIN_NAMESPACE(VulkanTemplate)

class VolumeGLM{
public:
    static glm::mat4 translate(glm::mat4 &modelView,glm::vec3 offset){
        glm::mat4 modelMat=glm::mat4(1.0f);
        modelMat[3][0]=offset.x;
        modelMat[3][1]=offset.y;
        modelMat[3][2]=offset.z;
        return (modelView*modelMat);
    }

    static glm::mat4 rotate(glm::mat4 &modelView,float angle,glm::vec3 direction){
        float cosValue=cosf(angle);
        float sinValue=sinf(angle);
        glm::mat4 modelMat=glm::mat4(1.f);
        modelMat[0][0]=(1.0f-cosValue)*direction.x*direction.x+cosValue;
        modelMat[0][1]=(1.0f-cosValue)*direction.x*direction.y+direction.z*sinValue;
        modelMat[0][2]=(1.0f-cosValue)*direction.x*direction.z-direction.y*sinValue;
        modelMat[1][0]=(1.0f-cosValue)*direction.y*direction.x-direction.z*sinValue;
        modelMat[1][1]=(1.0f-cosValue)*direction.y*direction.y+cosValue;
        modelMat[1][2]=(1.0f-cosValue)*direction.y*direction.z+direction.x*sinValue;
        modelMat[2][0]=(1.0f-cosValue)*direction.x*direction.z+direction.y*sinValue;
        modelMat[2][1]=(1.0f-cosValue)*direction.y*direction.z-direction.x*sinValue;
        modelMat[2][2]=(1.0f-cosValue)*direction.z*direction.z+cosValue;
        return modelMat*modelView;
    }
};


END_NAMESPACE(VulkanTemplate)

#endif // VULKAN_BASICENGINE_ALGORITHM_H
