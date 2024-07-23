#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>

class QHttpServer;
class KSrvConfig;

class KService : public QObject {
    Q_OBJECT
public:
    explicit KService(QObject* parent = nullptr);

    static KService* instance();
    static void destroy();

public:
    int init();
    int start();
    int stop();

signals:

private:
    void getSystemConfig();
    void getAIndustryPicture();
    void getADigitalPicture();

private:
    QHttpServer* m_httpService;

    KSrvConfig* m_pConfig;

    static KService* theService;
};

#endif // SERVICE_H
