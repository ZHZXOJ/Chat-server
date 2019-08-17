#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionclose_triggered();

public slots:
    void onNewConnection();

    void onReadReady();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    Ui::MainWindow *ui;
    QTcpServer server;
    QList<QTcpSocket*> clients;

    quint32 sizePackLast;
};

#endif // MAINWINDOW_H
