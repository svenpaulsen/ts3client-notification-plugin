/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#include "client.h"
#include "channel.h"
#include "server.h"
#include "cache.h"

/**
 * The Cache object constructor.
 */
Cache::Cache() : Singleton<Cache>()
{}

/**
 * The Cache object destructor.
 */
Cache::~Cache()
{}

/**
 * Returns a pair of client nickname and unique identifier from the cache.
 */
QPair<QString, QString> Cache::getClient(uint64 schID, anyID clientID)
{
    return m_cache[schID][clientID];
}

/**
 * Adds or updates a client entry in the cache.
 */
void Cache::addClient(uint64 schID, anyID clientID)
{
    Server server(schID);

    Client  self     = server.getClientById();
    Client  client   = server.getClientById(clientID);

    QString nickname = client.getName();
    QString uniqueID = client.getVarAsStr(CLIENT_UNIQUE_IDENTIFIER);

    if(clientID != self.getID() && !nickname.isEmpty())
    {
        m_cache[schID][clientID] = QPair<QString, QString>(nickname, uniqueID);
    }
}

/**
 * Removes a client entry from the cache.
 */
void Cache::remClient(uint64 schID, anyID clientID)
{
    m_cache[schID].remove(clientID);

    if(!m_cache[schID].count())
    {
        m_cache.remove(schID);
    }
}

/**
 * Wipes the cache.
 */
void Cache::clear(uint64 schID)
{
    if(!schID)
    {
        m_cache.clear();
    }
    else
    {
        m_cache.remove(schID);
    }
}
