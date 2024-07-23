#include "industorycameraconfig.h"

KIndustoryCameraConfig::KIndustoryCameraConfig(QObject* parent)
    : KConfig { parent }
{
    m_table = "t_industory_camera_config";
}

KIndustoryCameraConfig::~KIndustoryCameraConfig() { }
