# TeamSpeak 3 Client Notification Plugin

[![Build Status](https://travis-ci.org/svenpaulsen/ts3client-notification-plugin.svg?branch=master)](https://travis-ci.org/svenpaulsen/ts3client-notification-plugin)

This simple plugin sends desktop notifications when friends appear/disappear, when your nickname is mentioned 
in the chat and when you're receiving private messages.

**Note**: Notifications are sent using the tray icon so make sure you have it enabled in the TeamSpeak 
3 Client settings.

## Testing Functionality

To make sure the plugin is running and operational, simply enter the following command in your TeamSpeak 3 
Client chat area:

`/notify Hello World!`

## Requirements

- [TeamSpeak 3 Client Plugin SDK](https://github.com/svenpaulsen/ts3client-pluginsdk)
- [Qt 5.x.x](https://www.qt.io) (release depending on your TeamSpeak 3 Client version)

## Building from Source

After cloning this repository including submodules, issue the following commands in the root directory:

    qmake
    (n)make
