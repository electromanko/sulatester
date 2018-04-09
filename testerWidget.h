#ifndef TESTERWIDGET_H
#define TESTERWIDGET_H

#include <QBuffer>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

class TesterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TesterWidget(QWidget *parent = nullptr);
    QComboBox *testCombo;
    QPushButton *startPushButton;
    QPushButton *stopPushButton;
    QDialogButtonBox *testButtonBox;
    QTextEdit *testTextEdit;
private:
    int testNum;
    QBuffer cotRecvBuffer;
    QBuffer rtaRecvBuffer;
    int cotSendDataCounter;
    int cotRecvDataCounter;
    int rtaSendDataCounter;
    void startTest(int testNum);
    void recvTest(int num, QByteArray data);
    void stopTest();

signals:
    void rtaDataSend(QByteArray data);
    void cotDataSend(QByteArray data);
public slots:
    void rtaDataReceive(QByteArray data);
    void cotDataReceive(QByteArray data);
private slots:
    void pushStart();
    void pushStop();
};

#endif // TESTERWIDGET_H
