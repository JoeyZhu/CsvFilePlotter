#ifndef CSVPLOT_H
#define CSVPLOT_H

#include <QWidget>

#define INPUT_ARRAY_ROWS 7200       // TODO: change to const, const命名规范是啥？


class CsvPlot : public QWidget      //直接继承Qwidget不行吗？ 02201708
{
    Q_OBJECT
public:
    explicit CsvPlot(QWidget *parent = 0);
signals:

public slots:

private:
    void loadTextFile();
    int DataPNum[INPUT_ARRAY_ROWS];
    double xScale;
    double yScale;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // CSVPLOT_H
