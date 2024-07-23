#include <QCoreApplication>
#include <QSharedMemory>

#include "log.h"
#include "service.h"
#include "system.h"

#ifdef LINUX_DEAMON
#include "global.h"
#endif

int main(int argc, char* argv[])
{
#ifdef LINUX_DEAMON
    // 设置为守护进程
    KGlobal::initDaemon();
#endif

    // 注册自定义日志输出函数
    qInstallMessageHandler(KLog::outputLog);

    // 限制只能启动一个进程
    QSharedMemory sharedMemory;
    sharedMemory.setKey("camera-kjs");

    // 第一次防止在Linux上程序异常退出时，共享内存未被释放
    if (!sharedMemory.create(1)) {
        sharedMemory.attach();
        sharedMemory.detach();

        // 第二次正式检查释放已经存在同一进程
        if (!sharedMemory.create(1)) {
            qWarning("Another camera application has running.");
            return 0;
        }
    }

    QCoreApplication a(argc, argv);

    // 需要先完成系统初始化，再进行服务初始化，服务端口信息依赖系统配置
    qInfo("Camera application start.");
    KSystem* theSystem = KSystem::instance();
    theSystem->init();

    // 相机系统Http服务
    KService* theService = KService::instance();
    theService->init();
    theService->start();

    int iRetCode = a.exec();

    // 清理工作
    KSystem::destroy();
    KService::destroy();

    qInfo("Camera application exit with code=%d.", iRetCode);
    return iRetCode;
}
