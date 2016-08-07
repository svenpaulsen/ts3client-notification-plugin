#ifndef PLUGIN_H
#define PLUGIN_H

#include <QtCore>

#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"
#include "plugin.h"

extern struct TS3Functions pluginSDK;
extern char*               pluginID;

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
Q_DECL_EXPORT int         ts3plugin_processCommand(uint64 schID, const char* command);
Q_DECL_EXPORT int         ts3plugin_onTextMessageEvent(uint64 schID, anyID mode, anyID toID, anyID fromID, const char* fromName, const char* fromUID, const char* message, int ignored);
Q_DECL_EXPORT void        ts3plugin_registerPluginID(const char* id);

#ifdef __cplusplus
}
#endif

#endif // PLUGIN_H
