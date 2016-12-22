#include "appconfig.h"

QString AppConfig::strAppPath         = "./";
QString AppConfig::strConfPath        = "conf";
QString AppConfig::strIniFile         = "config.ini";
QString AppConfig::strLangeage        = "zh";
QString AppConfig::strCodec           = "UTF8";

AppConfig::AppConfig()
{

}

AppConfig::~AppConfig()
{

}

void AppConfig::initApplication(QApplication &app)
{
    strAppPath        = app.applicationDirPath() + "/";
    strConfPath       = strAppPath + "conf/";
    strIniFile        = strConfPath + "Config.ini";
    checkDirs();
    creatorSettingFile();
    strLangeage      = getSettingKeyValue("Setup", "Language", "zh").toString();
    strCodec         = getSettingKeyValue("Setup", "Codec", "UTF8").toInt();
}

void AppConfig::creatorSettingFile()
{
    QSettings settings(strIniFile, QSettings::IniFormat);
    QString strGroups = settings.childGroups().join("");
    if (!QFile::exists(strIniFile) || (strGroups.isEmpty()))
    {
        settings.beginGroup("Setup");
        settings.setValue("Language", "zh");
        settings.setValue("Codec",     "UTF8");
        settings.endGroup();
        settings.sync();
    }
}

void AppConfig::setSettingFile(const QString &group, const QString &key, const QVariant &value)
{
    QSettings settings(strIniFile, QSettings::IniFormat);
    settings.beginGroup(group);
    settings.setValue(key, value);
    settings.sync();
}

QVariant AppConfig::getSettingKeyValue(const QString &group, const QString &key, const QVariant &value)
{
    QSettings settings(strIniFile, QSettings::IniFormat);
    settings.beginGroup(group);
    return settings.value(key, value);
}

void AppConfig::checkDirs()
{
    QDir dir(strConfPath);
    if (!dir.exists()) {
        dir.mkdir(strConfPath);
    }
}
