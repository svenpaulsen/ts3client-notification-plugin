/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#ifndef CACHE_H
#define CACHE_H

#include "singleton.h"
#include "shared.h"

class Cache : public Singleton<Cache>
{
    friend class Singleton<Cache>;

    QHash<uint64, QHash<anyID, QPair<QString, QString>>> m_cache;

    Cache();
    ~Cache();

public:
    QPair<QString, QString> getClient(uint64 schID, anyID clientID);
    void                 addClient(uint64 schID, anyID clientID);
    void                 remClient(uint64 schID, anyID clientID);
    void                 clear(uint64 schID = 0);
};

#endif // CACHE_H
