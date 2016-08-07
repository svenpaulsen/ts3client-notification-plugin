#-------------------------------------------------
#
# TeamSpeak 3 Client Notification Plugin
#
#-------------------------------------------------

QT     += widgets
CONFIG += c++11

TARGET   = notification_plugin
TEMPLATE = lib

INCLUDEPATH += src
INCLUDEPATH += includes/pluginsdk/include

SOURCES += src/plugin.cpp \
    src/uihelper.cpp
HEADERS += src/plugin.h \
    src/uihelper.h
