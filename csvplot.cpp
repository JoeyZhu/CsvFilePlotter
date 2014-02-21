#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QToolButton>
#include <QStylePainter>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

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

    openFileButton = new QToolButton(this);
    openFileButton->setText(tr("打开文件"));
    openFileButton->adjustSize();
    connect(openFileButton, SIGNAL(clicked()), this, SLOT(open()));

    infoLabel = new QLabel(this);
    infoLabel->setFixedWidth(400);
    infoLabel->setText(tr("Please Load Files"));

    setPlotSettings(PlotSettings());

    csvPlotInit();
}

void CsvPlot::csvPlotInit(){
    xScale = 1;
    yScale = 1;
    dataMin = Y_MIN;
    dataMax = Y_MAX;
    powerTH = OVER_POWER_TH;
    powerOverCount = 0;
    maxCurve = 0;
    maxCurveIndex = 0;
    maxCurveFileIndex = 0;
}

void CsvPlot::setPlotSettings(const PlotSettings &settings){
    zoomStack.clear();
    zoomStack.append(settings);
    zoomStage = 0;

    //need refresh?
    //refreshPixmap();
}

void CsvPlot::zoomIn(){
    if(xScale < 100){
        xScale++;
        zoomOutButton->setEnabled(true);
    }else{
        zoomInButton->setEnabled(false);
    }
    refreshPixmap();
}

void CsvPlot::zoomOut(){
    if(xScale > 1){
        zoomOutButton->setEnabled(true);
        xScale--;
    }else{
        zoomOutButton->setEnabled(false);
    }
    refreshPixmap();
}

void CsvPlot::open(){
    csvPlotInit();

    QStringList filenames = QFileDialog::getOpenFileNames(this);
    if(!filenames.isEmpty()){
        for(int i = 0; i < filenames.count(); i++){
            loadTextFile(filenames.at(i), i);
        }
        //infoLabel->setText(filename.at(0)+filename.at(1));
        //infoLabel->adjustSize();
    }
}

void CsvPlot::resizeEvent(QResizeEvent *){
    int x = width() - (zoomInButton->width());
    zoomInButton->move(x,5);
    x -= zoomInButton->width() + 5;
    zoomOutButton->move(x, 5);
    x -= openFileButton->width() + 5;
    openFileButton->move(x, 5);
    refreshPixmap();
}

void CsvPlot::loadTextFile(const QString &filename, int ithFile){
    QFile inputFile(filename);
    if(!inputFile.open(QFile::ReadOnly)){
        QMessageBox::warning(this, tr("Files"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(filename)
                             .arg(inputFile.errorString()));
        return;
    }
    QTextStream in(&inputFile);

    QStringList list;
    list.clear();

    int i = 0;
    while(!inputFile.atEnd()){
        QString oneLine = in.readLine();
        list = oneLine.split(",", QString::SkipEmptyParts);

        if((i > 0)&&(i < INPUT_ARRAY_ROWS)){
            DataPNum[i] = list.at(TARGET_COLUMNS).toInt();
            if((DataPNum[i] > powerTH) && (DataPNumPre < powerTH)){
                powerOverCount++;
            }
            DataPNumPre = DataPNum[i];
            if(DataPNum[i] > maxCurve){
                maxCurve = DataPNum[i];
                maxCurveIndex = i + ithFile * INPUT_ARRAY_ROWS;
            }

        }
        i++;
    }
    inputFile.close();

    infoLabel->setText(tr("超过") + QString::number(powerTH)
                       + tr("的次数为") + QString::number(powerOverCount)
                       + "\n" + tr("最大值为") + QString::number((maxCurve))
                       + tr("。序号是") + QString::number(maxCurveIndex));
    infoLabel->adjustSize();
    refreshPixmap();
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
    QRect gridRect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    if(!gridRect.isValid()){
        return;
    }

    PlotSettings settings = zoomStack[zoomStage];
    settings.maxX = gridRect.width();
    settings.maxX = settings.maxX / xScale;

//    settings.minY = -Margin*2;
//    settings.maxY = gridRect.height() - settings.minY;
    settings.minY = dataMin;
    settings.maxY = dataMax;

    QPen quiteDark = palette().dark().color().lighter();
    QPen light = palette().light().color();

    for(int i = 0; i <= settings.numXTicks; i++){
        int x = gridRect.left() + (i * (gridRect.width() - 1) / settings.numXTicks);
        double label = settings.minX + (i * settings.spanX() / settings.numXTicks);
        painter->setPen(quiteDark);
        painter->drawLine(x,gridRect.top(), x, gridRect.bottom());
        painter->setPen(light);
        painter->drawLine(x, gridRect.bottom(), x, gridRect.bottom() + 5);
        painter->drawText(x - 50, gridRect.bottom() + 5, 100, 20,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(label));
    }
    for(int j = 0; j <= settings.numYTicks; j++){
        int y = gridRect.bottom() - (j * (gridRect.height() - 1) / settings.numYTicks);
        double label = settings.minY + (j * settings.spanY() / settings.numYTicks);
        painter->setPen(quiteDark);
        painter->drawLine(gridRect.left(), y, gridRect.right(), y);
        painter->setPen(light);
        painter->drawLine(gridRect.left() - 5, y, gridRect.left(), y);
        painter->drawText(gridRect.left() - Margin, y - 10, Margin - 5,20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
    }
    // Draw 0 axis
    double zeroPosition = gridRect.bottom() - (0 - dataMin) * (gridRect.height()) / (dataMax - dataMin);
    painter->drawLine(gridRect.left()-5, zeroPosition, gridRect.right(), zeroPosition);
    painter->drawText(gridRect.left() - Margin, zeroPosition - 10, Margin - 5,20,
                      Qt::AlignRight | Qt::AlignVCenter,
                      QString::number(0));

    // need draw another rect?
    //painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void CsvPlot::drawCurve(QPainter *painter){

        painter->setPen(QPen(Qt::yellow, 1));

        QRect graphRect(Margin, Margin, width() - Margin*2, height() - Margin*2);
        QPolygonF polyline(INPUT_ARRAY_ROWS);

        for(int i = 0; i < INPUT_ARRAY_ROWS; i++){
            polyline[i] = QPointF(graphRect.left() + i * xScale, graphRect.bottom() - (DataPNum[i] - dataMin) * (graphRect.height()) / (dataMax - dataMin));
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
