#ifndef SRVCONFIG_H
#define SRVCONFIG_H
#include <QMap>
#include <QObject>

#include "config.h"

class KSrvConfig : public KConfig {
    Q_OBJECT

public:
    explicit KSrvConfig(QObject* parent = nullptr);
    virtual ~KSrvConfig();

public:
    int getPort() const;
    void setPort(int iPort);
};

#endif // SRVCONFIG_H
