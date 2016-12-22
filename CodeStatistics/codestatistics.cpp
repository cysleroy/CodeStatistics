#include "codestatistics.h"
#include "ui_codestatistics.h"

CodeStatistics::CodeStatistics(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CodeStatistics)
{
    ui->setupUi(this);
    aboutDlg=new AboutDlg(nullptr);
    model=new QStandardItemModel(this);
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setMouseTracking(true);
    connect(ui->tableView,SIGNAL(entered(QModelIndex)),
            this,SLOT(onShowModelToolTip(QModelIndex)));
    headArgus = QStringList() << tr("文件") << tr("类型")<< tr("路径")<< tr("大小") << tr("总行数")
                              << tr("代码行数") << tr("注释行数")<< tr("空白行数");
    model->setHorizontalHeaderLabels(headArgus);
}

CodeStatistics::~CodeStatistics()
{
    delete ui;
    if(aboutDlg!=NULL)
    {
        delete aboutDlg;
        aboutDlg=NULL;
    }
}

void CodeStatistics::codeFilesStatistics(const QString &filePath)
{
    QDir dir(filePath);
    foreach (QFileInfo mfi ,dir.entryInfoList())
    {
        if (mfi.isFile())
        {
            QString strFileName = mfi.fileName();
            if (isSourceOrHeader(strFileName))
            {
                fileList << mfi.filePath();
            }
        }
        else
        {
            if(mfi.fileName()=="." || mfi.fileName() == "..") continue;
            codeFilesStatistics(mfi.absoluteFilePath());
        }
    }
}

bool CodeStatistics::isSourceOrHeader(const QString &fileName)
{
    return ((fileName.endsWith(".c") || fileName.endsWith(".cpp") || fileName.endsWith(".h"))
            && (!fileName.startsWith("moc_") && !fileName.startsWith("ui_")
                &&!fileName.startsWith("qrc_")));
}

void CodeStatistics::statisticsCodeFiles(QStringList files)
{
    // 清空列表
    clearResault();
    CodeFileInfo codeFileInfo;
    memset(&codeFileInfo, 0, sizeof(CodeFileInfo));

    int nRows = model->rowCount();
    model->setRowCount(nRows + files.size());

    quint32 nTotalLines  = 0;
    quint32 nTotalFiles  = files.size();
    quint32 nTotalBytes  = 0;
    quint32 nTotalCodes  = 0;
    quint32 nTotalNotes  = 0;
    quint32 nTotalBlanks = 0;

    for (int i = nRows; i < files.size(); i++)
    {
        QFileInfo fileInfo(files.at(i));
        codeFileInfo = Statistics::statisFile(fileInfo.filePath());
        int nLines = codeFileInfo.lineCode + codeFileInfo.lineBlank + codeFileInfo.lineNotes;
        model->setData(model->index(i, 0), fileInfo.fileName());
        model->setData(model->index(i, 1), QString("*.") + fileInfo.suffix());
        model->setData(model->index(i, 2), fileInfo.filePath());
        model->setData(model->index(i, 3), fileInfo.size());
        model->setData(model->index(i, 4), QString::number(nLines));
        model->setData(model->index(i, 5), QString::number(codeFileInfo.lineCode));
        model->setData(model->index(i, 6), QString::number(codeFileInfo.lineNotes));
        model->setData(model->index(i, 7), QString::number(codeFileInfo.lineBlank));

        nTotalBytes  += fileInfo.size();
        nTotalLines  += nLines;
        nTotalCodes  += codeFileInfo.lineCode;
        nTotalNotes  += codeFileInfo.lineNotes;
        nTotalBlanks += codeFileInfo.lineBlank;
    }

    fileList.clear();
    ui->fileCountEdit->setText(QString::number(nTotalFiles));
    ui->byteEdit->setText(QString::number(nTotalBytes));
    ui->lineEdit->setText(QString::number(nTotalLines));
    ui->codeCountEdit->setText(QString::number(nTotalCodes));
    ui->commentEdit->setText(QString::number(nTotalNotes));
    ui->blankEdit->setText(QString::number(nTotalBlanks));
    double dbTemp = 0.0;
    dbTemp = (double)nTotalCodes * 100 / nTotalLines;
    ui->labelCodePercent->setText(QString("[ %1%]").arg(dbTemp, 5, 'f', 2, QChar(' ')));
    dbTemp = (double)nTotalNotes * 100 / nTotalLines;
    ui->labelCommentPercent->setText(QString("[ %1%]").arg(dbTemp, 5, 'f', 2, QChar(' ')));
    dbTemp = (double)nTotalBlanks * 100 / nTotalLines;
    ui->labelBlankPercent->setText(QString("[ %1%]").arg(dbTemp, 5, 'f', 2, QChar(' ')));
    ui->statusBar->showMessage(tr("文件统计完成"));
}

