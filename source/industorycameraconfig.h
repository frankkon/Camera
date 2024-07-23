#ifndef KINDUSTORYCAMERACONFIG_H
#define KINDUSTORYCAMERACONFIG_H
#include "config.h"

class KIndustoryCameraConfig : public KConfig {
    Q_OBJECT
public:
    explicit KIndustoryCameraConfig(QObject* parent = nullptr);
    virtual ~KIndustoryCameraConfig();
};

#endif // KINDUSTORYCAMERACONFIG_H
