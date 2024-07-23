#ifdef WIN32
#undef WIN32
#endif

#include <MvCameraControl.h>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include <unistd.h>

#include "constdef.h"
#include "industorycameraconfig.h"
#include "industrycamera.h"

//================================================
//===============类公有函数实现=====================
KIndustryCamera::KIndustryCamera(QObject* parent)
    : KCamera { parent }
    , m_handle(nullptr)
    , m_pConfig(nullptr)
{
}

KIndustryCamera::~KIndustryCamera()
{
    // 销毁句柄，释放资源
    if (nullptr != m_handle) {
        int nRet = MV_CC_DestroyHandle(m_handle);
        if (MV_OK != nRet) {
            qCritical("DestroyHandle fail [%x]", nRet);
        }
    }
}

int KIndustryCamera::init()
{
    qDebug("industry camera initialization started");

    // 枚举子网内指定的传输协议对应的所有设备
    unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;
    MV_CC_DEVICE_INFO_LIST stDevList;
    memset(&stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int nRet = MV_CC_EnumDevices(nTLayerType, &stDevList);
    if (MV_OK != nRet) {
        qCritical("EnumDevices fail [%x]", nRet);
        return nRet;
    }

    if (stDevList.nDeviceNum == 0) {
        qWarning("no camera found!");
        return R_NO_CAMERA;
    }

    // 选择查找到的第一台在线设备，创建设备句柄
    MV_CC_DEVICE_INFO stDevInfo;
    memset(&stDevInfo, 0, sizeof(MV_CC_DEVICE_INFO));
    memcpy(&stDevInfo, stDevList.pDeviceInfo[0], sizeof(MV_CC_DEVICE_INFO));

    nRet = MV_CC_CreateHandle(&m_handle, &stDevInfo);

    if (MV_OK != nRet) {
        qCritical("CreateHandle fail [%x]", nRet);
        return nRet;
    }

    // 根据预设值初始化相机
    if (nullptr == m_pConfig) {
        m_pConfig = new KIndustoryCameraConfig();
    }
    m_pConfig->loadConfig();
    setConfig();

    qDebug("industry camera initialization finished");
    return nRet;
}

int KIndustryCamera::takeAPicture()
{
    qDebug("industry camera take a picture started");

    // 连接设备
    int nRet = isDeviceReady();
    if (MV_OK != nRet) {
        return nRet;
    }

    // 获取一帧数据的大小
    MVCC_INTVALUE stIntvalue;
    memset(&stIntvalue, 0, sizeof(MVCC_INTVALUE));
    nRet = MV_CC_GetIntValue(m_handle, "PayloadSize", &stIntvalue);
    if (nRet != MV_OK) {
        qCritical("Get PayloadSize failed! nRet [%x]", nRet);
        closeDevice();
        return nRet;
    }

    int nBufSize = stIntvalue.nCurValue; // 一帧数据大小
    unsigned int nTestFrameSize = 0;
    unsigned char* pFrameBuf = nullptr;
    pFrameBuf = (unsigned char*)malloc(nBufSize);

    MV_FRAME_OUT_INFO_EX stFrameInfo;
    memset(&stFrameInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

    qDebug("start get picture");

    while (1) {
        if (nTestFrameSize > 0) {
            break;
        }

        nRet = MV_CC_GetOneFrameTimeout(m_handle, pFrameBuf, nBufSize, &stFrameInfo, 1000);
        if (MV_OK != nRet) {
            qCritical("get one frame failure errcode=%d ", nRet);
            usleep(1);
        } else {
            // 图片数据输入输出参数
            MV_SAVE_IMAGE_PARAM_EX stParam;

            // 源数据
            stParam.pData = pFrameBuf; // 原始图像数据
            stParam.nDataLen = stFrameInfo.nFrameLen; // 原始图像数据长度
            stParam.enPixelType = stFrameInfo.enPixelType; // 原始图像数据的像素格式
            stParam.nWidth = stFrameInfo.nWidth; // 图像宽
            stParam.nHeight = stFrameInfo.nHeight; // 图像高
            stParam.nJpgQuality = 70; // JPEG图片编码质量

            // 目标数据
            stParam.enImageType = MV_Image_Jpeg; // 需要保存的图像类型，转换成JPEG格式
            stParam.nBufferSize = nBufSize; // 存储节点的大小
            unsigned char* pImage = (unsigned char*)malloc(nBufSize);
            stParam.pImageBuffer = pImage; // 输出数据缓冲区，存放转换之后的图片数据

            nRet = MV_CC_SaveImageEx2(m_handle, &stParam);
            if (MV_OK != nRet) {
                break;
            }

            // 将转换之后图片数据保存成文件
            qDebug("start save picture");
            QString sFilePath(S_DIR_INDUSTRY_IMAGE);
            sFilePath.append("image.jpg");
            QFile file(sFilePath);
            if (!file.open(QIODevice::WriteOnly)) {
                qCritical("open image file failure");
                closeDevice();
                return R_FILE_OPEN_FAILURE;
            }
            file.write(reinterpret_cast<const char*>(pImage), stParam.nImageLen);
            file.close();
            free(pImage);
            qDebug("save picture successfully");

            // 图像获取成功
            nTestFrameSize++;
        }
    }
    free(pFrameBuf);

    // 停止采集图像
    closeDevice();
    qDebug("industry camera take a picture finished");
    return nRet;
}

int KIndustryCamera::setConfig()
{
    qDebug("industry camera set config starting");

    if (nullptr == m_handle) {
        qCritical("camera do not initialize,exit set config.");
        return R_CAMERA_OPEN_FAILURE;
    }

    int nRet = MV_CC_OpenDevice(m_handle);
    if (MV_OK != nRet) {
        qCritical("OpenDevice fail, return code [%x],exit set config.", nRet);
        return nRet;
    }

    QMap<QString, QString>* mapConfig = m_pConfig->getConfig();

    qDebug() << "industry camera ExposureAuto is " << mapConfig->value("ExposureAuto");
    nRet = MV_CC_SetEnumValue(m_handle, "ExposureAuto", mapConfig->value("ExposureAuto").toInt());
    if (MV_OK != nRet) {
        qCritical("config ExposureAuto fail, return code [%x].", nRet);
    }

    qDebug() << "industry camera BalanceWhiteAuto is " << mapConfig->value("BalanceWhiteAuto");
    nRet = MV_CC_SetEnumValue(m_handle, "BalanceWhiteAuto", mapConfig->value("BalanceWhiteAuto").toInt());
    if (MV_OK != nRet) {
        qCritical("config BalanceWhiteAuto fail, return code [%x].", nRet);
    }

    qDebug() << "industry camera Brightness is " << mapConfig->value("Brightness");
    nRet = MV_CC_SetIntValueEx(m_handle, "Brightness", mapConfig->value("Brightness").toInt());
    if (MV_OK != nRet) {
        qCritical("config Brightness fail, return code [%x].", nRet);
    }

    nRet = MV_CC_CloseDevice(m_handle);
    if (MV_OK != nRet) {
        qCritical("CloseDevice fail [%x]", nRet);
    }
    qDebug("industry camera set config finished");
    return nRet;
}

//================================================
//===============类私有函数实现=====================
int KIndustryCamera::isDeviceReady()
{
    // 连接设备
    int nRet = MV_CC_OpenDevice(m_handle);
    if (MV_OK != nRet) {
        qCritical("OpenDevice fail [%x]", nRet);
        return nRet;
    }

    // 开始采集图像
    nRet = MV_CC_StartGrabbing(m_handle);
    if (MV_OK != nRet) {
        qCritical("StartGrabbing fail [%x]", nRet);
        nRet = MV_CC_CloseDevice(m_handle);
        if (MV_OK != nRet) {
            qCritical("CloseDevice fail [%x]", nRet);
        }
    }
    return nRet;
}

void KIndustryCamera::closeDevice()
{
    // 停止采集图像
    int nRet = MV_CC_StopGrabbing(m_handle);
    if (MV_OK != nRet) {
        qCritical("StopGrabbing fail [%x]", nRet);
    }

    // 关闭设备，释放资源
    nRet = MV_CC_CloseDevice(m_handle);
    if (MV_OK != nRet) {
        qCritical("CloseDevice fail [%x]", nRet);
    }
}
