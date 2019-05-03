
#include <QDebug>
#include <QLabel>

#include "mainwindow.h"
#include "globals.h"
#include "devicedatadisplay.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_TcpSocket(nullptr)
{

    QWidget *w = new QWidget;
    QVBoxLayout *vl = new QVBoxLayout(w);

    QLabel *lbl_title = new QLabel;
    lbl_title->setText("Server");
    QFont fnt(lbl_title->font());
    fnt.setPointSize(24);
    fnt.setBold(true);
    lbl_title->setFont(fnt);
    lbl_title->setAlignment(Qt::AlignCenter);
    lbl_title->setFrameStyle(QFrame::Raised | QFrame::Panel);

    vl->addWidget(lbl_title, 1);

    m_DataLayout = new QGridLayout;

    vl->addLayout(m_DataLayout, 60);

    m_MsgList = new QListWidget;
    vl->addWidget(m_MsgList, 40);


    this->setCentralWidget(w);

    this->resize(900, 700);


    g_TcpServer = new QTcpServer(this);

    const uint16_t listen_port = 33333;

    connect(g_TcpServer, &QTcpServer::newConnection, this, &MainWindow::tcpNewConnection);
    g_TcpServer->listen(QHostAddress::Any, listen_port);

    connect(this, &MainWindow::message, this, &MainWindow::showMessage);

    emit message(QString("Listening on TCP port %1").arg(listen_port));
}



MainWindow::~MainWindow()
{

}



void MainWindow::tcpNewConnection()
{
    //qDebug() << "new connection";

    m_TcpSocket = g_TcpServer->nextPendingConnection();

    if(m_TcpSocket)
    {
        emit message("New connection established from " + m_TcpSocket->peerAddress().toString());

        g_TcpServer->pauseAccepting();

        connect(m_TcpSocket, &QTcpSocket::readyRead, this, &MainWindow::tcpReadyRead);
        connect(m_TcpSocket, &QTcpSocket::stateChanged, this, &MainWindow::tcpSocketStateChanged);
    }

}


void MainWindow::tcpReadyRead()
{
    QByteArray read_data = m_TcpSocket->readAll();

    //qDebug() << read_data.size() << read_data;

    const int dev_name_len = 20;
    const int sync_len     = 4;
    const int dev_info_len = sync_len + dev_name_len*2 + 4;
    if(read_data.size() < dev_info_len) return;   //must be keep-alive data

    const char *data = read_data.constData();
    const char sync_pattern[] = { char(0xab), char(0x12), char(0xcd), char(0x34) };

    int idx = 0;
    char dev_id_name[dev_name_len+1];
    char dev_adv_name[dev_name_len+1];
    uint32_t data_value;

    dev_id_name[dev_name_len]  = '\0';
    dev_adv_name[dev_name_len] = '\0';

    while(true)
    {
        if(memcmp(data+idx, sync_pattern, sync_len) != 0) break;

        strncpy(dev_id_name,  data+idx+sync_len,                dev_name_len);
        strncpy(dev_adv_name, data+idx+sync_len+dev_name_len,   dev_name_len);
        memcpy(&data_value,   data+idx+sync_len+2*dev_name_len, sizeof(data_value));

        DeviceDataDisplay *dev = nullptr;
        for(int i = 0; i < m_DeviceDisplays.size(); ++i)
        {
            DeviceDataDisplay *d = m_DeviceDisplays[i];
            if(d->m_IdentifiedName == dev_id_name)
            {
                dev = d;
                break;
            }
        }

        if(dev == nullptr)
        {
            //create new
            dev = new DeviceDataDisplay;
            const int num_objects = m_DeviceDisplays.size();
            const int num_cols = 3;

            const int row = num_objects / num_cols;
            const int col = num_objects % num_cols;

            DeviceDataDisplay *ddd = new DeviceDataDisplay;
            m_DataLayout->addWidget(ddd, row, col, Qt::AlignCenter);
            ddd->setNames(dev_id_name, dev_adv_name);

            m_DeviceDisplays.push_back(ddd);
        }

        dev->dataUpdated(int(data_value));

        idx += dev_info_len;
        if( (idx+dev_info_len) >= (read_data.size()) ) break;
    }



}


void MainWindow::tcpSocketStateChanged(QAbstractSocket::SocketState socket_state)
{
    //qDebug() << "tcpSocketStateChanged";

    if(socket_state == QAbstractSocket::UnconnectedState)
    {
        emit message("Connection from " + m_TcpSocket->peerAddress().toString() + " closed");

        m_TcpSocket = nullptr;
        g_TcpServer->resumeAccepting();
    }
}


void MainWindow::closeEvent(QCloseEvent *)
{
    if(g_TcpServer)
    {
        if(m_TcpSocket)
        {
            m_TcpSocket->disconnectFromHost();
            m_TcpSocket = nullptr;
        }

        g_TcpServer->close();
    }
}


void MainWindow::showMessage(QString msg)
{
    m_MsgList->addItem(msg);
}




