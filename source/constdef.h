#ifndef CONSTDEF_H
#define CONSTDEF_H

// 返回码定义
#define R_OP_OK 0
#define R_SERVICE_START_FAILURE -1
#define R_FILE_OPEN_FAILURE -2
#define R_CAMERA_OPEN_FAILURE -3

// 相机操作错误码定义
#define R_NO_CAMERA -1

// 资源存放目录定义
#define S_DIR_INDUSTRY_IMAGE ("../pictures/industry/")
#define S_DIR_DIGITAL_IMAGE ("../pictures/digital/")
#define s_DIR_LOG ("../log/")
#define S_CONFIG_DB ("config.db")

// 长度定义
#define S_SQL_LENGTH 256

// 安全堆内存释放
#define SAFE_DELETE(x)        \
    do {                      \
        if ((x) != nullptr) { \
            delete (x);       \
            (x) = nullptr;    \
        }                     \
    } while (0);

#endif // CONSTDEF_H
