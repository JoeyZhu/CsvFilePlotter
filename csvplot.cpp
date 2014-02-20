#include "csvplot.h"
#include "ui_CsvPlot.h"

//constructor for CsvPlot class
CsvPlot::CsvPlot(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CsvPlot)     //TODO: don't know what is ui
{
    ui->setupUi(this);
}

CsvPlot::~CsvPlot(){
    delete ui;
}
