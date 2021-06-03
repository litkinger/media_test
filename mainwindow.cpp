#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logging/loghandler.h"

std::shared_ptr<LogHandler> logHandler(new LogHandlerImpl());
static CommonLog commonLogger(logHandler,"MainWindow");

#define MAINWINDOW_LOG(level) COMMON_LOG(commonLogger, level)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MAINWINDOW_LOG(INFO) << "Starting" << std::endl;
}

MainWindow::~MainWindow()
{
    MAINWINDOW_LOG(INFO) << "Stopped" << std::endl;
    delete ui;
}

