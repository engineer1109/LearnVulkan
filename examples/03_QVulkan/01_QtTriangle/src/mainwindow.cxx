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
    connect(ui->button_render,SIGNAL(clicked()), this, SLOT(setAutoRotate()));
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
    m_pStatictriangle=new StaticTriangle(false);
    m_pStatictriangle->initVulkan();
    m_pStatictriangle->setWindow(ui->widget_vulkan->getWindowHandle());
    m_pStatictriangle->prepare();
    m_pStatictriangle->renderAsyncThread();
    m_pStatictriangle->enableAutoRotation(true);
    ui->widget_vulkan->setVulkanPtr(m_pStatictriangle);
}

void MainWindow::setAutoRotate(){
    m_pStatictriangle->enableAutoRotation(m_rotate);
    m_rotate=!m_rotate;
}
