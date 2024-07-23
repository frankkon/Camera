#ifndef GLOBAL_H
#define GLOBAL_H

class KGlobal {
public:
    KGlobal();

#ifdef LINUX_DEAMON
public:
    static void restartService();
    static void signalHandler(int sig);
    static void initDaemon();
#endif
};

#endif // GLOBAL_H
