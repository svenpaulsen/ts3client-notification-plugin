/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#ifndef SERVER_H
#define SERVER_H

#include "shared.h"

class Server
{
    uint64 m_schID;

public:
    Server(uint64 schID);

    Client       getClientById(anyID clientID = 0);
    Channel      getChannelByID(uint64 channelID = 0);

    uint64       getID();
    uint64       getSchID();
    QString      getName();
    unsigned int requestVars();
    int          getVarAsInt(size_t flag, int fallback = 0);
    uint64       getVarAsU64(size_t flag, uint64 fallback = 0);
    QString      getVarAsStr(size_t flag, QString fallback = QString());
};

#endif // SERVER_H
