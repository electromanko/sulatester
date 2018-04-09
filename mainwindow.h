#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QWidget *mainWidget;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QWidget *CotConnectionWidget;
    QWidget *RtaConnectionWidget;
    QWidget *testerWidget;

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *actionQuit;
    QAction *actionAbout;

    void createActions();
    void createMenus();
};

#endif // MAINWINDOW_H
