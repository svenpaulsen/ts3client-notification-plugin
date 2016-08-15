/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#include "plugin.h"
#include "server.h"
#include "client.h"
#include "channel.h"
#include "uihelper.h"

/**
 * Returns the name of the plugin.
 */
const char* ts3plugin_name()
{
    return "Notifications";
}

/**
 * Returns the version number of the plugin.
 */
const char* ts3plugin_version()
{
    return "1.0.1";
}

/**
 * Returns the author of the plugin.
 */
const char* ts3plugin_author()
{
    return "Sven Paulsen";
}

/**
 * Returns the description of the plugin.
 */
const char* ts3plugin_description()
{
    return "Plugin to send simple desktop notifications when being mentioned or receiving private messages";
}

/**
 * Returns the API version number of the plugin.
 */
int ts3plugin_apiVersion()
{
    return PLUGIN_API;
}

/**
 * Loads the plugin.
 */
int ts3plugin_init()
{
    if(!UIHelper::getMainWindow())
    {
        pluginSDK.logMessage(QString("Failed to initialize plugin; no QMainWindow available").toUtf8().data(), LogLevel_ERROR, ts3plugin_name(), 0);

        return 1;
    }

    pluginSDK.logMessage(QString("Plugin initialized; version %1 built on %2 %3").arg(ts3plugin_version(), __DATE__, __TIME__).toUtf8().data(), LogLevel_INFO, ts3plugin_name(), 0);

    return 0;
}

/**
 * Unloads the plugin.
 */
void ts3plugin_shutdown()
{
    if(pluginID)
    {
        free(pluginID);
        pluginID = NULL;
    }
}

/**
 * Registers TeamSpeak 3 callback functions.
 */
void ts3plugin_setFunctionPointers(const struct TS3Functions functions)
{
    pluginSDK = functions;
}

/**
 * Registers an auto-generated plugin ID used for return codes, commands, menus and hotkeys.
 */
void ts3plugin_registerPluginID(const char* id)
{
    pluginID = (char*) malloc((strlen(id) +1)*sizeof(char));

    strncpy(pluginID, id, strlen(id)+1);
}

/**
 * Returns the command keyword of the plugin.
 */
const char* ts3plugin_commandKeyword()
{
    return "notify";
}

/**
 * Sends a test notification when invoking a command using the plugin keyword.
 */
int ts3plugin_processCommand(uint64 schID, const char* cmd)
{
    QSystemTrayIcon* ico = UIHelper::getTrayIcon();
    QString          str = UIHelper::removeBBCode(cmd);

    pluginSDK.logMessage(QString("%1(): %2").arg(__FUNCTION__, str).toUtf8().data(), LogLevel_DEBUG, ts3plugin_name(), schID);

    if(!ico || !ico->isVisible())
    {
        pluginSDK.logMessage(QString("Failed to send notification; no QSystemTrayIcon available").toUtf8().data(), LogLevel_WARNING, ts3plugin_name(), schID);

        return 1;
    }

    ico->showMessage(qApp->applicationName(), str, QSystemTrayIcon::NoIcon);

    return 0;
}

/**
 * Sends a notification on incoming text messages when the main window is not the active window.
 *
 * @todo cleanup code
 */
int ts3plugin_onTextMessageEvent(uint64 schID, anyID mode, anyID rcvID, anyID srcID, const char* srcName, const char* srcUID, const char* msg, int ignored)
{
    if(UIHelper::getMainWindow()->isActiveWindow())
    {
        return ignored;
    }

    Server server(schID);

    Client self = server.getClientById(rcvID);
    Client from = server.getClientById(srcID);

    if(self.getName().isNull())
    {
        return ignored;
    }

    QSystemTrayIcon* ico = UIHelper::getTrayIcon();
    QString          str = UIHelper::removeBBCode(msg);

    if(!ico || !ico->isVisible())
    {
        return ignored;
    }

    if(self.getID() && from.getID() != self.getID() && !ignored)
    {
        if(mode == TextMessageTarget_CLIENT || str.contains(self.getName()))
        {
            switch(mode)
            {
            case TextMessageTarget_SERVER:
                ico->showMessage(server.getVarAsStr(VIRTUALSERVER_NAME), QString("%1:\n%2").arg(from.getName(), str), QSystemTrayIcon::NoIcon);
                break;

            case TextMessageTarget_CHANNEL:
                ico->showMessage(server.getVarAsStr(VIRTUALSERVER_NAME), QString("%1 in %2:\n%3").arg(from.getName(), server.getChannelByID().getName(), str), QSystemTrayIcon::NoIcon);
                break;

            case TextMessageTarget_CLIENT:
                ico->showMessage(from.getName(), str, QSystemTrayIcon::NoIcon);
                break;

            default:
                ico->showMessage(QString("%1 (%2)").arg(srcName, srcUID), str, QSystemTrayIcon::NoIcon);
            }
        }
    }

    return ignored;
}
