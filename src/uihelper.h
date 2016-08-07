#ifndef UIHELPER_H
#define UIHELPER_H

#include <QApplication>
#include <QMainWindow>
#include <QSystemTrayIcon>

class UIHelper
{
public:
    static QMainWindow*     getMainWindow();
    static QSystemTrayIcon* getTrayIcon();
};

#endif // UIHELPER_H
