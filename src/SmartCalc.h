#ifndef SRC_SMARTCALC_H_
#define SRC_SMARTCALC_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define ERROR 0
#define BUFF 255
#define DIGIT(data) (data >= '0' && data <= '9')
#define SIGN(data) \
  (data == '*' || data == '/' || data == '+' || data == '-' || data == '^')

typedef enum {
  NUMBER,
  X,
  OPENING_BRACKET,
  CLOSING_BRACKET,
  ADD,
  SUB,
  MUL,
  DIV,
  POW,
  MOD,
  UN_PLUS,
  UN_MINUS,
  COS,
  SIN,
  TAN,
  ARCCOS,
  ARCSIN,
  ARCTAN,
  SQRT,
  LN,
  LOG
} token_type;

typedef struct Node {
  double data;
  token_type type;
  int priority;
  struct Node *next;
} token;

int calculator(char *str, double x, double *answ);
int is_empty(token *head);
void push(token **head, double data, int type, int priority);
int pop(token **head);
void clear(token **node);

int check_string(char *str, char *new_str);
void remove_spaces(char *str, char *new_str);
int check_bracket(char *str);
int check_input_elements(char *str);
int check_input_expression(char *str);
int check_open_bracket(char *str, int i);
int check_close_bracket(char *str, int i);
int check_number(char *str, int i);
int check_x(char *str, int i);
int check_dot(char *str, int i);
int is_sign(char *str, int i);
int check_sign(char *str, int i);
int check_mod(char *str, int i);
int is_func(char *str, int i);
int check_func(char *str, int *i);
int parser_token(char *input_expression, double x, token **lexeme);
int reading_number(char *str, int *i, double *number);
int check_un_sign(char *str, int i);
void invers_stack(token **input_string, token **output_string);
void reverse_polish_notacion(token **str, token **numbers, token **operators);
int calculate(token **rpn, token **result);
int bin_expression(token_type type, double *data1, double *data2);
int un_expression(token_type type, double *data);
#endif  // SRC_SMARTCALC_H_