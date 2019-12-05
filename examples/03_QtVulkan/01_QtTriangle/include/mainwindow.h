/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void setAutoRotate();
    void setBackground();

private:
    Ui::MainWindow *ui=nullptr;
    StaticTriangle* m_pStatictriangle=nullptr;
    bool m_quitRender=false;
    bool m_rotate=false;
};
#endif
