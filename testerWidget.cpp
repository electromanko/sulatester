#include "testerWidget.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QThread>
#include <QTimer>
#define RECV_TIMEOUT_MS 5000

TesterWidget::TesterWidget(QWidget *parent) : QWidget(parent)
{
    testCombo = new QComboBox();
    testCombo->setEditable(false);
    //testCombo->addItem(QString(""));
    testCombo->addItem(QString("Send->Receive 1B"),QVariant(1));
    testCombo->addItem(QString("Send->Receive 127B"),QVariant(127));
    testCombo->addItem(QString("Send->Receive 200B"),QVariant(200));
    testCombo->addItem(QString("Send->Receive 400B"),QVariant(400));
    testCombo->addItem(QString("Send->Receive 1kB"),QVariant(1000));
    testCombo->addItem(QString("Send->Receive 100kB"),QVariant(100000));
    testCombo->addItem(QString("Send->Receive 1MB"),QVariant(1000000));
    testCombo->addItem(QString("Send->Receive 10MB"),QVariant(10000000));
    testCombo->addItem(QString("Send->Receive 100MB"),QVariant(100000000));
    //testCombo->lineEdit()->setCursorPosition(0);

    testButtonBox = new QDialogButtonBox();
    startPushButton = testButtonBox->addButton(tr("Start"),QDialogButtonBox::AcceptRole);
    stopPushButton = testButtonBox->addButton(tr("Stop"), QDialogButtonBox::RejectRole);
    startPushButton->setEnabled(true);
    stopPushButton->setEnabled(false);

    sendProgressBar = new QProgressBar();
    sendProgressBar->setMinimum(0);
    sendProgressBar->setMaximum(100);
    sendProgressBar->setFormat("%v");
    receiveProgressBar = new QProgressBar();
    receiveProgressBar->setMinimum(0);
    receiveProgressBar->setMaximum(100);
    receiveProgressBar->setFormat("%v");
    //receiveProgressBar->setAlignment(Qt::AlignCenter);

    testTextEdit = new QTextEdit();
    testTextEdit->setReadOnly(true);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(testCombo);
    mainLayout->addWidget(testButtonBox);
    QLayout *shlayout= new QHBoxLayout();
        shlayout->addWidget(new QLabel(tr("Sended:")));
        shlayout->addWidget(sendProgressBar);
    //mainLayout->addWidget(new QLabel(tr("Sended:")));
    //mainLayout->addWidget(sendProgressBar);
    mainLayout->addLayout(shlayout);
    QLayout *rhlayout= new QHBoxLayout();
        rhlayout->addWidget(new QLabel(tr("Received:")));
        rhlayout->addWidget(receiveProgressBar);
    mainLayout->addLayout(rhlayout);
    mainLayout->addWidget(testTextEdit);

    timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    measureTimer = new QTime();

    connect(testButtonBox, SIGNAL(accepted()), this, SLOT(pushStart()));
    connect(testButtonBox, SIGNAL(rejected()), this, SLOT(pushStop()));
    connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(stopTest()));

    setLayout(mainLayout);
}

void TesterWidget::pushStart()
{
    //stopPushButton->setEnabled(false);

    startTest(testCombo->currentData().toInt());

}

void TesterWidget::pushStop()
{
    //startPushButton->setEnabled(true);
    //stopPushButton->setEnabled(false);
}

void TesterWidget::startTest(int num)
{
        unsigned char u;
        QByteArray bytes;
        testNum=num;
        startPushButton->setEnabled(false);
        testCombo->setEnabled(false);
        sendProgressBar->setMaximum(testNum);
        receiveProgressBar->setMaximum(testNum);
        testRuned=true;

        cotSendData.clear();
        cotRecvData.clear();
        receiveProgressBar->setValue(cotRecvData.size());
        sendProgressBar->setValue(cotSendData.size());

        while (num--){
          cotSendData.append((unsigned char )(rand() % 256));
          //QThread::msleep(100);
          /*testTextEdit->moveCursor (QTextCursor::End);
          testTextEdit->insertPlainText(QString(QChar(u)));
          testTextEdit->moveCursor (QTextCursor::End);*/
        };

        emit cotDataSend(cotSendData);
        sendProgressBar->setValue(cotSendData.size());

        //QString("\n%1 Тест на передачу %2 байт запущен…\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(testNum);
        testTextEdit->append(QString(tr("\n%1 Тест на передачу %2 байт запущен…"))
                             .arg(QDateTime::currentDateTime()
                                  .toString("yyyy.MM.dd hh:mm:ss"))
                             .arg(testNum));
        timeoutTimer->start(RECV_TIMEOUT_MS);
        measureTimer->start();
}

void TesterWidget::recvTest(int num, QByteArray data)
{
    if (testRuned){
        cotRecvData.append(data);
        receiveProgressBar->setValue(cotRecvData.size());
        if (cotSendData.size()==cotRecvData.size()) {
            timeoutTimer->stop();
            stopTest();
        } else {
            timeoutTimer->start(RECV_TIMEOUT_MS);
        }
    }
    /*testTextEdit->moveCursor (QTextCursor::End);
    testTextEdit->insertPlainText(QString::number(cotRecvDataCounter)+" ");
    testTextEdit->moveCursor (QTextCursor::End);*/

    /*if (testNum==1000){
        cotRecvBuffer.write(data);
    }*/
    /*testTextEdit->moveCursor (QTextCursor::End);
    testTextEdit->insertPlainText (data.toHex()+" ");
    testTextEdit->moveCursor (QTextCursor::End);*/

}

void TesterWidget::stopTest()
{
    int testTime= measureTimer->elapsed();
    double speed = (double)(cotRecvData.size())/(double)(testTime?testTime:1);
    testRuned=false;
    if (cotSendData.size()==cotRecvData.size()){
        testTextEdit->append(QString(tr("%1 Тест успешно завершён…"))
                         .arg(QDateTime::currentDateTime()
                              .toString("yyyy.MM.dd hh:mm:ss"))
                );
    }
    else {
        testTextEdit->append(QString(tr("%1 Тест остановлен по таймауту…\n      Недополучено: %2 байт"))
                         .arg(QDateTime::currentDateTime()
                              .toString("yyyy.MM.dd hh:mm:ss"))
                         .arg(cotSendData.size()-cotRecvData.size())
                );
    };

    testTextEdit->append(QString(tr("      Время приёма: %1 мс\n"\
                                    "      Средняя скорость: %2 КБ/с (%3 Кбит/c)\n"\
                                    "      Ошибок: %4 бит"))
                         .arg(testTime)
                         .arg(speed)
                         .arg(speed*8)
                         .arg(compareData(cotSendData,cotRecvData))
                         );

    testCombo->setEnabled(true);
    startPushButton->setEnabled(true);
}

void TesterWidget::rtaDataReceive(QByteArray data)
{

}

void TesterWidget::cotDataReceive(QByteArray data)
{
    recvTest(testNum, data);
}

long int TesterWidget::compareData(QByteArray &send, QByteArray &recv){
    long int cnt_err=0;
    int num = recv.size()<send.size()? recv.size():send.size();
    while (num--){
        for (int i=0; i<8;i++){
           if (((send[num]>>i)&0x01) != ((recv[num]>>i)&0x01)) cnt_err++;
        }
    }
    return cnt_err;
}
