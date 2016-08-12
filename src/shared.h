/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#ifndef SHARED_H
#define SHARED_H

#include <QtCore>
#include <QApplication>
#include <QMainWindow>
#include <QSystemTrayIcon>

#include "teamspeak/public_rare_definitions.h"
#include "ts3_functions.h"

#define PLUGIN_API 20
#define BUFFSIZE_L 512
#define BUFFSIZE_M 256
#define BUFFSIZE_D 128
#define BUFFSIZE_S 64

extern struct TS3Functions pluginSDK;
extern char*               pluginID;

#endif // SHARED_H
