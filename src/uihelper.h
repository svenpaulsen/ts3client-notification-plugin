/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#ifndef UIHELPER_H
#define UIHELPER_H

#include "shared.h"

class UIHelper
{
public:
    static QMainWindow*     getMainWindow();
    static QSystemTrayIcon* getTrayIcon();
    static QString          getAppPath();
    static QString          getConfigPath();
    static QString          removeBBCode(QString str);
};

#endif // UIHELPER_H
