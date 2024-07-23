#include <QtDebug>
#include <sqlite3.h>

#include "constdef.h"
#include "srvconfig.h"

// ================================================
// ===============类公有函数实现=====================
KSrvConfig::KSrvConfig(QObject* parent)
    : KConfig { parent }
{
    m_table = "t_service_config";
}

KSrvConfig::~KSrvConfig() { }

int KSrvConfig::getPort() const
{
    return m_config->value("port").toInt();
}

void KSrvConfig::setPort(int iPort)
{
    m_config->insert("port", QString::number(iPort));
}
