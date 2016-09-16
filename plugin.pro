#-------------------------------------------------
#
# TeamSpeak 3 Client Notification Plugin
#
#-------------------------------------------------

QT     += sql widgets
CONFIG += c++11

VERSION  = 1.0.2
TARGET   = notification_plugin
TEMPLATE = lib

INCLUDEPATH += src
INCLUDEPATH += includes/pluginsdk/include

DEFINES += PLUGIN_VER=\\\"$${VERSION}\\\"

SOURCES += src/plugin.cpp \
    src/uihelper.cpp \
    src/server.cpp \
    src/shared.cpp \
    src/client.cpp \
    src/channel.cpp \
    src/database.cpp
HEADERS += src/plugin.h \
    src/uihelper.h \
    src/server.h \
    src/shared.h \
    src/client.h \
    src/channel.h \
    src/database.h \
    src/singleton.h
