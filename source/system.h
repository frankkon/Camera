#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>

class KCamera;

class KSystem : public QObject {
    Q_OBJECT
public:
    explicit KSystem(QObject* parent = nullptr);

    static KSystem* instance();
    static void destroy();

public:
    int init();
    int stop();
    void getAIndustryPicture();
    void getADigitalPicture();

signals:

private:
    static KSystem* theSystem;

    KCamera* m_pIndustryCamera;
    KCamera* m_pDigitalCamera;
};

#endif // SYSTEM_H
