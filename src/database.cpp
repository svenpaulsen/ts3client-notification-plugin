/**
 * TeamSpeak 3 Client Notification Plugin
 *
 * Copyright (c) Sven Paulsen. All rights reserved.
 */

#include "database.h"
#include "uihelper.h"

/**
 * The Database object constructor.
 */
Database::Database() : Singleton<Database>()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", QString(pluginID));

    m_db.setDatabaseName(UIHelper::getConfigPath() + "settings.db");
}

/**
 * The Database object destructor.
 */
Database::~Database()
{
    if(isOpen())
    {
        close();
    }

    QSqlDatabase::removeDatabase(QString(pluginID));
}

/**
 * Returns TRUE if the underlying database connection is valid.
 */
bool Database::isValid()
{
    return m_db.isValid();
}

/**
 * Returns TRUE if the underlying database connection is open.
 */
bool Database::isOpen()
{
    return m_db.isOpen();
}

/**
 * Opens the database connection.
 */
bool Database::open()
{
    return m_db.open();
}

/**
 * Closes the database connection.
 */
void Database::close()
{
    m_db.close();
}

/**
 * Returns the TeamSpeak 3 Client database scheme version.
 */
int Database::getVersion()
{
    QString version;

    if(!getValue("General", "DatabaseVersion", version, "0"))
    {
        return -1;
    }

    return version.toInt();
}

int Database::hasContact(QString uid)
{
    QStringList contacts;

    if(!getValues("Contacts", contacts))
    {
        return -1;
    }

    for(int i = 0; i < contacts.size(); ++i)
    {
        if(contacts.at(i).contains("IDS=" + uid))
        {
            QMap<QString, QString> contact = parseValue(contacts.at(i));

            return contact.contains("Friend") ? contact["Friend"].toInt() : -1;
        }
    }

    return -1;
}

/**
 * Queries a list of values from the TeamSpeak 3 Client database.
 */
bool Database::getValues(QString table, QStringList &result)
{
    if(!isOpen() && !open())
    {
        return false;
    }

    QMap<QString, QString> map;

    QSqlQuery query = prepare("SELECT value FROM " + table, map);

    if(!query.exec())
    {
        return false;
    }

    while(query.next())
    {
        result.append(query.value(0).toString());
    }

    return true;
}

/**
 * Queries a single value from the TeamSpeak 3 Client database.
 */
bool Database::getValue(QString table, QString key, QString &result, QString fallback)
{
    result = fallback;

    if(!isOpen() && !open())
    {
        return false;
    }

    QMap<QString, QString> map;

    map["key"] = key;

    QSqlQuery query = prepare("SELECT value FROM " + table + " WHERE key = :key", map);

    if(!query.exec())
    {
        return false;
    }

    while(query.next())
    {
        result = query.value(0).toString();
    }

    return true;
}

/**
 * Returns a prepared SQL statement for later use.
 */
QSqlQuery Database::prepare(QString sql, QMap<QString, QString> map)
{
    QSqlQuery query(m_db);
    QMapIterator<QString, QString> it(map);

    query.prepare(sql);

    while(it.hasNext())
    {
        it.next();
        query.bindValue(":" + it.key(), it.value());
    }

    return query;
}

/**
 * Converts a given string into a map.
 */
QMap<QString, QString> Database::parseValue(QString str)
{
    QMap<QString, QString> map;
    QStringList lst = str.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

     for(int i = 0; i < lst.count(); ++i)
     {
         map.insert(lst.at(i).section("=", 0, 0), lst.at(i).section("=", 1));
     }

     return map;
}
