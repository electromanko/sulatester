#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configConnectionWidget.h"
#include "connection_widget.h"

#include <QMainWindow>
#include <QMenu>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget *mainWidget;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void startConf();
private:
    ConnectionWidget *CotConnectionWidget;
    ConnectionWidget *RtaConnectionWidget;
    ConfigConnectionWidget  *ModemConnectionWidget;
    QWidget *testerWidget;

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *actionQuit;
    QAction *actionAbout;

    void createActions();
    void createMenus();
};

#endif // MAINWINDOW_H
