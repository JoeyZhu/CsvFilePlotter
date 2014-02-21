#ifndef CSVPLOT_H
#define CSVPLOT_H

#include <QWidget>
#include <QPixmap>

class QToolButton;
class PlotSettings;     //save each zoom stage setting

#define INPUT_ARRAY_ROWS 7200       // TODO: change to const, const命名规范是啥？


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

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

private:
    void loadTextFile();
    void drawGrid(QPainter *painter);
    void drawCurve(QPainter *painter);
    void refreshPixmap();   //TODO: what's Pixmap?
    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QPixmap pixmap;

    enum { Margin = 50 };   //no plot area

    QVector<PlotSettings> zoomStack;
    int zoomStage;

    int DataPNum[INPUT_ARRAY_ROWS];


    double xScale;
    double yScale;

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













