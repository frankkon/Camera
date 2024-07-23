#include <QCoreApplication>
#include <QtDebug>

#include "global.h"

#ifdef LINUX_DEAMON
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "service.h"
#include "system.h"

void KGlobal::restartService()
{
    qInfo("stopping service.");
    KSystem::destroy();
    KService::destroy();

    // 初始化系统
    qInfo("starting systerm.");
    KSystem* theSystem = KSystem::instance();
    theSystem->init();

    // 相机系统Http服务
    qInfo("starting service.");
    KService* theService = KService::instance();
    theService->init();
    theService->start();
}

void KGlobal::signalHandler(int sig)
{
    switch (sig) {
    case SIGHUP:
        qInfo("hangup signal catched,restart service.");
        KGlobal::restartService();
        break;
    case SIGTERM:
        qInfo("terminate signal catched,exit application.");
        QCoreApplication::instance()->exit(0);
        break;
    }
}

void KGlobal::initDaemon()
{
    int pid = fork();
    if (pid < 0) {
        exit(1); // fork error
    } else if (pid > 0) {
        exit(0); // parent exits
    }

    // child (daemon) continues
    setsid(); // obtain a new process group

    // close all descriptors
    for (int i = getdtablesize(); i >= 0; --i) {
        close(i);
    }

    // 设置标准输入stdin/标准输出stdout/标准错误stderr到/dev/null
    int fd = open("/dev/null", O_RDWR);
    fd = dup(fd);
    fd = dup(fd); // handle standart I/O

    umask(027); // set newly created file permissions

    signal(SIGCHLD, SIG_IGN); // ignore child
    signal(SIGTSTP, SIG_IGN); // ignore tty signals
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGHUP, KGlobal::signalHandler); // catch hangup signal
    signal(SIGTERM, KGlobal::signalHandler); // catch kill signal
}

#endif

KGlobal::KGlobal()
{
}
