#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ShadowMapping.h"

using namespace VulkanEngine;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->vulkanWidget->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint); // important
    uiSettings();
    connectSettings();
    initRender();
}

MainWindow::~MainWindow()
{
    delete ui;
    ui = nullptr;
}

void MainWindow::uiSettings() {
    ui->slider_distance->setRange(-100, 100);
    ui->slider_distance->setValue(-40);
}

void MainWindow::connectSettings() {
    connect(ui->btn_debugquad, SIGNAL(clicked()), this, SLOT(debugQuad()));
    connect(ui->slider_distance, SIGNAL(valueChanged(int)), this, SLOT(setDistance()));
    connect(ui->slider_reflect, SIGNAL(valueChanged(int)), this, SLOT(setReflect()));
}

void MainWindow::initRender() {
    m_vkEngine = new VulkanEngine::ShadowMapping();
#if defined(_WIN32)
    m_vkEngine->setWindow((HWND)ui->vulkanWidget->winId());
#else
    m_vkEngine->setWindow(ui->vulkanWidget->winId());
#endif
    ui->vulkanWidget->setVulkanPtr(m_vkEngine);
}

void MainWindow::debugQuad() {
    m_vkEngine->seeDebugQuad();
}

void MainWindow::setDistance() {
    m_vkEngine->setZoom(ui->slider_distance->value()/10.f);
}

void MainWindow::setReflect() {
    m_vkEngine->setReflect(ui->slider_reflect->value()/100.f);
}

void MainWindow::showEvent(QShowEvent *event) {}

void MainWindow::closeEvent(QCloseEvent *event) {
    ui->vulkanWidget->close();
}
