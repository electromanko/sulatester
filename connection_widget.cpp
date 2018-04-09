#include "connection_widget.h"

#include <QGridLayout>

ConnectionWidget::ConnectionWidget(QString addr, int port, QWidget *parent) : QWidget(parent)
{
    connected=false;
    ipAddrLineEdit = new QLineEdit(addr);
    ipAddrLineEdit->setMaxLength(16);
    ipAddrLineEdit->setMaximumWidth(100);
    portLineEdit = new QLineEdit(QString::number(port));
    portLineEdit->setMaxLength(6);
    portLineEdit->setMaximumWidth(60);
    connectButtonBox = new QDialogButtonBox();
    //connectButtonBox->addButton(connectPushButton);
    //connectButtonBox->addButton(disconnectPushButton);
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

    connect( tcpSocket, SIGNAL(readyRead()), SLOT(readTcpData()) );
    connect( tcpSocket, SIGNAL(connected()), SLOT(tcpConnect()) );
    connect( tcpSocket, SIGNAL(disconnected()), SLOT(tcpDisconnect()) );
    connect( tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(tcpError(QAbstractSocket::SocketError)) );

    connect(connectButtonBox, SIGNAL(accepted()), this, SLOT(pushConnect()));
    connect(connectButtonBox, SIGNAL(rejected()), this, SLOT(pushDisconnect()));

    //connect(connectButtonBox, &QDialogButtonBox::rejected, this, SLOT(tcpDisconnect()));
}

ConnectionWidget::~ConnectionWidget()
{

}

void ConnectionWidget::dataSend(QByteArray data)
{
    if (connected==true) tcpSocket->write(data);
}

void ConnectionWidget::tcpConnect()
{
    connected=true;
    connectPushButton->setEnabled(false);
    disconnectPushButton->setEnabled(true);
    ipAddrLineEdit->setEnabled(false);
    portLineEdit->setEnabled(false);
}

void ConnectionWidget::tcpDisconnect()
{
    connected=false;
    connectPushButton->setEnabled(true);
    disconnectPushButton->setEnabled(false);
    ipAddrLineEdit->setEnabled(true);
    portLineEdit->setEnabled(true);
}

void ConnectionWidget::tcpError(QAbstractSocket::SocketError error)
{
    connected=false;
    connectPushButton->setEnabled(true);
    disconnectPushButton->setEnabled(false);
    ipAddrLineEdit->setEnabled(true);
    portLineEdit->setEnabled(true);
}

void ConnectionWidget::pushConnect()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->connectToHost(ipAddrLineEdit->text(), portLineEdit->text().toInt());
    connectPushButton->setEnabled(false);
    disconnectPushButton->setEnabled(false);
    ipAddrLineEdit->setEnabled(false);
    portLineEdit->setEnabled(false);
}

void ConnectionWidget::pushDisconnect()
{
    tcpSocket->disconnectFromHost();
}

void ConnectionWidget::readTcpData()
{
    emit dataReceive(tcpSocket->readAll());
}
