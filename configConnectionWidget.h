#ifndef CONFIGCONNECTIONWIDGET_H
#define CONFIGCONNECTIONWIDGET_H

#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTcpSocket>
#include <QTextEdit>
#include <QTimer>
#include <QWidget>

class ConfigConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigConnectionWidget(QWidget *parent = nullptr);
    QPushButton *connectPushButton;
    QTextEdit *configTextEdit;
    QLineEdit *ipAddrLineEdit;
    QLineEdit *portLineEdit;
    void tcpConnect(QHostAddress ipaddr, int port);
private:
    QTcpSocket *tcpSocket;
    QTimer *updateTimer;
signals:
    void dataSend(QByteArray data);
public slots:
    void dataReceive();
    void updateInfo(void );

};

#endif // CONFIGCONNECTIONWIDGET_H
