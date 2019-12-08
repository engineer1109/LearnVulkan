/*
* LearnVulkan Examples
*
* Copyright (C) by engineer1109 - https://github.com/engineer1109/LearnVulkan
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "static_triangle.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAutoFillBackground(false);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);
    ui->widget_vulkan->setAutoFillBackground(false);
    ui->widget_vulkan->setWindowFlags(Qt::FramelessWindowHint);
    ui->widget_vulkan->setAttribute(Qt::WA_TranslucentBackground,true);
}

MainWindow::~MainWindow()
{
    if(m_pStatictriangle!=nullptr){
        delete m_pStatictriangle;
        m_pStatictriangle=nullptr;
    }
    if(ui!=nullptr){
        delete ui;
        ui=nullptr;
    }
}

void MainWindow::showEvent(QShowEvent *event){
    vkRender();
}

void MainWindow::closeEvent(QCloseEvent *event){
    m_pStatictriangle->quitRender();
}

void MainWindow::vkRender()
{
    if(!m_pStatictriangle){
        m_pStatictriangle=new StaticTriangle(false);
        m_pStatictriangle->initVulkan();
        m_pStatictriangle->setWindow(ui->widget_vulkan->getWindowHandle());
        m_pStatictriangle->prepare();
        m_pStatictriangle->renderAsyncThread();
        m_pStatictriangle->enableAutoRotation(false);

        ui->widget_vulkan->setVulkanPtr(m_pStatictriangle);
    }
}

void MainWindow::setAutoRotate(){
    m_pStatictriangle->enableAutoRotation(m_rotate);
    m_rotate=!m_rotate;
}
