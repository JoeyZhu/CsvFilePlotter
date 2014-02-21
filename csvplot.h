#ifndef CSVPLOT_H
#define CSVPLOT_H

#include <QWidget>
#include <QPixmap>

class QToolButton;
class QLabel;
class PlotSettings;     //save each zoom stage setting

#define INPUT_ARRAY_ROWS 7200       // TODO: change to const, const命名规范是啥？
#define Y_MAX 2800
#define Y_MIN 1000
#define OVER_POWER_TH 2500
#define TARGET_COLUMNS 1

class CsvPlot : public QWidget
{
    Q_OBJECT
public:
    explicit CsvPlot(QWidget *parent = 0);

    void setPlotSettings(const PlotSettings &settings);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
signals:

public slots:
    void zoomIn();
    void zoomOut();
    void open();

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

private:
    void loadTextFile(const QString &fileName, int i);
    void drawGrid(QPainter *painter);
    void drawCurve(QPainter *painter);
    void refreshPixmap();   //TODO: what's Pixmap?
    void csvPlotInit();
    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QToolButton *openFileButton;
    QPixmap pixmap;

    QLabel *infoLabel;

    enum { Margin = 50 };   //no plot area

    QVector<PlotSettings> zoomStack;
    int zoomStage;

    int DataPNum[INPUT_ARRAY_ROWS];

    double dataMin;
    double dataMax;
    double xScale;
    double yScale;

    int powerTH;
    int DataPNumPre;
    int powerOverCount;

    int maxCurve;
    long maxCurveIndex;
    int maxCurveFileIndex;
};


class PlotSettings{
public:
    PlotSettings();

    double spanX() const { return maxX - minX; }
    double spanY() const { return maxY - minY; }

    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;
};

#endif // CSVPLOT_H
