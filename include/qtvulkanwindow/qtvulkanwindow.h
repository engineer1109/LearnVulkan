#ifndef QTVULKANWINDOW_H
#define QTVULKANWINDOW_H
#include <QMainWindow>
#include "qtvulkanwindow_def.h"
#include "vulkan_basicengine.h"
class VK_QTVULKANWINDOW_EXPORT QtVulkanWindow:public QMainWindow{
    Q_OBJECT
public:
    QtVulkanWindow(QWidget *parent = Q_NULLPTR);
    ~QtVulkanWindow();

    void setFlag(Qt::WindowFlags flags);
    uint32_t getWindowHandle();
    void setVulkanPtr(VulkanBasicEngine* vulkan);

public slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    VulkanBasicEngine* m_vulkan=nullptr;
    glm::vec2* m_mousePos=nullptr;
    bool* m_mouseLeftClick=nullptr;
};
#endif
