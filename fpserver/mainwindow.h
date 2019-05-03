#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QGridLayout>
#include <QListWidget>
#include <QList>

class DeviceDataDisplay;


class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QTcpSocket   *m_TcpSocket;
    QGridLayout *m_DataLayout;
    QListWidget *m_MsgList;
    QList<DeviceDataDisplay*> m_DeviceDisplays;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void message(QString msg);

private slots:
    void tcpNewConnection();
    void tcpReadyRead();
    void tcpSocketStateChanged(QAbstractSocket::SocketState socket_state);

    void showMessage(QString msg);



protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H


