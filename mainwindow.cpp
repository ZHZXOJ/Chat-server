#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Chat-Server");

    connect(&server,SIGNAL(newConnection()),
            this,SLOT(onNewConnection()));

    bool ok = server.listen(QHostAddress::AnyIPv4,8888);
    qDebug() << "listen:" << ok;
    if(ok)
        ui->label_2->setText("True");
    else
        ui->label_2->setText("False");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionclose_triggered()
{
    MainWindow::close();
}

void MainWindow::onNewConnection()
{
    //使用这个与客户端通信
    QTcpSocket *socket = server.nextPendingConnection();

    //连接信号和槽
    connect(socket,SIGNAL(readyRead()),
            this,SLOT(onReadReady()));

    connect(socket,SIGNAL(connected()),
            this,SLOT(onConnected()));

    connect(socket,SIGNAL(disconnected()),
            this,SLOT(onDisconnected()));

    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(onError(QAbstractSocket::SocketError)));
}

void MainWindow::onReadReady()
{
    QObject *obj = this->sender();
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(obj);
    QByteArray data = socket->readAll();
    qDebug() << data;
}

void MainWindow::onConnected()//连接成功
{
    qDebug() << "connected" ;
}

void MainWindow::onDisconnected()//断开
{
    qDebug() << "disconnected" ;
}

void MainWindow::onError(QAbstractSocket::SocketError socketError)//错误
{
    qDebug() << "error:" << socketError;
}
