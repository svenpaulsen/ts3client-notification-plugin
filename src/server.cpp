/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#include "client.h"
#include "channel.h"
#include "server.h"

/**
 * The Server object constructor.
 */
Server::Server(uint64 schID)
{
    m_schID = schID;
}

Client Server::getClientById(anyID clientID)
{
    return Client(m_schID, clientID);
}

Channel Server::getChannelByID(uint64 channelID)
{
    return Channel(m_schID, channelID);
}

/**
 * Returns the underlying server connection handler ID.
 */
uint64 Server::getSchID()
{
    return m_schID;
}

/**
 * Alias for getSchID().
 */
uint64 Server::getID()
{
    return getSchID();
}

/**
 * Returns the name of the underlying server.
 */
QString Server::getName()
{
    return getVarAsStr(VIRTUALSERVER_NAME);
}

/**
 * Requests updated variables from the server.
 */
unsigned int Server::requestVars()
{
    return pluginSDK.requestServerVariables(m_schID);
}

/**
 * Returns a server variable as int.
 */
int Server::getVarAsInt(size_t flag, int fallback)
{
    int var;

    if(pluginSDK.getServerVariableAsInt(m_schID, flag, &var))
    {
        return fallback;
    }

    return var;
}

/**
 * Returns a server variable as uint64.
 */
uint64 Server::getVarAsU64(size_t flag, uint64 fallback)
{
    uint64 var;

    if(pluginSDK.getServerVariableAsUInt64(m_schID, flag, &var))
    {
        return fallback;
    }

    return var;
}

/**
 * Returns a server variable as QString.
 */
QString Server::getVarAsStr(size_t flag, QString fallback)
{
    char* var;

    if(pluginSDK.getServerVariableAsString(m_schID, flag, &var))
    {
        return fallback;
    }

    QString ret = QString::fromUtf8(var);

    pluginSDK.freeMemory(var);

    return ret;
}
