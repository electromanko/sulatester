#include "configConnectionWidget.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QHostAddress>

ConfigConnectionWidget::ConfigConnectionWidget( QWidget *parent) : QWidget(parent)
{
    /*tableWidget = new QTableWidget(10, 2, this);
    tableWidget->setMaximumWidth(200);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setShowGrid(false);
    tableHeader<<"Var"<<"Val";
    tableWidget->setHorizontalHeaderLabels(tableHeader);
    tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableWidget->setRowHeight(10,10);*/
    configTextEdit = new QTextEdit();
    configTextEdit->setReadOnly(true);
    connectPushButton= new QPushButton(tr("Connect"));
    ipAddrLineEdit = new QLineEdit();
    ipAddrLineEdit->setMaxLength(16);
    ipAddrLineEdit->setMaximumWidth(100);
    portLineEdit = new QLineEdit();
    portLineEdit->setMaxLength(6);
    portLineEdit->setMaximumWidth(60);
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(tableWidget);
    QHBoxLayout *hLayout = new QHBoxLayout;
    //hLayout->addWidget(ipAddrLineEdit);
    //hLayout->addWidget(portLineEdit);
    mainLayout->addLayout(hLayout);

    //mainLayout->addWidget(connectPushButton);
    mainLayout->addWidget(configTextEdit);
    setLayout(mainLayout);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    tcpSocket = new QTcpSocket();

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateInfo()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceive()));
    updateTimer->start(10000);

}

void ConfigConnectionWidget::tcpConnect(QHostAddress ipaddr, int port)
{
    tcpSocket->connectToHost(ipaddr, port);
}

void ConfigConnectionWidget::dataReceive()
{
    while (tcpSocket->canReadLine()){
        QString str = QString(tcpSocket->readLine());
        if (!str.contains("DSL>", Qt::CaseInsensitive)
                && !str.contains("DSL/", Qt::CaseInsensitive)) configTextEdit->append(str);
    }
}

void ConfigConnectionWidget::updateInfo()
{
     configTextEdit->clear();
    if (tcpSocket->state()==QAbstractSocket::ConnectedState){
        tcpSocket->write(QString("..\r\n").toLocal8Bit());
        tcpSocket->write(QString("port 0\r\n").toLocal8Bit());
        tcpSocket->write(QString("get state\r\n").toLocal8Bit());
        //tcpSocket->waitForBytesWritten();
        /*QByteArray data;
        while (tcpSocket->canReadLine()){
            data.append(tcpSocket->readLine());
        }
        configTextEdit->append(QString(data));*/
        //tableWidget->setItem(0, 1, new QTableWidgetItem(QString(data)));
    }
}

