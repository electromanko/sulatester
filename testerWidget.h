#ifndef TESTERWIDGET_H
#define TESTERWIDGET_H

#include <QBuffer>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QProgressBar>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

class TesterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TesterWidget(QWidget *parent = nullptr);
    QComboBox *testCombo;
    QComboBox *seqCombo;
    QProgressBar *sendProgressBar;
    QProgressBar *receiveProgressBar;
    QPushButton *startPushButton;
    QPushButton *stopPushButton;
    QDialogButtonBox *testButtonBox;
    QTextEdit *testTextEdit;
private:
    int testNum;
    QTimer *timeoutTimer;
    QTime *measureTimer;
    QBuffer cotRecvBuffer;
    QBuffer rtaRecvBuffer;
    QByteArray cotSendData;
    QByteArray cotRecvData;
    int rtaSendDataCounter;
    bool testRuned=false;
    void startTest(int testNum);
    void recvTest(int num, QByteArray data);
    long int compareData(QByteArray &send, QByteArray &recv, int *first_err_byte_num);

signals:
    void rtaDataSend(QByteArray data);
    void cotDataSend(QByteArray data);
public slots:
    void rtaDataReceive(QByteArray data);
    void cotDataReceive(QByteArray data);
private slots:
    void pushStart();
    void pushStop();
    void stopTest();
};

#endif // TESTERWIDGET_H
