/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#ifndef DATABASE_H
#define DATABASE_H

#include "singleton.h"
#include "shared.h"

class Database : public Singleton<Database>
{
    friend class Singleton<Database>;

    QSqlDatabase m_db;

    Database();
    ~Database();

public:
    bool      isValid();
    bool      isOpen();
    bool      open();
    void      close();
    int       getVersion();
    int       hasContact(QString uid);
    bool      getValues(QString table, QStringList &result);
    bool      getValue(QString table, QString key, QString &result, QString fallback = QString());
    QSqlQuery prepare(QString sql, QMap<QString, QString> map);

private:
    QMap<QString, QString> parseValue(QString str);
};

#endif // DATABASE_H
