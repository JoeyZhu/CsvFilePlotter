#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QPainter>

#include "csvplot.h"

//constructor for CsvPlot class
CsvPlot::CsvPlot(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);    //TODO: what's StrongFocus?

    xScale = 1;
    yScale = 1;
    loadTextFile();
}

void CsvPlot::loadTextFile(){
    QFile inputFile(":/input.txt");
    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);

    QStringList list;
    list.clear();

    int i = 0;
    while(!inputFile.atEnd()){
        QString oneLine = in.readLine();
        list = oneLine.split(",", QString::SkipEmptyParts);

        if((i > 0)&&(i < INPUT_ARRAY_ROWS)){
            DataPNum[i] = list.at(4).toInt();
        }
        i++;
    }
    inputFile.close();
}


void CsvPlot::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.setPen(QPen(Qt::blue, 2));

    QRect rect(5, 5, width() - 5*2, height() - 5*40);
    QPolygonF polyline(INPUT_ARRAY_ROWS);

    for(int i = 0; i < INPUT_ARRAY_ROWS; i++){
        polyline[i] = QPointF(rect.left() + i * xScale, rect.bottom() - DataPNum[i] * yScale);
    }
    painter.drawPolyline(polyline);
    //painter.drawLine(5, height() - 5 * 40, width(), height() - 5 * 40);
}




