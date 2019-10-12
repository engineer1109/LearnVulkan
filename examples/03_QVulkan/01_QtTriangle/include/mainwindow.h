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

private:
    void render();

private slots:
    void on_button_render_pressed();

private:
    Ui::MainWindow *ui;
    StaticTriangle* m_pStatictriangle;
    std::thread* m_thread;
};
#endif
