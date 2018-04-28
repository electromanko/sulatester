#include "depthWidget.h"

#include <QHBoxLayout>
#include <QIntValidator>
#include <QNetworkProxy>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QLabel>

DepthWidget::DepthWidget(QWidget *parent) : QWidget(parent)
{
    ipAddrLineEdit = new QLineEdit();
    ipAddrLineEdit->setMaxLength(16);
    ipAddrLineEdit->setMaximumWidth(100);

    portLineEdit = new QLineEdit();//QString::number(port)
    portLineEdit->setMaxLength(6);
    portLineEdit->setMaximumWidth(60);
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    connectButtonBox = new QDialogButtonBox();
    connectPushButton = connectButtonBox->addButton(tr("&Connect"), QDialogButtonBox::AcceptRole);
    disconnectPushButton = connectButtonBox->addButton(tr("&Disconnect"), QDialogButtonBox::RejectRole);
    connectPushButton->setEnabled(true);
    disconnectPushButton->setEnabled(false);
    shkpLineEdit = new  QLineEdit();
    shkmLineEdit = new  QLineEdit();
    magnetmLineEdit = new  QLineEdit();
    shkpLineEdit->setReadOnly(true);
    shkmLineEdit->setReadOnly(true);
    magnetmLineEdit->setReadOnly(true);

    depthResetPushButton = new QPushButton(tr("&Reset"));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(ipAddrLineEdit);
    hLayout->addWidget(portLineEdit);

    //mainLayout->addWidget(connectPushButton, 2,0);
    //mainLayout->addWidget(disconnectPushButton, 2,1);
    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(connectButtonBox);

    mainLayout->addWidget(new QLabel(tr("SHK+ :")));
    mainLayout->addWidget(shkpLineEdit);
    mainLayout->addWidget(new QLabel(tr("SHK- :")));
    mainLayout->addWidget(shkmLineEdit);
    mainLayout->addWidget(new QLabel(tr("MagnetMark:")));
    mainLayout->addWidget(magnetmLineEdit);

    mainLayout->addWidget(depthResetPushButton);
    //QHBoxLayout *hLayout2 = new QHBoxLayout;
    //hLayout2->addWidget(depthLineEdit);
    //hLayout2->addWidget(depthResetPushButton);

    //mainLayout->addLayout(hLayout2);
    //mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tcpSocket = new QTcpSocket(this);
    tcpSocket->setProxy(QNetworkProxy::NoProxy);

    connect( tcpSocket, SIGNAL(readyRead()), SLOT(readTcpData()) );
    connect( tcpSocket, SIGNAL(connected()), SLOT(tcpConnect()) );
    connect( tcpSocket, SIGNAL(disconnected()), SLOT(tcpDisconnect()) );
    connect( tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(tcpError(QAbstractSocket::SocketError)) );

    connect(connectButtonBox, SIGNAL(accepted()), this, SLOT(pushConnect()));
    connect(connectButtonBox, SIGNAL(rejected()), this, SLOT(pushDisconnect()));

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateDepth()));
    updateTimer->start(1000);
}

bool DepthWidget::isConnected()
{
    return (tcpSocket->state() == QAbstractSocket::ConnectedState);
}


void DepthWidget::pushConnect()
{
    tcpSocket->connectToHost(QHostAddress(ipAddrLineEdit->text()), portLineEdit->text().toInt());
    connectPushButton->setEnabled(false);
    disconnectPushButton->setEnabled(false);
    ipAddrLineEdit->setEnabled(false);
    portLineEdit->setEnabled(false);
}

void DepthWidget::pushDisconnect()
{
    tcpSocket->disconnectFromHost();
}

void DepthWidget::tcpConnect()
{
    connectPushButton->setEnabled(false);
    disconnectPushButton->setEnabled(true);
    ipAddrLineEdit->setEnabled(false);
    portLineEdit->setEnabled(false);
}

void DepthWidget::tcpDisconnect()
{
    connectPushButton->setEnabled(true);
    disconnectPushButton->setEnabled(false);
    ipAddrLineEdit->setEnabled(true);
    portLineEdit->setEnabled(true);
}

void DepthWidget::readTcpData()
{

}

void DepthWidget::tcpError(QAbstractSocket::SocketError error)
{
    connectPushButton->setEnabled(true);
    disconnectPushButton->setEnabled(false);
    ipAddrLineEdit->setEnabled(true);
    portLineEdit->setEnabled(true);
    tcpSocket->disconnectFromHost();
    QMessageBox::information(this,  tr("Tcp SocketError"), ipAddrLineEdit->text()
                             +QString(" : ")+portLineEdit->text()+QString("\n")+tcpSocket->errorString());
}

void DepthWidget::updateDepth()
{
    if (isConnected()){
        shkpLineEdit->setText(QString::number(getShkp()));
        shkmLineEdit->setText(QString::number(getShkm()));
        magnetmLineEdit->setText(QString::number(getMagnetMark()));
    }
}

QByteArray DepthWidget::getDepthData(cuart_cmd_t cmd, int size)
{
    char dataSend;
    QByteArray buf;

    dataSend=cmd;
    tcpSocket->write(&dataSend,1); //write the data itself
    tcpSocket->waitForBytesWritten();
    while ((buf.size()<size) && tcpSocket->waitForReadyRead(1000)){
        buf = tcpSocket->readAll();
    }
    if (buf.size()==size) return buf;
    else return NULL;
}

int DepthWidget::getShkp()
{
    QByteArray recv = getDepthData(CMD_GET_SHK_P, 4);
    if (recv.size()==4) return (recv[0] | recv[1]<<8 | recv[2]<<16 | recv[3]<<24);
    else return -1;
}

int DepthWidget::getShkm()
{
    QByteArray recv = getDepthData(CMD_GET_SHK_M, 4);
    if (recv.size()==4) return recv[0] | recv[1]<<8 | recv[2]<<16 | recv[3]<<24;
    else return -1;
}

int DepthWidget::getMagnetMark()
{
    QByteArray recv = getDepthData(CMD_GET_MAGNETM, 4);
    if (recv.size()==4) return recv[0] | recv[1]<<8 | recv[2]<<16 | recv[3]<<24;
    else return -1;
}


