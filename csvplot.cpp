#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QToolButton>
#include <QStylePainter>

#include "csvplot.h"

//constructor for CsvPlot class
CsvPlot::CsvPlot(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);    //TODO: what's StrongFocus?

    zoomInButton = new QToolButton(this);
    zoomInButton->setIcon(QIcon(":/images/zoomin.png"));
    zoomInButton->adjustSize();
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

    zoomOutButton = new QToolButton(this);
    zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));
    zoomOutButton->adjustSize();
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    setPlotSettings(PlotSettings());

    xScale = 1;
    yScale = 1;
    loadTextFile();
}

void CsvPlot::setPlotSettings(const PlotSettings &settings){
    zoomStack.clear();
    zoomStack.append(settings);
    zoomStage = 0;
    zoomInButton->hide();
    zoomOutButton->hide();
    //need refresh?
    //refreshPixmap();
}

void CsvPlot::zoomIn(){
    xScale++;
    update();
}

void CsvPlot::zoomOut(){
    xScale--;
    update();
}

void CsvPlot::resizeEvent(QResizeEvent *){
    int x = width() - (zoomInButton->width() + zoomOutButton->width() + 10);
    zoomInButton->move(x,5);
    zoomOutButton->move(x + zoomInButton->width() + 5, 5);
    refreshPixmap();
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

void CsvPlot::paintEvent(QPaintEvent */*event*/){

    QStylePainter painter(this);
    painter.drawPixmap(0, 0, pixmap);       //TODO: must have this, painter.draw can take effect in other functions.
}

void CsvPlot::refreshPixmap(){

    pixmap = QPixmap(size());

    QPainter painter(&pixmap);
    painter.initFrom(this);
    drawGrid(&painter);
    drawCurve(&painter);
    update();
}

void CsvPlot::drawGrid(QPainter *painter){
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    if(!rect.isValid()){
        return;
    }

    PlotSettings settings = zoomStack[zoomStage];
    settings.maxX = width() - Margin*2;
    settings.maxY = height() - Margin*2;
    QPen quiteDark = palette().dark().color().lighter();
    QPen light = palette().light().color();

    for(int i = 0; i <= settings.numXTicks; i++){
        int x = rect.left() + (i * (rect.width() - 1) / settings.numXTicks);
        double label = settings.minX + (i * settings.spanX() / settings.numXTicks);
        painter->setPen(quiteDark);
        painter->drawLine(x,rect.top(), x, rect.bottom());
        painter->setPen(light);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, 20,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(label));
    }
    for(int j = 0; j <= settings.numYTicks; j++){
        int y = rect.bottom() - (j * (rect.height() - 1) / settings.numYTicks);
        double label = settings.minY + (j * settings.spanY() / settings.numYTicks);
        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(light);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - Margin, y - 10, Margin - 5,20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
    }
    // need draw another rect?
    //painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void CsvPlot::drawCurve(QPainter *painter){

        painter->setPen(QPen(Qt::yellow, 1));

        QRect rect(Margin, Margin, width() - Margin*2, height() - Margin*4);
        QPolygonF polyline(INPUT_ARRAY_ROWS);

        for(int i = 0; i < INPUT_ARRAY_ROWS; i++){
            polyline[i] = QPointF(rect.left() + i * xScale, rect.bottom() - DataPNum[i] * yScale);
        }
        painter->drawPolyline(polyline);
}

QSize CsvPlot::minimumSizeHint() const
{
    return QSize(6 * Margin, 4 * Margin);
}

QSize CsvPlot::sizeHint() const{
    return QSize(12 * Margin, 8 * Margin);  // default size: 800*600
}

PlotSettings::PlotSettings(){
    minX = 0.0;
    maxX = 10.0;
    numXTicks = 5;

    minY = 0.0;
    maxY = 10.0;
    numYTicks = 5;
}
