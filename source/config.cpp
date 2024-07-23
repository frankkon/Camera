#include <QtDebug>
#include <sqlite3.h>
#include <stdio.h>

#include "config.h"
#include "constdef.h"

// ================================================
// ===============类静态函数实现=====================
// callback fun for query
int KConfig::dealResult(void* vpData, int iColNum, char** cppResult,
    char** cppColName)
{
    qDebug("deal query result starting.");
    Q_UNUSED(vpData)
    Q_UNUSED(cppColName)
    if (iColNum > 0) {
        QString key(cppResult[0] ? cppResult[0] : "");
        QString value(cppResult[1] ? cppResult[1] : "");

        qInfo("loaded key:value = %s:%s", key.toStdString().c_str(),
            value.toStdString().c_str());
        static_cast<KConfig*>(vpData)->getConfig()->insert(key, value);
    }
    qDebug("deal query result finished.");
    return R_OP_OK;
}

// ================================================
// ===============类公有函数实现=====================
KConfig::KConfig(QObject* parent)
    : QObject { parent }
    , m_config(nullptr)
    , m_table("t_service_config")
{
}

KConfig::~KConfig()
{
    SAFE_DELETE(m_config)
}

void KConfig::loadConfig()
{
    qDebug("load config starting.");

    if (nullptr == m_config) {
        m_config = new QMap<QString, QString>();
    }

    sqlite3* db;

    if (SQLITE_OK != sqlite3_open(S_CONFIG_DB, &db)) {
        qCritical("open config db failure. %s.", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char sql[S_SQL_LENGTH + 1] = { 0 };
    snprintf(sql, S_SQL_LENGTH, "select f_key, f_value from %s where 1=1;", m_table.toStdString().c_str());

    char* errMsg(nullptr);
    if (SQLITE_OK != sqlite3_exec(db, sql, KConfig::dealResult, this, &errMsg)) {
        qCritical("query config db failure. %s.", errMsg);
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return;
    }
    sqlite3_close(db);
    qDebug("load config finished.");
}

QMap<QString, QString>* KConfig::getConfig() const
{
    return m_config;
}
