/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#ifndef PLUGIN_H
#define PLUGIN_H

#include "shared.h"

#ifdef __cplusplus
extern "C" {
#endif

// required functions
Q_DECL_EXPORT const char* ts3plugin_name();
Q_DECL_EXPORT const char* ts3plugin_version();
Q_DECL_EXPORT const char* ts3plugin_author();
Q_DECL_EXPORT const char* ts3plugin_description();
Q_DECL_EXPORT int         ts3plugin_apiVersion();
Q_DECL_EXPORT int         ts3plugin_init();
Q_DECL_EXPORT void        ts3plugin_shutdown();
Q_DECL_EXPORT void        ts3plugin_setFunctionPointers(const struct TS3Functions functions);

// optional functions
Q_DECL_EXPORT const char* ts3plugin_commandKeyword();
Q_DECL_EXPORT int         ts3plugin_processCommand(uint64 schID, const char* cmd);
Q_DECL_EXPORT void        onConnectStatusChangeEvent(uint64 schID, int status, unsigned int error);
Q_DECL_EXPORT int         ts3plugin_onTextMessageEvent(uint64 schID, anyID mode, anyID rcvID, anyID srcID, const char* srcName, const char* srcUID, const char* msg, int ignored);
Q_DECL_EXPORT void        ts3plugin_onClientMoveEvent(uint64 schID, anyID clientID, uint64 frChanID, uint64 toChanID, int visibility, const char* msg);
Q_DECL_EXPORT void        ts3plugin_onClientMoveSubscriptionEvent(uint64 schID, anyID clientID, uint64 frChanID, uint64 toChanID, int visibility);
Q_DECL_EXPORT void        ts3plugin_registerPluginID(const char* id);

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_H
