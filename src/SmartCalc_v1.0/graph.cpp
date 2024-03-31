#include "graph.h"

#include "../SmartCalc.h"
#include "mainwindow.h"
#include "ui_graph.h"

Graph::Graph(QWidget *parent) : QWidget(parent), ui(new Ui::Graph) {
  ui->setupUi(this);

  ui->widget->xAxis->setLabel("x");
  ui->widget->yAxis->setLabel("y");

  ui->widget->xAxis->setRange(-10, 10);
  ui->widget->yAxis->setRange(-10, 10);

  ui->xmin->setText("-10");
  ui->xmax->setText("10");
  ui->ymin->setText("-10");
  ui->ymax->setText("10");
}

Graph::~Graph() { delete ui; }

void Graph::on_pushButton_Draw_clicked() {
  ui->widget->clearGraphs();

  xBegin = (ui->xmin->text()).toDouble();
  xEnd = (ui->xmax->text()).toDouble();
  yBegin = (ui->ymin->text()).toDouble();
  yEnd = (ui->ymax->text()).toDouble();

  if (fabs(xBegin) > 1000000 || fabs(xEnd) > 1000000 ||
      fabs(yBegin) > 1000000 || fabs(yEnd) > 1000000 || xBegin > xEnd ||
      yBegin > yEnd)
    ui->error->setText("Incorrect value");
  else {
    ui->error->setText(" ");

    h = (xEnd - xBegin) / 1000;

    ui->widget->xAxis->setRange(xBegin, xEnd);
    ui->widget->yAxis->setRange(yBegin, yEnd);

    double answer = 0.0;
    std::string str = ui->expression_show->text().toStdString();
    char *expression = &str[0];

    for (X = xBegin; X <= xEnd; X += h) {
      calculator(expression, X, &answer);

      x.push_back(X);
      y.push_back(answer);
    }

    ui->widget->addGraph();
    ui->widget->graph(0)->addData(x, y);
    ui->widget->replot();

    x.clear();
    y.clear();
  }
}

void Graph::slot(QString func) { ui->expression_show->setText(func); }
