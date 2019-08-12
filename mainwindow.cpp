#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Chat-Server");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionclose_triggered()
{
    MainWindow::close();
}
