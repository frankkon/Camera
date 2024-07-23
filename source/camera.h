#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

class KCamera : public QObject {
    Q_OBJECT

public:
    explicit KCamera(QObject* parent = nullptr);
    virtual ~KCamera() = 0;

public:
    virtual int init() = 0;
    virtual int takeAPicture() = 0;
    virtual int setConfig() = 0;

signals:
};

#endif // CAMERA_H
