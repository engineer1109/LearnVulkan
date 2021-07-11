//
// Created by wjl on 21-2-28.
//

#ifndef LEARNVULKAN_THIRDPERSONCAMERA_H
#define LEARNVULKAN_THIRDPERSONCAMERA_H

#include <glm/glm.hpp>

class ThirdPersonCamera {
public:
    ThirdPersonCamera()=default;
    ~ThirdPersonCamera()=default;

    void setZoom(float zoom) { m_zoom = zoom; }

    glm::vec3 m_rotation = glm::vec3();
    glm::vec3 m_cameraPos = glm::vec3();
    const float m_baseZoom = -2.f;
    float m_zoom = m_baseZoom;
};


#endif //LEARNVULKAN_THIRDPERSONCAMERA_H
