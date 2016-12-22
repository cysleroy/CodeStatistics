#include "codestatistics.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CodeStatistics w;
    w.show();
    return a.exec();
}
