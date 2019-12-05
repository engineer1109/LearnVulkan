/*
* LearnVulkan QtVulkanWindow
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include <QMouseEvent>
#include "qtvulkanwindow.h"
#include <iostream>
QtVulkanWindow::QtVulkanWindow(QWidget *parent):QMainWindow (parent){
    this->setMouseTracking(true);
}

QtVulkanWindow::~QtVulkanWindow(){

}

void QtVulkanWindow::setFlag(Qt::WindowFlags flags){
    setWindowFlags(flags);
}

uint32_t QtVulkanWindow::getWindowHandle(){
    return uint32_t(winId());
}

void QtVulkanWindow::setVulkanPtr(VulkanBasicEngine* vulkan){
    m_vulkan=vulkan;
}

void QtVulkanWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() & Qt::LeftButton){
        m_vulkan->mouseButtons.left=true;
    }
    else if (event->button() & Qt::RightButton) {
        m_vulkan->mouseButtons.right=true;
    }
    else if (event->button() & Qt::MidButton) {
        m_vulkan->mouseButtons.middle=true;
    }
    m_vulkan->handleMouse(event->pos().x(),event->pos().y());
}

void QtVulkanWindow::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() & Qt::LeftButton){
        m_vulkan->mouseButtons.left=false;
    }
    else if (event->button() & Qt::RightButton) {
        m_vulkan->mouseButtons.right=false;
    }
    else if (event->button() & Qt::MidButton) {
        m_vulkan->mouseButtons.middle=false;
    }
    m_vulkan->handleMouse(event->pos().x(),event->pos().y());
}

void QtVulkanWindow::mouseMoveEvent(QMouseEvent *event){
    m_vulkan->handleMouse(event->pos().x(),event->pos().y());
}
