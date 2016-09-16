/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#include "uihelper.h"

/**
 * Returns the main window object used within the TeamSpeak 3 Client UI if available.
 */
QMainWindow* UIHelper::getMainWindow()
{
    QWidgetList widgets = qApp->allWidgets();

    for(QWidgetList::iterator i = widgets.begin(); i != widgets.end(); ++i)
    {
        if((*i)->inherits("QMainWindow")) return (QMainWindow*) (*i);
    }

    return NULL;
}

/**
 * Returns the tray icon object used within the TeamSpeak 3 Client UI if available.
 */
QSystemTrayIcon* UIHelper::getTrayIcon()
{
    QMainWindow* win = getMainWindow();

    if(!win)
    {
        return NULL;
    }

    QSystemTrayIcon* ico = win->findChild<QSystemTrayIcon*>();

    if(!ico->supportsMessages())
    {
        return NULL;
    }

    return ico;
}

/**
 * Returns the full path of the TeamSpeak 3 Client application directory.
 */
QString UIHelper::getAppPath()
{
    char path[BUFFSIZE_S];

    pluginSDK.getAppPath(path, BUFFSIZE_S);

    return QString(path);
}

/**
 * Returns the full path of the TeamSpeak 3 Client settings directory.
 */
QString UIHelper::getConfigPath()
{
    char path[BUFFSIZE_S];

    pluginSDK.getConfigPath(path, BUFFSIZE_S);

    return QString(path);
}

/**
 * Quick-and-dirty function to remove BBCode tags from a specified string.
 */
QString UIHelper::removeBBCode(QString str)
{
    return str.replace(QRegExp("\\[[^\\]]+\\]"), "");
}
