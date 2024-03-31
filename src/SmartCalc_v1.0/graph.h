#ifndef GRAPH_H
#define GRAPH_H

extern "C" {
#include "../SmartCalc.h"
}

#include <QVector>
#include <QWidget>

namespace Ui {
class Graph;
}

class Graph : public QWidget {
  Q_OBJECT

 public:
  explicit Graph(QWidget *parent = nullptr);
  ~Graph();

 private slots:
  void on_pushButton_Draw_clicked();

 private:
  Ui::Graph *ui;
  double xBegin, xEnd, yBegin, yEnd, h, X;

  QVector<double> x, y;

 public slots:
  void slot(QString func);
};

#endif  // GRAPH_H
