#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QString>
#include <QTextStream>

#include "constdef.h"
#include "log.h"

KLog::KLog(QObject* parent)
    : QObject { parent }
{
}

void KLog::outputLog(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString text;
    switch (type) {
    case QtDebugMsg:
        text = QString("[DEBUG]   ");
        break;
    case QtInfoMsg:
        text = QString("[INFO]    ");
        break;
    case QtWarningMsg:
        text = QString("[WARN]    ");
        break;
    case QtCriticalMsg:
        text = QString("[CRITICAL]");
        break;
    case QtFatalMsg:
        text = QString("[FATAL]   ");
    }

    // 将 当前时间 日志级别 源文件名 函数名 行号 日志内容，拼装成一行日志
    // 当前时间
    QDateTime now = QDateTime::currentDateTime();
    QString sCurrentTime = now.toString("[hh:mm:ss:zzz]");

    // 源文件名：函数名：行号
    QString sSrcFile(": ");
#ifndef QT_NO_DEBUG
    sSrcFile = QString(context.file ? context.file : "");
    QString sFunName = QString(context.function ? context.function : "");
    sSrcFile = QString("[%1:%2:%3]").arg(sSrcFile).arg(context.line).arg(sFunName);
#else
    Q_UNUSED(context)
#endif
    // 完整一行日志
    QString sLogMsg = QString("%1 %2 %3 %4\n").arg(sCurrentTime, text, sSrcFile, msg);

    // 写日志文件
    QString sLogFile = QString("%1%2.log").arg(s_DIR_LOG, now.toString("yyyy-MM-dd"));
    static QMutex mutex;
    mutex.lock();
    QFile file(sLogFile);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream stream(&file);
    stream << sLogMsg;
    file.flush();
    file.close();
    mutex.unlock();
}
