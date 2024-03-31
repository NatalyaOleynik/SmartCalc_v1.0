#ifndef MAINWINDOW_H
#define MAINWINDOW_H

extern "C" {
#include "../SmartCalc.h"
}

#include <QMainWindow>

#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  Graph *graph;

 private slots:
  void digits_numbers();
  void on_pushButton_dot_clicked();
  void on_pushButton_C_clicked();
  void on_pushButton_equal_clicked();
  void math_operations();
  void math_functions();
  void brackets();
  void on_pushButton_X_clicked();
  void on_pushButton_backspace_clicked();
  void on_pushButton_graph_clicked();

 signals:
  void signal(QString);
};

#endif  // MAINWINDOW_H
