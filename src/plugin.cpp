#include "plugin.h"
#include "uihelper.h"

struct TS3Functions pluginSDK;
char*               pluginID = NULL;

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
    return "1.0.0";
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
    return 20;
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
int ts3plugin_processCommand(uint64 schID, const char* command)
{
    pluginSDK.logMessage(QString("%1(): %2").arg(__FUNCTION__, command).toUtf8().data(), LogLevel_DEBUG, ts3plugin_name(), schID);

    UIHelper::getTrayIcon()->showMessage(qApp->applicationName(), command);

    return 0;
}

/**
 * Sends a notification on incoming text messages when the main window is not the active window.
 *
 * @todo cleanup code
 */
int ts3plugin_onTextMessageEvent(uint64 schID, anyID mode, anyID toID, anyID fromID, const char* fromName, const char* fromUID, const char* msg, int ignored)
{
    if(UIHelper::getMainWindow()->isActiveWindow())
    {
        return ignored;
    }

    anyID  selfID   = 0;
    uint64 chanID   = 0;
    char*  selfNick = (char*) malloc(TS3_MAX_SIZE_CLIENT_NICKNAME_NONSDK*sizeof(char));
    char*  fromNick = (char*) malloc(TS3_MAX_SIZE_CLIENT_NICKNAME_NONSDK*sizeof(char));
    char*  location;

    if(pluginSDK.getClientID(schID, &selfID) || pluginSDK.getClientDisplayName(schID, selfID, selfNick, TS3_MAX_SIZE_CLIENT_NICKNAME_NONSDK) || pluginSDK.getChannelOfClient(schID, selfID, &chanID))
    {
        free(selfNick);
        free(fromNick);
        return ignored;
    }

    if(pluginSDK.getClientDisplayName(schID, fromID, fromNick, TS3_MAX_SIZE_CLIENT_NICKNAME_NONSDK))
    {
        strncpy(fromNick, fromName, strlen(fromName)+1);
    }

    if(selfID && fromID != selfID && !ignored)
    {
        if(mode == TextMessageTarget_CLIENT || QString(msg).contains(selfNick))
        {
            if(mode == TextMessageTarget_CHANNEL && !pluginSDK.getChannelVariableAsString(schID, chanID, CHANNEL_NAME, &location))
            {
                UIHelper::getTrayIcon()->showMessage(QString("%1 (%2)").arg(fromNick, location), msg);
                pluginSDK.freeMemory(location);
            }
            else if(mode == TextMessageTarget_SERVER && !pluginSDK.getServerVariableAsString(schID, VIRTUALSERVER_NAME, &location))
            {
                UIHelper::getTrayIcon()->showMessage(QString("%1 (%2)").arg(fromNick, location), msg);
                pluginSDK.freeMemory(location);
            }
            else
            {
                UIHelper::getTrayIcon()->showMessage(QString("%1").arg(fromNick), msg);
            }
        }
    }

    free(selfNick);
    free(fromNick);
    return ignored;
}
