/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#include "plugin.h"

#include "cache.h"
#include "client.h"
#include "channel.h"
#include "server.h"
#include "database.h"
#include "uihelper.h"

/**
 * Returns the name of the plugin.
 */
const char* ts3plugin_name()
{
    return PLUGIN_NAME;
}

/**
 * Returns the version number of the plugin.
 */
const char* ts3plugin_version()
{
    return PLUGIN_VER;
}

/**
 * Returns the author of the plugin.
 */
const char* ts3plugin_author()
{
    return "Sven 'ScP' Paulsen";
}

/**
 * Returns the description of the plugin.
 */
const char* ts3plugin_description()
{
    return "Plugin to send simple desktop notifications when being mentioned or receiving private messages.";
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
        pluginSDK.logMessage("Failed to initialize plugin; no QMainWindow available", LogLevel_ERROR, PLUGIN_NAME, 0);

        return 1;
    }

    if(!Database::instance()->isValid())
    {
        pluginSDK.logMessage("Failed to initialize plugin; no QSqlDriver available", LogLevel_ERROR, PLUGIN_NAME, 0);

        return 1;
    }

    pluginSDK.logMessage(QString("Plugin initialized; version %1 (API %2) built on %3 %4").arg(ts3plugin_version(), QString::number(ts3plugin_apiVersion()), __DATE__, __TIME__).toUtf8().data(), LogLevel_INFO, PLUGIN_NAME, 0);

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

    pluginSDK.logMessage(QString("%1(): %2").arg(__FUNCTION__, str).toUtf8().data(), LogLevel_DEBUG, PLUGIN_NAME, schID);

    if(!ico || !ico->isVisible())
    {
        pluginSDK.logMessage("Failed to send notification; no QSystemTrayIcon available", LogLevel_WARNING, PLUGIN_NAME, schID);

        return 1;
    }

    ico->showMessage(qApp->applicationName(), str, QSystemTrayIcon::NoIcon);

    return 0;
}

/**
 * Clears the cache for the specified server connection handler when disconnecting.
 */
void onConnectStatusChangeEvent(uint64 schID, int status, unsigned int error)
{
    if(status == STATUS_DISCONNECTED)
    {
        Cache::instance()->clear(schID);
    }

    Q_UNUSED(error)
}

/**
 * Sends a notification on incoming text messages when the main window is not the active window.
 */
int ts3plugin_onTextMessageEvent(uint64 schID, anyID mode, anyID rcvID, anyID srcID, const char* srcName, const char* srcUID, const char* msg, int ignored)
{
    if(UIHelper::getMainWindow()->isActiveWindow() && pluginSDK.getCurrentServerConnectionHandlerID() == schID)
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
                ico->showMessage(server.getName(), QString("%1:\n%2").arg(from.getName(), str), QSystemTrayIcon::NoIcon);
                break;

            case TextMessageTarget_CHANNEL:
                ico->showMessage(server.getName(), QString("%1 in %2:\n%3").arg(from.getName(), server.getChannelByID().getName(), str), QSystemTrayIcon::NoIcon);
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

/**
 * Sends a notification when a friend connects, disconnects or joins/leaves your current channel.
 */
void ts3plugin_onClientMoveEvent(uint64 schID, anyID clientID, uint64 frChanID, uint64 toChanID, int visibility, const char* msg)
{
    if(UIHelper::getMainWindow()->isActiveWindow() && pluginSDK.getCurrentServerConnectionHandlerID() == schID)
    {
        return;
    }

    Server server(schID);

    Client  self = server.getClientById();
    Channel chan = server.getChannelByID();

    if(self.getName().isNull())
    {
        return;
    }

    QSystemTrayIcon* ico = UIHelper::getTrayIcon();

    if(!ico || !ico->isVisible() || clientID == self.getID())
    {
        return;
    }

    if(visibility == ENTER_VISIBILITY)
    {
        Cache::instance()->addClient(schID, clientID);
    }

    QString nickname = Cache::instance()->getClient(schID, clientID).first;
    QString uniqueID = Cache::instance()->getClient(schID, clientID).second;
    Channel frChan   = server.getChannelByID(frChanID);
    Channel toChan   = server.getChannelByID(toChanID);

    if(uniqueID.isEmpty() || Database::instance()->hasContact(uniqueID) != 0)
    {
        return;
    }

    switch(visibility)
    {
    case ENTER_VISIBILITY:
        if(!frChanID)
        {
            ico->showMessage(server.getName(), qApp->translate("ServerView", "%1 connected to channel %2").arg(nickname, toChan.getName()), QSystemTrayIcon::NoIcon);
        }
        else
        {
            ico->showMessage(server.getName(), qApp->translate("ServerView", "%1 appears, coming from channel %2").arg(nickname, frChan.getName()), QSystemTrayIcon::NoIcon);
        }
        break;

    case LEAVE_VISIBILITY:
        if(!toChanID)
        {
            QString message(msg);

            if(message.isEmpty())
            {
                ico->showMessage(server.getName(), qApp->translate("ServerView", "%1 disconnected").arg(nickname), QSystemTrayIcon::NoIcon);
            }
            else
            {
                ico->showMessage(server.getName(), qApp->translate("ServerView", "%1 disconnected %2").arg(nickname, "(" + message + ")"), QSystemTrayIcon::NoIcon);
            }
        }
        else
        {
            ico->showMessage(server.getName(), qApp->translate("ServerView", "%1 left heading to channel %2").arg(nickname, toChan.getName()), QSystemTrayIcon::NoIcon);
        }
        break;

    default:
        if(toChanID == chan.getID() || frChanID == chan.getID())
        {
            ico->showMessage(server.getName(), qApp->translate("ServerView", "%1 switched from channel %2 to %3").arg(nickname, frChan.getName(), toChan.getName()), QSystemTrayIcon::NoIcon);
        }
    }

    if(visibility == LEAVE_VISIBILITY)
    {
        Cache::instance()->remClient(schID, clientID);
    }
}

/**
 * Maintains an internal list of clients in your view.
 */
void ts3plugin_onClientMoveSubscriptionEvent(uint64 schID, anyID clientID, uint64 frChanID, uint64 toChanID, int visibility)
{
    if(!visibility)
    {
        Cache::instance()->addClient(schID, clientID);
    }

    Q_UNUSED(frChanID)
    Q_UNUSED(toChanID)
}
