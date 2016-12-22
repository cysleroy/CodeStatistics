#ifndef STATISTICS_H
#define STATISTICS_H

#include <QFile>
#include <QObject>
#include <QTextStream>

#include "appconfig.h"

// 文件
typedef struct CodeFileInfo{
    int lineCode;
    int lineBlank;
    int lineNotes;
} CodeFileInfo;

class Statistics : public QObject
{
    Q_OBJECT
public:
    explicit Statistics(QObject *parent = 0);

    static CodeFileInfo statisFile(const QString &fileName);

private:
    static QString remoeBlankFront(const QString &strTxt);
};
#endif // STATISTICS_H
