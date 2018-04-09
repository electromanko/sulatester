#ifndef CONNECTION_WIDGET_H
#define CONNECTION_WIDGET_H

#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QWidget>


class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionWidget(QString addr, int port, QWidget *parent = nullptr);
    virtual ~ConnectionWidget();
    QPushButton *connectPushButton;
    QPushButton *disconnectPushButton;
    QDialogButtonBox *connectButtonBox;
    QLineEdit *ipAddrLineEdit;
    QLineEdit *portLineEdit;

private:
    bool connected;
    QTcpSocket *tcpSocket;
signals:
    void dataReceive(QByteArray data);
public slots:
    void dataSend(QByteArray data);
private slots:
    void pushConnect();
    void pushDisconnect();
    void tcpConnect();
    void tcpDisconnect();
    void readTcpData();
    void tcpError(QAbstractSocket::SocketError error);
};

#endif // CONNECTION_WIDGET_H
