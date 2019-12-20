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
    ui->widget_vulkan->setFlag(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    ui->slider_red->setMaximum(256);
    ui->slider_red->setValue(int(0.1*256));
    ui->slider_green->setMaximum(256);
    ui->slider_green->setValue(int(0.2*256));
    ui->slider_blue->setMaximum(256);
    ui->slider_blue->setValue(int(0.3*256));
    ui->comboBox_object->addItem("Triangle");
    connect(ui->button_render,SIGNAL(clicked()), this, SLOT(setAutoRotate()));
    connect(ui->slider_red,SIGNAL(sliderReleased()),this,SLOT(setBackground()));
    connect(ui->slider_green,SIGNAL(sliderReleased()),this,SLOT(setBackground()));
    connect(ui->slider_blue,SIGNAL(sliderReleased()),this,SLOT(setBackground()));

    vkRender();
}

MainWindow::~MainWindow()
{
    if(ui!=nullptr){
        delete ui;
        ui=nullptr;
    }
    if(m_pStatictriangle!=nullptr){
        delete m_pStatictriangle;
        m_pStatictriangle=nullptr;
    }
}

void MainWindow::showEvent(QShowEvent *event){

}

void MainWindow::closeEvent(QCloseEvent *event){
    ui->widget_vulkan->close();  //Vulkan Window need close first
}

void MainWindow::vkRender()
{
    if(!m_pStatictriangle){
        m_pStatictriangle=new StaticTriangle(false);
        m_pStatictriangle->enableAutoRotation(true);
        ui->widget_vulkan->setVulkanPtr(m_pStatictriangle);
    }
}

void MainWindow::setAutoRotate(){
    m_pStatictriangle->enableAutoRotation(m_rotate);
    m_rotate=!m_rotate;
}

void MainWindow::setBackground(){
    m_pStatictriangle->rebuildCommandBuffers(ui->slider_red->value()/256.f,ui->slider_green->value()/256.f,ui->slider_blue->value()/256.f);
}
