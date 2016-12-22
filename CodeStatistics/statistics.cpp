#include "statistics.h"

Statistics::Statistics(QObject *parent)
    : QObject(parent)
{

}

CodeFileInfo Statistics::statisFile(const QString &fileName)
{
    CodeFileInfo codeFileInfo;
    memset(&codeFileInfo, 0, sizeof(CodeFileInfo));

    if (fileName.isEmpty())
        return codeFileInfo;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))  return codeFileInfo;

    QTextStream out(&file);
    out.setCodec(AppConfig::strCodec.toLatin1().data());

    QString strTemp = "";
    bool bNotes = false;
    int nLineCnt = 0;
    while (!out.atEnd()) {
        strTemp = out.readLine();
        strTemp = remoeBlankFront(strTemp);

        if (strTemp.startsWith("/*")) {
            bNotes = true;
        }

        // /**/注释部分
        if (bNotes) codeFileInfo.lineNotes++;
        else {
            // "//"注释部分
            if (strTemp.startsWith("//"))
            {
                codeFileInfo.lineNotes++;
            }
            // 空白行
            else if (strTemp.isEmpty())
            {
                codeFileInfo.lineBlank++;
            }
            // 代码行
            else {
                codeFileInfo.lineCode++;
            }
        }
        if (strTemp.endsWith("*/"))
            bNotes = false;
        nLineCnt++;
    }
    return codeFileInfo;
}

QString Statistics::remoeBlankFront(const QString &strTxt)
{
    // 如果为空，直接返回
    if (strTxt.isEmpty())
        return NULL;
    QString strRet = strTxt;
    // 去掉这一行中所有的空格
    if (strRet.startsWith(" ")) {
        strRet.remove(" ");
    }

    return strRet;
}
