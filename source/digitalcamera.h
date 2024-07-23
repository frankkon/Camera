#ifndef DIGITALCAMERA_H
#define DIGITALCAMERA_H

#include <gphoto2/gphoto2.h>

#include "camera.h"

class KDigitalCamera : public KCamera {
public:
    explicit KDigitalCamera(QObject* parent = nullptr);
    virtual ~KDigitalCamera();

public:
    virtual int init();
    virtual int takeAPicture();
    virtual int setConfig();

private:
    Camera* m_pCamera;
    GPContext* m_pContext;
};
#endif // DIGITALCAMERA_H
