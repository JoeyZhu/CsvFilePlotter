#include "csvplot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    CsvPlot c;
    c.show();

    return a.exec();
}
