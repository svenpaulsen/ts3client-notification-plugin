/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#include "client.h"
#include "channel.h"

/**
 * The Channel object constructor.
 */
Channel::Channel(uint64 schID, uint64 channelID)
{
    if(!channelID && pluginSDK.getChannelOfClient(schID, Client(schID).getID(), &channelID))
    {
        channelID = 0;
    }

    m_schID     = schID;
    m_channelID = channelID;
}

/**
 * Returns the underlying server connection handler ID.
 */
uint64 Channel::getSchID()
{
    return m_schID;
}

/**
 * Returns the underlying channel ID.
 */
uint64 Channel::getID()
{
    return m_channelID;
}

/**
 * Returns the name of the underlying channel.
 */
QString Channel::getName()
{
    return getVarAsStr(CHANNEL_NAME);
}

/**
 * Requests updated variables from the channel.
 */
QString Channel::requestVars()
{
    char returnCode[BUFFSIZE_S];

    pluginSDK.createReturnCode(pluginID, returnCode, BUFFSIZE_S);

    if(pluginSDK.requestChannelDescription(m_schID, m_channelID, returnCode))
    {
        return QString();
    }

    return QString::fromUtf8(returnCode);
}

/**
 * Returns a channel variable as int.
 */
int Channel::getVarAsInt(size_t flag, int fallback)
{
    int var;

    if(pluginSDK.getChannelVariableAsInt(m_schID, m_channelID, flag, &var))
    {
        return fallback;
    }

    return var;
}

/**
 * Returns a channel variable as uint64.
 */
uint64 Channel::getVarAsU64(size_t flag, uint64 fallback)
{
    uint64 var;

    if(pluginSDK.getChannelVariableAsUInt64(m_schID, m_channelID, flag, &var))
    {
        return fallback;
    }

    return var;
}

/**
 * Returns a channel variable as QString.
 */
QString Channel::getVarAsStr(size_t flag, QString fallback)
{
    char* var;

    if(pluginSDK.getChannelVariableAsString(m_schID, m_channelID, flag, &var))
    {
        return fallback;
    }

    QString ret = QString::fromUtf8(var);

    pluginSDK.freeMemory(var);

    return ret;
}
