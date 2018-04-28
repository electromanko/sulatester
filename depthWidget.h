#ifndef DEPTHWIDGET_H
#define DEPTHWIDGET_H

#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QTimer>
#include <QWidget>

typedef enum{
  CMD_GET_SHK_P=0xa1,
  CMD_GET_SHK_M=0xa2,
  CMD_GET_MAGNETM=0xa3,
  CMD_RESET_ALL_COUNTER=0xb1,
  CMD_GET_ERR=0xe1,
  CMD_RESET_ERR=0xe0,
} cuart_cmd_t;

class DepthWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DepthWidget(QWidget *parent = nullptr);
    bool isConnected();
    QTcpSocket *tcpSocket;
    QDialogButtonBox *connectButtonBox;
    QPushButton *connectPushButton;
    QPushButton *disconnectPushButton;
    QLineEdit   *ipAddrLineEdit;
    QLineEdit   *portLineEdit;
    QLineEdit   *shkpLineEdit;
    QLineEdit   *shkmLineEdit;
    QLineEdit   *magnetmLineEdit;
    QPushButton *depthResetPushButton;
    QTimer *updateTimer;

private:
    QByteArray getDepthData(cuart_cmd_t cmd, int size);
    int getShkp();
    int getShkm();
    int getMagnetMark();
signals:

public slots:
private slots:
    void pushConnect();
    void pushDisconnect();
    void tcpConnect();
    void tcpDisconnect();
    void readTcpData();
    void tcpError(QAbstractSocket::SocketError error);
    void updateDepth();
};

#endif // DEPTHWIDGET_H
