#include "aboutdlg.h"
#include "ui_aboutdlg.h"

#include <QMessageBox>

AboutDlg::AboutDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDlg)
{
    ui->setupUi(this);
}

AboutDlg::~AboutDlg()
{
    delete ui;
}

void AboutDlg::on_pushButton_clicked()
{
    QMessageBox::aboutQt(this,tr("关于QT"));
}
