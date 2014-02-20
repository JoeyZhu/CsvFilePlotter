#include "csvplot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    CsvPlot csvPlot;
    csvPlot.setWindowTitle(QObject::tr("CsvFilePlotter"));
    csvPlot.show();

    return a.exec();
}
