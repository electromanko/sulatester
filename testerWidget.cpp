#include "testerWidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QThread>

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

    testTextEdit = new QTextEdit();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(testCombo);
    mainLayout->addWidget(testButtonBox);
    mainLayout->addWidget(testTextEdit);

    connect(testButtonBox, SIGNAL(accepted()), this, SLOT(pushStart()));
    connect(testButtonBox, SIGNAL(rejected()), this, SLOT(pushStop()));
    setLayout(mainLayout);
}

void TesterWidget::pushStart()
{
    startPushButton->setEnabled(false);
    stopPushButton->setEnabled(true);
    startTest(testNum=testCombo->currentData().toInt());
    startPushButton->setEnabled(true);
}

void TesterWidget::pushStop()
{
    startPushButton->setEnabled(true);
    stopPushButton->setEnabled(false);
}

void TesterWidget::startTest(int num)
{
        cotRecvDataCounter=0;
        while (num--){
          char u = (char) ('a');//qrand() % 128
          QByteArray bytes;
          bytes.append(u);
          emit cotDataSend(bytes);
          //QThread::msleep(100);
          /*testTextEdit->moveCursor (QTextCursor::End);
          testTextEdit->insertPlainText(QString(QChar(u)));
          testTextEdit->moveCursor (QTextCursor::End);*/
        };

}

void TesterWidget::recvTest(int num, QByteArray data)
{
    cotRecvDataCounter+=data.size();
    testTextEdit->moveCursor (QTextCursor::End);
    testTextEdit->insertPlainText(QString::number(cotRecvDataCounter)+" ");
    testTextEdit->moveCursor (QTextCursor::End);
    /*if (testNum==1000){
        cotRecvBuffer.write(data);
    }*/
    /*testTextEdit->moveCursor (QTextCursor::End);
    testTextEdit->insertPlainText (data.toHex()+" ");
    testTextEdit->moveCursor (QTextCursor::End);*/

}

void TesterWidget::stopTest()
{

}

void TesterWidget::rtaDataReceive(QByteArray data)
{

}

void TesterWidget::cotDataReceive(QByteArray data)
{
    recvTest(testNum, data);
}
