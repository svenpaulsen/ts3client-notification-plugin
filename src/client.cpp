/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#include "client.h"
#include "database.h"

/**
 * The Client object constructor.
 */
Client::Client(uint64 schID, anyID clientID)
{
    if(!clientID && pluginSDK.getClientID(schID, &clientID))
    {
        clientID = 0;
    }

    m_schID    = schID;
    m_clientID = clientID;
}

/**
 * Returns the underlying client ID.
 */
anyID Client::getID()
{
    return m_clientID;
}

/**
 * Returns the underlying server connection handler ID.
 */
uint64 Client::getSchID()
{
    return m_schID;
}

/**
 * Returns the display name for the underlying client.
 */
QString Client::getName()
{
    char name[TS3_MAX_SIZE_CLIENT_NICKNAME_NONSDK];

    if(pluginSDK.getClientDisplayName(m_schID, m_clientID, name, TS3_MAX_SIZE_CLIENT_NICKNAME_NONSDK))
    {
        return getVarAsStr(CLIENT_NICKNAME);
    }

    return QString::fromUtf8(name);
}

/**
 * Requests updated variables from the client.
 */
QString Client::requestVars()
{
    char returnCode[BUFFSIZE_S];

    pluginSDK.createReturnCode(pluginID, returnCode, BUFFSIZE_S);

    if(pluginSDK.requestClientVariables(m_schID, m_clientID, returnCode))
    {
        return QString();
    }

    return QString::fromUtf8(returnCode);
}

/**
 * Returns a client variable as int.
 */
int Client::getVarAsInt(size_t flag, int fallback)
{
    int var;

    if(pluginSDK.getClientVariableAsInt(m_schID, m_clientID, flag, &var))
    {
        return fallback;
    }

    return var;
}

/**
 * Returns a client variable as uint64.
 */
uint64 Client::getVarAsU64(size_t flag, uint64 fallback)
{
    uint64 var;

    if(pluginSDK.getClientVariableAsUInt64(m_schID, m_clientID, flag, &var))
    {
        return fallback;
    }

    return var;
}

/**
 * Returns a client variable as QString.
 */
QString Client::getVarAsStr(size_t flag, QString fallback)
{
    char* var;

    if(pluginSDK.getClientVariableAsString(m_schID, m_clientID, flag, &var))
    {
        return fallback;
    }

    QString ret = QString::fromUtf8(var);

    pluginSDK.freeMemory(var);

    return ret;
}

/**
 * Returns TRUE if the client is on the local friend list.
 */
bool Client::isFriend()
{
    QString uid = getVarAsStr(CLIENT_UNIQUE_IDENTIFIER);

    if(uid.isEmpty())
    {
        return false;
    }

    return Database::instance()->hasContact(uid) == 0;
}
