#include "depthWidget.h"

#include <QHBoxLayout>
#include <QIntValidator>
#include <QNetworkProxy>
#include <QNetworkInterface>

DepthWidget::DepthWidget(QWidget *parent) : QWidget(parent)
{
    isConnected=false;
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

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(ipAddrLineEdit);
    hLayout->addWidget(portLineEdit);

    //mainLayout->addWidget(connectPushButton, 2,0);
    //mainLayout->addWidget(disconnectPushButton, 2,1);
    mainLayout->addLayout(hLayout);
    mainLayout->addWidget(connectButtonBox);
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
    isConnected=true;
    connectPushButton->setEnabled(false);
    disconnectPushButton->setEnabled(true);
    ipAddrLineEdit->setEnabled(false);
    portLineEdit->setEnabled(false);
}

void DepthWidget::tcpDisconnect()
{
    isConnected=false;
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

}

