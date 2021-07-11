//
// Created by wjl on 2021/6/27.
//

#ifndef LEARNVULKAN_QTVULKANWINDOW_H
#define LEARNVULKAN_QTVULKANWINDOW_H

#include <QMainWindow>

#include "VulkanBaseEngine.h"

class VULKANENGINE_EXPORT_API QtVulkanWindow : public QMainWindow {
Q_OBJECT
public:
    QtVulkanWindow(QWidget *parent = Q_NULLPTR);

    virtual ~QtVulkanWindow();

    void setVulkanPtr(VulkanEngine::VulkanBaseEngine *vulkan);

public slots:

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

protected:
    VulkanEngine::VulkanBaseEngine *m_vulkan = nullptr;
    glm::vec2 *m_mousePos = nullptr;
};

#endif //LEARNVULKAN_QTVULKANWINDOW_H
