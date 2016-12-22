#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QDir>
#include <QFile>
#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QApplication>

#include <QDebug>

class AppConfig
{
public:
    explicit AppConfig();
    ~AppConfig();

    inline QString getStrAppPath() const
    {
        return strAppPath;
    }

    inline QString getStrConfPath() const
    {
        return strConfPath;
    }

    inline QString getStrIniFile() const
    {
        return strIniFile;
    }

    inline QString getStrCodec() const
    {
        return strCodec;
    }

    inline QString getStrLangeage() const
    {
        return strLangeage;
    }

    // 初始化
    static void initApplication(QApplication &app);
    // 创建配置文件
    static void creatorSettingFile();
    // 读取/修改配置文件
    static void setSettingFile(const QString &group, const QString &key, const QVariant &value);
    static QVariant getSettingKeyValue(const QString &group, const QString &key, const QVariant &value);
    // 检查目录
    static void checkDirs();

public:
    // 默认目录
    static QString strAppPath;
    static QString strConfPath;
    static QString strIniFile;
    static QString strCodec;
    static QString strLangeage;
};

#endif // APPCONFIG_H
