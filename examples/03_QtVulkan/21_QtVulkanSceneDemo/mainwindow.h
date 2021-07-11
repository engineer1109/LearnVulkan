#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace VulkanEngine {
class ShadowMapping;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void uiSettings();

    void connectSettings();

    void initRender();

public slots:
    void debugQuad();

    void setDistance();

    void setReflect();

protected:
    void showEvent(QShowEvent *event);

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui = nullptr;

    VulkanEngine::ShadowMapping *m_vkEngine = nullptr;
};

#endif // MAINWINDOW_H
