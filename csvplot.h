#ifndef CSVPLOT_H
#define CSVPLOT_H

#include <QMainWindow>

namespace Ui {
class CsvPlot;
}

class CsvPlot : public QMainWindow
{
    Q_OBJECT
public:
    explicit CsvPlot(QWidget *parent = 0);
    ~CsvPlot();
signals:

public slots:

private:
    Ui::CsvPlot *ui;

};

#endif // CSVPLOT_H
