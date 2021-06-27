//
// Created by wjl on 2021/6/27.
//

#ifndef LEARNVULKAN_QTVULKANWINDOW_H
#define LEARNVULKAN_QTVULKANWINDOW_H

#include <QMainWindow>

#include "VulkanBaseEngine.h"

BEGIN_NAMESPACE(VulkanEngine)

class VULKANENGINE_EXPORT_API QtVulkanWindow : public QMainWindow {
Q_OBJECT
public:
    QtVulkanWindow(QWidget *parent = Q_NULLPTR);

    ~QtVulkanWindow();

    void setVulkanPtr(VulkanBaseEngine *vulkan);

public slots:

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

protected:
    VulkanBaseEngine *m_vulkan = nullptr;
    glm::vec2 *m_mousePos = nullptr;
};

END_NAMESPACE(VulkanEngine)

#endif //LEARNVULKAN_QTVULKANWINDOW_H