void CodeStatistics::clearResault()
{
    model->clear();
    model->setHorizontalHeaderLabels(headArgus);
    ui->fileCountEdit->clear();
    ui->codeCountEdit->clear();
    ui->byteEdit->clear();
    ui->commentEdit->clear();
    ui->lineEdit->clear();
    ui->blankEdit->clear();
    ui->labelCodePercent->setText("0%");
    ui->labelCommentPercent->setText("0%");
    ui->labelBlankPercent->setText("0%");
}

void CodeStatistics::on_actionOpenFile_triggered()
{
    QStringList strFiles = QFileDialog::getOpenFileNames(this, tr("选择文件"),
                                                   AppConfig::strAppPath,
                                                   "文件(*.h *.cpp *.c)");
    if (strFiles.size() <= 0) {
        ui->statusBar->showMessage(tr("文件选择错误！"));
        return;
    }
    statisticsCodeFiles(strFiles);
}

void CodeStatistics::on_actionOpenDir_triggered()
{
    QString strPath = QFileDialog::getExistingDirectory(this, tr("选择目录"),
                                                        AppConfig::strAppPath,
                                                        QFileDialog::ShowDirsOnly |
                                                        QFileDialog::DontResolveSymlinks);
    if (strPath.isEmpty()) {
        ui->statusBar->showMessage(tr("未选择任何目录！"));
        return;
    }

    // 判断是否有可用文件
    fileList.clear();
    // 统计代码文件
    codeFilesStatistics(strPath);

    // 如没有文件，直接返回
    if (fileList.size() <= 0)  {
        ui->statusBar->showMessage(tr("选择目录没有代码文件！"));
        return;
    }

    // 执行统计
    statisticsCodeFiles(fileList);
}

void CodeStatistics::on_actionQuit_triggered()
{
    this->close();
}

void CodeStatistics::on_actionClearModel_triggered()
{
     clearResault();
}

void CodeStatistics::on_actionCN_triggered()
{
    languageTrans.load("tr_zh.qm", AppConfig::strAppPath + "translations/");
    qApp->installTranslator(&languageTrans);
    headArgus.clear();
    headArgus = QStringList() << tr("文件") << tr("类型")
                              << tr("路径")  << tr("大小") << tr("总行数")
                              << tr("代码行数") << tr("注释行数")
                              << tr("空白行数");
   model->setHorizontalHeaderLabels(headArgus);

    AppConfig::strLangeage = "zh";
    AppConfig::setSettingFile("Setup", "Language", "zh");
    ui->actionEN->setChecked(false);
}

void CodeStatistics::on_actionEN_triggered()
{
    // 加载英文翻译
    languageTrans.load("tr_en.qm", AppConfig::strAppPath + "translations/");
    qApp->installTranslator(&languageTrans);

    headArgus.clear();
    headArgus = QStringList() << tr("文件") << tr("类型")
                              << tr("路径")  << tr("大小") << tr("总行数")
                              << tr("代码行数") << tr("注释行数")
                              << tr("空白行数");
    model->setHorizontalHeaderLabels(headArgus);

    AppConfig::strLangeage = "en";
    AppConfig::setSettingFile("Setup", "Language", "en");
    ui->actionCN->setChecked(false);
}

void CodeStatistics::on_actionUTF8_triggered()
{
    AppConfig::strCodec = "UTF8";
    AppConfig::setSettingFile("Setup", "Codec", "UTF8");
    ui->actionGBK->setChecked(false);
}

void CodeStatistics::on_actionGBK_triggered()
{
    AppConfig::strCodec = "GBK";
    AppConfig::setSettingFile("Setup", "Codec", "GB18030");
    ui->actionUTF8->setChecked(false);
}

void CodeStatistics::on_actionAbout_triggered()
{
    aboutDlg->show();
}

void CodeStatistics::onShowModelToolTip(const QModelIndex &modelIndex)
{
    if (!modelIndex.isValid()) {
        qDebug() << "Invalid index";
        return;
    }

    QString strData = headArgus.at(modelIndex.column());
    strData.append("\n");
    strData.append(modelIndex.data().toString());

    QToolTip::showText(QCursor::pos(), strData);
}
