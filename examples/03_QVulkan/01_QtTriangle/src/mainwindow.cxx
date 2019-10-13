#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "static_triangle.h"

#ifdef VK_USE_PLATFORM_XCB_KHR
#include <QtX11Extras/QX11Info>
#include <xcb/xcb.h>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

    //delete m_thread;
   // m_thread=nullptr;
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event){
    //m_thread=new std::thread(&MainWindow::vkRender,this);
    vkRender();
    //m_pStatictriangle->enableAutoRotation();
    //m_thread=new std::thread(&MainWindow::eventLoop,this);
}

void MainWindow::closeEvent(QCloseEvent *event){
    m_pStatictriangle->quitRender();
    m_quitRender=true;
}

void MainWindow::vkRender()
{
    int loop=1;
    uint32_t window=this->ui->widget_vulkan->winId();
    StaticTriangle* m_pStatictriangle=new StaticTriangle(false);
    m_pStatictriangle->initVulkan();
    m_pStatictriangle->setWindow(window,QX11Info::connection());
    m_pStatictriangle->prepare();
    m_pStatictriangle->renderAsyncThread();
    m_pStatictriangle->enableAutoRotation();
    connect(ui->button_render, SIGNAL(clicked()), this, SLOT(disableRotate()));
    //m_thread=new std::thread(&MainWindow::eventLoop,this);

//    while(loop){
//        if(m_quitRender){
//            printf("1");
//            m_pStatictriangle->quitRender();
//            delete m_pStatictriangle;
//            m_pStatictriangle=nullptr;
//            loop=0;
//        }
//    }
}

void MainWindow::eventLoop(){
    int loop=1;
        while(loop){
            //printf("0");
            if(m_quitRender){
                std::cout<<"1"<<std::endl;
                //m_pStatictriangle->quitRender();
                //delete m_pStatictriangle;
                //m_pStatictriangle=nullptr;
                loop=0;
            }
        }
}

void MainWindow::on_pushButton_pressed()
{
    m_quitRender=true;
    //m_thread->join();
}

void MainWindow::disableRotate(){
    m_pStatictriangle->enableAutoRotation(false);
}
