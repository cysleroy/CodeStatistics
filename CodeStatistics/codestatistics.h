#ifndef CODESTATISTICS_H
#define CODESTATISTICS_H

#include <QDebug>
#include <QToolTip>
#include <QFileDialog>
#include <QMainWindow>
#include <QTranslator>
#include <QStandardItemModel>

#include "aboutdlg.h"
#include "appconfig.h"
#include "statistics.h"

namespace Ui {
class CodeStatistics;
}

class CodeStatistics : public QMainWindow
{
    Q_OBJECT

public:
    explicit CodeStatistics(QWidget *parent = 0);
    ~CodeStatistics();
private:
    void codeFilesStatistics(const QString &filePath);
    bool isSourceOrHeader(const QString &fileName);
    void statisticsCodeFiles(QStringList files);

    void clearResault();
private slots:
    void on_actionOpenFile_triggered();
    void on_actionOpenDir_triggered();
    void on_actionQuit_triggered();
    void on_actionClearModel_triggered();
    void on_actionCN_triggered();
    void on_actionEN_triggered();
    void on_actionUTF8_triggered();
    void on_actionGBK_triggered();
    void on_actionAbout_triggered();

    //
    void onShowModelToolTip(const QModelIndex &modelIndex);
private:
    Ui::CodeStatistics *ui;
    QStandardItemModel *model;
    QStringList headArgus;
    QStringList fileList;
    QTranslator languageTrans;
    AboutDlg *aboutDlg;
};

#endif // CODESTATISTICS_H
