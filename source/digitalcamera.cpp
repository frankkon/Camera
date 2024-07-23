#include <QFile>
#include <QtDebug>
#include <gphoto2/gphoto2.h>

#include "digitalcamera.h"

#include "constdef.h"

KDigitalCamera::KDigitalCamera(QObject* parent)
    : KCamera { parent }
    , m_pCamera(nullptr)
    , m_pContext(nullptr)
{
}

KDigitalCamera::~KDigitalCamera()
{
    gp_camera_exit(m_pCamera, m_pContext);
}

int KDigitalCamera::init()
{
    qDebug("digital camera initialization started");

    m_pContext = gp_context_new();
    gp_camera_new(&m_pCamera);
    int iRetCode = gp_camera_init(m_pCamera, m_pContext);
    if (iRetCode != GP_OK) {
        qCritical("camera init failure: %d", iRetCode);
        return iRetCode;
    }

    qDebug("digital camera initialization finished");
    return R_OP_OK;
}

int KDigitalCamera::takeAPicture()
{
    qDebug("digital camera take a picture started");
    int retval;
    CameraFile* file;
    CameraFilePath camera_file_path;

    qDebug("Capturing.");

    /* NOP: This gets overridden in the library to /capt0000.jpg */
    strcpy(camera_file_path.folder, "/");
    strcpy(camera_file_path.name, "foo.jpg");

    retval = gp_camera_capture(m_pCamera, GP_CAPTURE_IMAGE, &camera_file_path, m_pContext);
    if (GP_OK != retval) {
        qCritical("gp_camera_capture Retval: %d", retval);
        return retval;
    }

    retval = gp_file_new(&file);
    if (GP_OK != retval) {
        qCritical("gp_file_new Retval: %d", retval);
        return retval;
    }

    retval = gp_camera_file_get(m_pCamera, camera_file_path.folder, camera_file_path.name,
        GP_FILE_TYPE_NORMAL, file, m_pContext);
    if (GP_OK != retval) {
        qCritical("gp_camera_file_get Retval: %d", retval);
        return retval;
    }

    char* data;
    unsigned long size;
    gp_file_get_data_and_size(file, (const char**)&data, &size);

    qDebug("Deleting.");
    retval = gp_camera_file_delete(m_pCamera, camera_file_path.folder, camera_file_path.name, m_pContext);
    if (GP_OK != retval) {
        qWarning("gp_camera_file_delete Retval: %d", retval);
    }

    // 将图片数据保存成文件
    qDebug("start save picture");
    QString sFilePath(S_DIR_DIGITAL_IMAGE);
    sFilePath.append("image.jpg");
    QFile image(sFilePath);
    if (!image.open(QIODevice::WriteOnly)) {
        qCritical("open image file failure");
        return R_FILE_OPEN_FAILURE;
    }
    image.write(reinterpret_cast<const char*>(data), size);
    image.close();
    qDebug("save picture successfully");

    qDebug("digital camera take a picture finished");
    return R_OP_OK;
}

int KDigitalCamera::setConfig()
{
    return R_OP_OK;
}
