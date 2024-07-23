#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtDebug>

#include "camera.h"
#include "constdef.h"
#include "digitalcamera.h"
#include "industrycamera.h"
#include "system.h"

//================================================
//===============类静态函数实现=====================
// 相机系统
KSystem* KSystem::theSystem = nullptr;

// 系统唯一实例
KSystem* KSystem::instance()
{
    qDebug("create system instance started");
    if (nullptr == theSystem) {
        theSystem = new KSystem();
    }

    qDebug("create system instance finished");
    return theSystem;
}

void KSystem::destroy()
{
    qDebug("destroy system instance started");
    SAFE_DELETE(theSystem);
    qDebug("destroy system instance finished");
}

//================================================
//===============类公有函数实现=====================
KSystem::KSystem(QObject* parent)
    : QObject { parent }
    , m_pIndustryCamera(nullptr)
    , m_pDigitalCamera(nullptr)
{
}

int KSystem::init()
{
    qDebug("System initialization started");

    // 初始化工业相机
    if (nullptr == m_pIndustryCamera) {
        m_pIndustryCamera = new KIndustryCamera(this);
        m_pIndustryCamera->init();
    }

    // 初始化数字相机
    if (nullptr == m_pDigitalCamera) {
        m_pDigitalCamera = new KDigitalCamera(this);
        m_pDigitalCamera->init();
    }

    qDebug("System initialization finished");
    return R_OP_OK;
}

int KSystem::stop()
{
    KSystem::destroy();
    return R_OP_OK;
}

void KSystem::getAIndustryPicture()
{
    qDebug("System getAIndustryPicture started");
    m_pIndustryCamera->takeAPicture();
    qDebug("System getAIndustryPicture finished");
}

void KSystem::getADigitalPicture()
{
    qDebug("System getADigitalPicture started");
    m_pDigitalCamera->takeAPicture();
    qDebug("System getADigitalPicture finished");
}

//================================================
//===============类私有函数实现=====================
