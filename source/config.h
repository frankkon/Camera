#ifndef KCONFIG_H
#define KCONFIG_H

#include <QMap>
#include <QObject>

class KConfig : public QObject {
    Q_OBJECT
public:
    explicit KConfig(QObject* parent = nullptr);
    virtual ~KConfig();

signals:

public:
    // 查询结果回调函数，处理配置信息查询结果
    static int dealResult(void* vpData, int iColNum, char** cppResult, char** cppColName);

public:
    void loadConfig();
    QMap<QString, QString>* getConfig() const;

protected:
    // 配置列表
    QMap<QString, QString>* m_config;

    // db table name
    QString m_table;
};

#endif // KCONFIG_H
