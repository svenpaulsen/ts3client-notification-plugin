/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "shared.h"

class Client
{
    uint64 m_schID;
    anyID  m_clientID;

public:
    Client(uint64 schID, anyID clientID = 0);

    anyID   getID();
    uint64  getSchID();
    QString getName();
    QString requestVars();
    int     getVarAsInt(size_t flag, int fallback = 0);
    uint64  getVarAsU64(size_t flag, uint64 fallback = 0);
    QString getVarAsStr(size_t flag, QString fallback = QString());
    bool    isFriend();
};

#endif // CLIENT_H
