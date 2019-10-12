#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "static_triangle.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete m_pStatictriangle;
    m_pStatictriangle=nullptr;
    m_thread->join();
    delete m_thread;
    m_thread=nullptr;
    delete ui;
}

void MainWindow::on_button_render_pressed()
{
    m_thread=new std::thread(&MainWindow::render,this);
}

void MainWindow::render()
{
    uint32_t window=this->ui->widget_vulkan->winId();
    StaticTriangle* m_pStatictriangle=new StaticTriangle(false);
    m_pStatictriangle->initVulkan();
    m_pStatictriangle->setWindow(window);
    m_pStatictriangle->prepare();
    m_pStatictriangle->renderLoop();
}
