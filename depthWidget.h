#ifndef DEPTHWIDGET_H
#define DEPTHWIDGET_H

#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QWidget>

class DepthWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DepthWidget(QWidget *parent = nullptr);
    bool isConnected;
    QTcpSocket *tcpSocket;
    QDialogButtonBox *connectButtonBox;
    QPushButton *connectPushButton;
    QPushButton *disconnectPushButton;
    QLineEdit *ipAddrLineEdit;
    QLineEdit *portLineEdit;
signals:

public slots:
private slots:
    void pushConnect();
    void pushDisconnect();
    void tcpConnect();
    void tcpDisconnect();
    void readTcpData();
    void tcpError(QAbstractSocket::SocketError error);
};

#endif // DEPTHWIDGET_H
