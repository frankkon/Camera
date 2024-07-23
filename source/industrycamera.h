#ifndef INDUSTRYCAMERA_H
#define INDUSTRYCAMERA_H

#include "camera.h"

class KIndustoryCameraConfig;

class KIndustryCamera : public KCamera {
public:
    explicit KIndustryCamera(QObject* parent = nullptr);
    virtual ~KIndustryCamera();

public:
    virtual int init();
    virtual int takeAPicture();
    virtual int setConfig();

private:
    int isDeviceReady();
    void closeDevice();

private:
    void* m_handle;
    KIndustoryCameraConfig* m_pConfig;
};

#endif // INDUSTRYCAMERA_H
