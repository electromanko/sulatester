#include "mainwindow.h"
#include "connection_widget.h"
#include "testerWidget.h"

#include <QtWidgets>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mainWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    QGroupBox *cotGroupBox = new QGroupBox(tr("COT"));
    QVBoxLayout *cotLayout = new QVBoxLayout();
    QGroupBox *rtaGroupBox = new QGroupBox(tr("RTA"));
    QVBoxLayout *rtaLayout = new QVBoxLayout();

    CotConnectionWidget = new ConnectionWidget("192.168.0.13",8899);
    RtaConnectionWidget = new ConnectionWidget("192.168.0.14",8899);
    testerWidget = new TesterWidget();
    // Loop
    connect(RtaConnectionWidget,SIGNAL(dataReceive(QByteArray)),
            RtaConnectionWidget,SLOT(dataSend(QByteArray)));

    /*connect(RtaConnectionWidget,SIGNAL(dataReceive(QByteArray)),
            testerWidget,SLOT(cotDataReceive(QByteArray)));
    connect(testerWidget,SIGNAL(cotDataSend(QByteArray)),
            CotConnectionWidget,SLOT(dataSend(QByteArray)));*/
    /*connect(testerWidget,SIGNAL(cotDataSend(QByteArray)),
            testerWidget,SLOT(cotDataReceive(QByteArray)));*/
    connect(testerWidget,SIGNAL(cotDataSend(QByteArray)),
            CotConnectionWidget,SLOT(dataSend(QByteArray)));
    connect(CotConnectionWidget,SIGNAL(dataReceive(QByteArray)),
                testerWidget,SLOT(cotDataReceive(QByteArray)));

    cotLayout->addWidget(CotConnectionWidget);
    cotLayout->setSpacing(0);
    cotLayout->setMargin(0);
    cotGroupBox->setLayout(cotLayout);
    rtaLayout->addWidget(RtaConnectionWidget);
    rtaLayout->setSpacing(0);
    rtaLayout->setMargin(0);
    rtaGroupBox->setLayout(rtaLayout);

    vLayout->addWidget(cotGroupBox);
    vLayout->addWidget(rtaGroupBox);
    /*vLayout->addWidget(CotConnectionWidget);
    vLayout->addWidget(RtaConnectionWidget);*/
    vLayout->addStretch();
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    mainLayout->addLayout(vLayout);
    mainLayout->addWidget(testerWidget);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(2);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    createActions();
    createMenus();

}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions(){
    actionQuit = new QAction(tr("&Quit"), this);
    actionQuit->setShortcuts(QKeySequence::Quit);
    actionQuit->setStatusTip(tr("Quit from app"));
    //connect(quitAction, &QAction::triggered, this, &MainWindow::newFile);

    actionAbout = new QAction(tr("&About"), this);
    actionAbout->setShortcuts(QKeySequence::Cancel);
    actionAbout->setStatusTip(tr("Quit from app"));
}

void MainWindow::createMenus(){
        fileMenu = menuBar()->addMenu(tr("&File"));
        fileMenu->addSeparator();
        fileMenu->addAction(actionQuit);

        fileMenu = menuBar()->addMenu(tr("&Help"));
        fileMenu->addSeparator();
        fileMenu->addAction(actionAbout);

}
