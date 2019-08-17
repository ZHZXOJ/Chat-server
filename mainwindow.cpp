#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QHostAddress>
#include <QDataStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Chat-Server");

    connect(&server,SIGNAL(newConnection()),
            this,SLOT(onNewConnection()));

    sizePackLast=0;
    bool ok = server.listen(QHostAddress::AnyIPv4,8888);
    qDebug() << "listen:" << ok;

    if(ok)
        ui->label_2->setText("True"),ui->textEdit->append("listen: True");
    else
        ui->label_2->setText("False"),ui->textEdit->append("listen: False");
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
    clients.append(socket);
    ui->label_4->setText(QString('0'+clients.size()));

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

    quint64 sizeNow =0;
    do{
        sizeNow = socket->bytesAvailable();

        QDataStream stream(socket);

        if(sizePackLast == 0)
        {
            if(sizeNow < sizeof (quint32))
            {
                return;
            }

            stream >> sizePackLast;
        }
        qDebug()<<sizePackLast;
        if(sizeNow >= sizePackLast - 4)
        {
            qDebug()<<"full pack";
            QByteArray dataFull;
            stream >> dataFull;
            sizeNow = socket->bytesAvailable();

            QByteArray dataSend;
            QDataStream stream(&dataSend,QIODevice::WriteOnly);
            stream << (quint32)0 << dataFull;
            stream.device()->seek(0);
            stream << dataSend.size();
            qDebug()<<dataSend;

            for (QList<QTcpSocket*>::iterator itr = clients.begin();itr != clients.end();++itr)
            {
                QTcpSocket *client = *itr;

                client->write(dataSend);
            }

            qDebug() << dataFull;
        }
    }while (sizeNow>0);

}

void MainWindow::onConnected()//连接成功
{
    qDebug() << "connected" ;
    ui->textEdit->append("Connected");
}

void MainWindow::onDisconnected()//断开
{
    QObject *obj = this->sender();
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(obj);

    clients.removeAll(socket);
    socket->deleteLater();

    ui->label_4->setText(QString('0'+clients.size()));
    ui->textEdit->append("Disconnected");
    qDebug() << "disconnected" ;
}

void MainWindow::onError(QAbstractSocket::SocketError socketError)//错误
{
    qDebug() << "error:" << socketError;
    ui->textEdit->append("Error");
}
