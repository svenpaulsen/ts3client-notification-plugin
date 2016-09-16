/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#ifndef CHANNEL_H
#define CHANNEL_H

#include "shared.h"

class Channel
{
    uint64 m_schID;
    uint64 m_channelID;

public:
    Channel(uint64 schID, uint64 channelID = 0);

    uint64  getID();
    uint64  getSchID();
    QString getName();
    QString requestVars();
    int     getVarAsInt(size_t flag, int fallback = 0);
    uint64  getVarAsU64(size_t flag, uint64 fallback = 0);
    QString getVarAsStr(size_t flag, QString fallback = QString());
};

#endif // CHANNEL_H
