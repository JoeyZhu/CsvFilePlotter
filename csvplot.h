#ifndef CSVPLOT_H
#define CSVPLOT_H

#include <QMainWindow>

#define INPUT_ARRAY_ROWS 7200       // TODO: change to const, const命名规范是啥？

namespace Ui {
class CsvPlot;
}

class CsvPlot : public QMainWindow      //直接继承Qwidget不行吗？ 02201708
{
    Q_OBJECT
public:
    explicit CsvPlot(QWidget *parent = 0);
    ~CsvPlot();
signals:

public slots:

private:
    Ui::CsvPlot *ui;
    void loadTextFile();
    int DataPNum[INPUT_ARRAY_ROWS];
    double xScale;
    double yScale;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // CSVPLOT_H
