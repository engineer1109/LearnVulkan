//
// Created by wjl on 2021/6/27.
//

#include "QtVulkanWindow.h"

#include <QMouseEvent>

QtVulkanWindow::QtVulkanWindow(QWidget *parent) : QMainWindow(parent) {
    this->setMouseTracking(true);
}

QtVulkanWindow::~QtVulkanWindow() {}

void QtVulkanWindow::setVulkanPtr(VulkanEngine::VulkanBaseEngine *vulkan) {
    m_vulkan = vulkan;
}

void QtVulkanWindow::mousePressEvent(QMouseEvent *event) {
    if(!m_vulkan) return;
    if (event->button() & Qt::LeftButton) {
        m_vulkan->setMouseButtonLeft(true);
    } else if (event->button() & Qt::RightButton) {
        m_vulkan->setMouseButtonRight(true);
    } else if (event->button() & Qt::MidButton) {
        m_vulkan->setMouseButtonMiddle(true);
    }
    m_vulkan->handleMouseMove(event->pos().x(), event->pos().y());
}

void QtVulkanWindow::mouseReleaseEvent(QMouseEvent *event) {
    if(!m_vulkan) return;
    if (event->button() & Qt::LeftButton) {
        m_vulkan->setMouseButtonLeft(false);
    } else if (event->button() & Qt::RightButton) {
        m_vulkan->setMouseButtonRight(false);
    } else if (event->button() & Qt::MidButton) {
        m_vulkan->setMouseButtonMiddle(false);
    }
    m_vulkan->handleMouseMove(event->pos().x(), event->pos().y());
}

void QtVulkanWindow::mouseMoveEvent(QMouseEvent *event) {
    if(m_vulkan) {
        m_vulkan->handleMouseMove(event->pos().x(), event->pos().y());
    }
}

void QtVulkanWindow::showEvent(QShowEvent *event) {
    if(m_vulkan && !m_vulkan->getPrepared()) {
#if defined(VK_USE_PLATFORM_XCB_KHR)
        m_vulkan->initxcbConnection();
#endif
        m_vulkan->initVulkan();
        m_vulkan->setWindow(uint32_t(winId()));
        m_vulkan->prepare();
        m_vulkan->renderAsyncThread();
    }
}

void QtVulkanWindow::closeEvent(QCloseEvent *event) {
    if(m_vulkan) {
        m_vulkan->quit();
        m_vulkan->renderJoin();
    }
}
