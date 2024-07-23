#ifndef LOG_H
#define LOG_H

#include <QObject>

class KLog : public QObject {
    Q_OBJECT
public:
    explicit KLog(QObject* parent = nullptr);

public:
    static void outputLog(QtMsgType type, const QMessageLogContext& context, const QString& msg);
signals:
};

#endif // LOG_H
