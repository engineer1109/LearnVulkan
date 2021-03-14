//
// Created by wjl on 21-2-28.
//

#include "ThirdPersonEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

void ThirdPersonEngine::updateCamera() {
    defaultTouchOperation();

    if (m_touchMode == TouchMode::SINGLE) {
        if (m_mousePos[0].x > m_mousePosOld[0].x) {
            m_rotation.y += 1.f;
        } else if (m_mousePos[0].x < m_mousePosOld[0].x) {
            m_rotation.y -= 1.f;
        }
        if (m_mousePos[0].y > m_mousePosOld[0].y) {
            m_rotation.x += 1.f;
        } else if (m_mousePos[0].y < m_mousePosOld[0].y) {
            m_rotation.x -= 1.f;
        }
    } else if (m_touchMode == TouchMode::DOUBLE) {
        m_zoom = m_distance + m_baseZoom;
    }
    m_mousePosOld[0] = m_mousePos[0];
}

END_NAMESPACE(VulkanEngine)