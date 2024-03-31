#include "mainwindow.h"

#include "graph.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  graph = new Graph();
  connect(this, &MainWindow::signal, graph, &Graph::slot);

  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_sub, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(math_operations()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(math_functions()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(math_functions()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(math_functions()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(math_functions()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(math_functions()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(math_functions()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(math_functions()));
  connect(ui->pushButton_log10, SIGNAL(clicked()), this,
          SLOT(math_functions()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(math_functions()));
  connect(ui->pushButton_open_bracket, SIGNAL(clicked()), this,
          SLOT(brackets()));
  connect(ui->pushButton_close_bracket, SIGNAL(clicked()), this,
          SLOT(brackets()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::digits_numbers() {
  QPushButton *button = (QPushButton *)sender();

  ui->expression_show->setText(ui->expression_show->text() + button->text());
}

void MainWindow::on_pushButton_dot_clicked() {
  if (!(ui->expression_show->text().endsWith(".")))
    ui->expression_show->setText(ui->expression_show->text() + ".");
}

void MainWindow::on_pushButton_X_clicked() {
  if (!(ui->expression_show->text().endsWith("x")))
    ui->expression_show->setText(ui->expression_show->text() + "x");
}

void MainWindow::math_operations() {
  QPushButton *button = (QPushButton *)sender();

  if (button->text() == "+" && !(ui->expression_show->text().endsWith("+")))
    ui->expression_show->setText(ui->expression_show->text() + "+");
  else if (button->text() == "-" &&
           !(ui->expression_show->text().endsWith("-")))
    ui->expression_show->setText(ui->expression_show->text() + "-");
  else if (button->text() == "*" &&
           !(ui->expression_show->text().endsWith("*")))
    ui->expression_show->setText(ui->expression_show->text() + "*");
  else if (button->text() == "÷" &&
           !(ui->expression_show->text().endsWith("/")))
    ui->expression_show->setText(ui->expression_show->text() + "/");
  else if (button->text() == "mod" &&
           !(ui->expression_show->text().endsWith("mod")))
    ui->expression_show->setText(ui->expression_show->text() + "mod");
  else if (button->text() == "^" &&
           !(ui->expression_show->text().endsWith("^")))
    ui->expression_show->setText(ui->expression_show->text() + "^");
}

void MainWindow::math_functions() {
  QPushButton *button = (QPushButton *)sender();

  ui->expression_show->setText(ui->expression_show->text() + button->text() +
                               "(");
}

void MainWindow::brackets() {
  QPushButton *button = (QPushButton *)sender();

  ui->expression_show->setText(ui->expression_show->text() + button->text());
}

void MainWindow::on_pushButton_C_clicked() {
  ui->expression_show->setText("");
  ui->result_show->setText("0");
}

void MainWindow::on_pushButton_backspace_clicked() {
  QString expression_show = ui->expression_show->text();
  size_t len = expression_show.length();
  QString new_label;
  if (ui->expression_show->text().endsWith("acos") ||
      ui->expression_show->text().endsWith("asin") ||
      ui->expression_show->text().endsWith("atan") ||
      ui->expression_show->text().endsWith("sqrt"))
    new_label = expression_show.left(len - 4);
  else if (ui->expression_show->text().endsWith("cos") ||
           ui->expression_show->text().endsWith("sin") ||
           ui->expression_show->text().endsWith("tan") ||
           ui->expression_show->text().endsWith("log"))
    new_label = expression_show.left(len - 3);
  else if (ui->expression_show->text().endsWith("ln"))
    new_label = expression_show.left(len - 2);
  else
    new_label = expression_show.left(len - 1);
  ui->expression_show->setText(new_label);
}

void MainWindow::on_pushButton_equal_clicked() {
  int error = 1;

  std::string str = ui->expression_show->text().toStdString();
  char *expression = &str[0];
  double x = 0.0;
  x = (ui->x_show->text()).toDouble();

  double answer = 0.0;

  error = (calculator(expression, x, &answer));

  if (error == 1) {
    QString result = QString::number(answer, 'g', 15);
    ui->result_show->setText(result);
  }

  else
    ui->expression_show->setText("Error");
}

void MainWindow::on_pushButton_graph_clicked() {
  int flag = 0;

  if (ui->expression_show->text() == "") {
    ui->expression_show->setText("Error");
    flag = 1;
  }
  if (!flag && !(ui->expression_show->text() == "Error")) {
    emit signal(ui->expression_show->text());
    graph->show();
  }
}
