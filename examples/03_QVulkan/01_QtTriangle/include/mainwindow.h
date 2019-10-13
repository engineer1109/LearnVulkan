#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class StaticTriangle;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    void vkRender();
    void eventLoop();

private slots:
    void on_pushButton_pressed();
    void disableRotate();
    void pressButton();

private:
    Ui::MainWindow *ui;
    StaticTriangle* m_pStatictriangle;
    std::thread* m_thread;
    bool m_quitRender=false;
    bool m_rotate=false;
};
#endif
