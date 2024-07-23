#include <QDebug>
#include <QFile>
#include <QHttpServer>
#include <QJsonObject>
#include <QMap>
#include <QtDebug>

#include "constdef.h"
#include "service.h"
#include "srvconfig.h"
#include "system.h"

//================================================
//===============类静态函数实现=====================
// 相机系统对外服务
KService* KService::theService = nullptr;
KService* KService::instance()
{
    qDebug("create service instance started");
    if (nullptr == theService) {
        theService = new KService();
    }

    qDebug("create service instance finished");
    return theService;
}

void KService::destroy()
{
    qDebug("destroy service instance started");
    SAFE_DELETE(theService);
    qDebug("destroy service instance finished");
}

//================================================
//===============类公有函数实现=====================
KService::KService(QObject* parent)
    : QObject { parent }
    , m_httpService(nullptr)
    , m_pConfig(nullptr)
{
}

int KService::init()
{
    qDebug("Service initialization started");

    if (nullptr == m_pConfig) {
        m_pConfig = new KSrvConfig(this);
    }
    m_pConfig->loadConfig();

    if (nullptr == m_httpService) {
        m_httpService = new QHttpServer(this);
    }

    // 默认，welcome API
    m_httpService->route("/", []() {
        return "Welcome to use CameraSystem.";
    });

    // 获取系统配置信息
    getSystemConfig();

    // 获取工业相机当前照片一张
    getAIndustryPicture();

    // 获取数码相机当前照片一张
    getADigitalPicture();

    qDebug("Service initialization finished ");
    return R_OP_OK;
}

int KService::start()
{
    qDebug("begin service starting");

    int iCfgPort = m_pConfig->getPort();
    const auto port = m_httpService->listen(QHostAddress::Any, iCfgPort);
    if (!port) {
        qCritical("Server failed to listen on port：%d.", iCfgPort);
        return R_SERVICE_START_FAILURE;
    }

    qDebug("Service is running, listen port:%d ", port);
    return R_OP_OK;
}

int KService::stop()
{
    qDebug("Service halt started");

    KService::destroy();

    qDebug("Service halt finished");
    return R_OP_OK;
}

//================================================
//===============类私有函数实现=====================
void KService::getSystemConfig()
{
    qDebug("Service getSystemConfig started");
    //    m_httpService->route("/GetSystemConfig", []() {
    //        QMap<QString, QString>* mapConfig = KSystem::instance()->getConfig()->getConfig();
    //        QJsonObject json;
    //        QMap<QString, QString>::ConstIterator i = mapConfig->constBegin();
    //        while (i != mapConfig->constEnd()) {
    //            json.insert(i.key(), i.value());
    //            ++i;
    //        }
    //        return QHttpServerResponse(json);
    //    });
    qDebug("Service getSystemConfig finished");
}

void KService::getAIndustryPicture()
{
    qDebug("Service getAIndustryPicture API define started");
    m_httpService->route("/IndustryCameraGetAPicture", [](QHttpServerResponder&& responder) {
        // 获取图片并保持到本地目录
        KSystem* theSystem = KSystem::instance();
        theSystem->getAIndustryPicture();

        // 从本地目录读取图片文件
        QString sFilePath(S_DIR_INDUSTRY_IMAGE);
        sFilePath.append("image.jpg");
        QFile file(sFilePath);
        if (!file.open(QIODevice::ReadOnly) || file.size() == 0) {
            file.close();
            qCritical("open picture failed");
            return;
        }

        // 写响应头，数据长度，和方法体数据长度一致
        responder.writeStatusLine(QHttpServerResponder::StatusCode::Ok);
        responder.writeHeader("Accept-Ranges", "bytes");
        responder.writeHeader("Content-Type", "image/jpg");
        responder.writeHeader("Content-Length", QString::number(file.size()).toUtf8());
        responder.writeBody(file.readAll(), file.size());
    });
    qDebug("Service getAIndustryPicture API define finished");
}

void KService::getADigitalPicture()
{
    qDebug("Service getADigitalPicture API define started");
    m_httpService->route("/DigitalCameraGetAPicture", [](QHttpServerResponder&& responder) {
        // 获取图片并保持到本地目录
        KSystem* theSystem = KSystem::instance();
        theSystem->getADigitalPicture();

        // 从本地目录读取图片文件
        QString sFilePath(S_DIR_DIGITAL_IMAGE);
        sFilePath.append("image.jpg");
        QFile file(sFilePath);
        if (!file.open(QIODevice::ReadOnly) || file.size() == 0) {
            file.close();
            qCritical("open picture failed");
            return;
        }

        // 写响应头，数据长度，和方法体数据长度一致
        responder.writeStatusLine(QHttpServerResponder::StatusCode::Ok);
        responder.writeHeader("Accept-Ranges", "bytes");
        responder.writeHeader("Content-Type", "image/jpg");
        responder.writeHeader("Content-Length", QString::number(file.size()).toUtf8());
        responder.writeBody(file.readAll(), file.size());
    });
    qDebug("Service getADigitalPicture API define finished");
}
