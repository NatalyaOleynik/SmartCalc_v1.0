#include "SmartCalc.h"

int calculator(char *str, double x, double *answ) {
  int res = OK;
  char new_str[512];
  token *result = NULL;
  token *lexeme = NULL;
  token *output_string = NULL;
  token *numbers = NULL;
  token *operators = NULL;
  token *rpn = NULL;
  if (!check_string(str, new_str)) res = ERROR;
  if (res) {
    if (!parser_token(new_str, x, &lexeme)) res = ERROR;
    if (res) {
      invers_stack(&lexeme, &output_string);
      reverse_polish_notacion(&output_string, &numbers, &operators);
      invers_stack(&numbers, &rpn);
      if (!calculate(&rpn, &result)) res = ERROR;
      if (res) {
        *answ = result->data;
      }
      free(result);
    }
    free(lexeme);
  }
  return res;
}

int is_empty(token *head) { return head == NULL; }

void push(token **head, double data, int type, int priority) {
  token *p = (token *)malloc(sizeof(token));
  if (p != NULL) {
    p->data = data;
    p->type = type;
    p->priority = priority;
    p->next = *head;
    *head = p;
  }
}

int pop(token **head) {
  token *p = *head;
  int res = p->data;
  *head = p->next;
  free(p);
  return res;
}

void clear(token **node) {
  while (!is_empty(*node)) {
    pop(node);
  }
}

int check_string(char *str, char *new_str) {
  int res = OK;
  remove_spaces(str, new_str);
  if (strlen(str) > BUFF) {
    res = ERROR;
    printf("TOO MANY CHARACTERS\n");
  } else if (!check_input_elements(new_str)) {
    res = ERROR;
    printf("ERROR\n");
  } else if (!check_bracket(new_str)) {
    res = ERROR;
    printf("ERROR\n");
  } else if (!check_input_expression(new_str)) {
    res = ERROR;
    printf("ERROR\n");
  }
  return res;
}

void remove_spaces(char *str, char *new_str) {
  int size = strlen(str);
  int j = 0;
  for (int i = 0; i < size; i++) {
    if (str[i] == ' ')
      continue;
    else {
      new_str[j] = str[i];
      j++;
    }
  }
  new_str[j] = '\0';
}

int check_bracket(char *str) {
  int res = OK;
  int open_bracket = 0, close_bracket = 0;
  for (size_t i = 0; (i < strlen(str)) && res; i++) {
    if (str[i] == '(') {
      open_bracket++;
      if (str[i + 1] == ')') res = ERROR;

    } else if (str[i] == ')')
      close_bracket++;
    if (close_bracket > open_bracket) res = ERROR;
  }
  if (open_bracket != close_bracket) res = ERROR;

  return res;
}

int check_input_elements(char *str) {
  int res = OK;
  for (size_t i = 0; i < strlen(str) && res; i++) {
    if (str[0] == ')' || str[0] == '.' || str[0] == '*' || str[0] == '/' ||
        str[0] == '^' || str[0] == 'm')
      res = ERROR;
    else if (DIGIT(str[i]) || str[i] == '.' || str[i] == 'x' || str[i] == 'X' ||
             str[i] == '(' || str[i] == ')' || str[i] == '+' || str[i] == '-' ||
             str[i] == '*' || str[i] == '/' || str[i] == '^')
      res = OK;
    else if (str[i] == 'l' && str[i + 1] == 'n') {
      res = OK;
      i += 1;
    } else if ((str[i] == 'm' && str[i + 1] == 'o' && str[i + 2] == 'd') ||
               (str[i] == 'c' && str[i + 1] == 'o' && str[i + 2] == 's') ||
               (str[i] == 's' && str[i + 1] == 'i' && str[i + 2] == 'n') ||
               (str[i] == 't' && str[i + 1] == 'a' && str[i + 2] == 'n') ||
               (str[i] == 'l' && str[i + 1] == 'o' && str[i + 2] == 'g')) {
      res = OK;
      i += 2;
    } else if ((str[i] == 'a' && str[i + 1] == 'c' && str[i + 2] == 'o' &&
                str[i + 3] == 's') ||
               (str[i] == 'a' && str[i + 1] == 's' && str[i + 2] == 'i' &&
                str[i + 3] == 'n') ||
               (str[i] == 'a' && str[i + 1] == 't' && str[i + 2] == 'a' &&
                str[i + 3] == 'n') ||
               (str[i] == 's' && str[i + 1] == 'q' && str[i + 2] == 'r' &&
                str[i + 3] == 't')) {
      res = OK;
      i += 3;
    } else
      res = ERROR;
  }
  return res;
}

int check_input_expression(char *str) {
  int res = OK;
  int size = strlen(str);
  for (int i = 0; i < size && res; i++) {
    if (str[i] == '(') {
      if (!check_open_bracket(str, i)) res = ERROR;
    } else if (str[i] == ')') {
      if (!check_close_bracket(str, i)) res = ERROR;
    } else if (DIGIT(str[i])) {
      if (!check_number(str, i)) res = ERROR;
    } else if (str[i] == 'x' || str[i] == 'X') {
      if (!check_x(str, i)) res = ERROR;
    } else if (str[i] == '.') {
      if (!check_dot(str, i)) res = ERROR;
    } else if (SIGN(str[i])) {
      if (!check_sign(str, i)) res = ERROR;
    } else if (str[i] == 'm') {
      if (!check_mod(str, i))
        res = ERROR;
      else
        i = i + 2;
    } else if (is_func(str, i)) {
      if (!check_func(str, &i)) res = ERROR;
    }
  }

  return res;
}

int check_open_bracket(char *str, int i) {
  int res = OK;
  if (i != 0) {
    if (str[i - 1] == ')' || DIGIT(str[i - 1]) || str[i - 1] == 'x')
      res = ERROR;
  }
  if (str[i + 1] == '.' || str[i + 1] == '*' || str[i + 1] == '/' ||
      str[i + 1] == '^' || str[i + 1] == 'm')
    res = ERROR;
  return res;
}

int check_close_bracket(char *str, int i) {
  int res = OK;
  if (str[i - 1] == '.' || str[i - 1] == '*' || str[i - 1] == '/' ||
      str[i - 1] == '+' || str[i - 1] == '-' || str[i - 1] == '^' ||
      str[i - 1] == 'd' || str[i - 1] == 'n' || str[i - 1] == 's' ||
      str[i - 1] == 'g' || str[i - 1] == 't')
    res = ERROR;
  if (DIGIT(str[i + 1]) || str[i + 1] == 'x' || str[i + 1] == 'X' ||
      str[i + 1] == '.' || str[i + 1] == 's' || str[i + 1] == 'c' ||
      str[i + 1] == 't' || str[i + 1] == 'a' || str[i + 1] == 'l')
    res = ERROR;
  return res;
}

int check_number(char *str, int i) {
  int res = OK;
  if (str[i + 1] == '(' || str[i + 1] == 'x' || str[i + 1] == 'X' ||
      str[i + 1] == 's' || str[i + 1] == 'c' || str[i + 1] == 't' ||
      str[i + 1] == 'a' || str[i + 1] == 'l')
    res = ERROR;
  return res;
}

int check_x(char *str, int i) {
  int res = OK;
  if (str[i + 1] == '(' || DIGIT(str[i + 1]) || str[i + 1] == 'x' ||
      str[i + 1] == 'X' || str[i - 1] == '.' || str[i + 1] == 's' ||
      str[i + 1] == 'c' || str[i + 1] == 't' || str[i + 1] == 'a' ||
      str[i + 1] == 'l')
    res = ERROR;
  return res;
}

int check_dot(char *str, int i) {
  int res = OK;
  if (!DIGIT(str[i - 1]) || !DIGIT(str[i + 1])) res = ERROR;
  return res;
}

int check_sign(char *str, int i) {
  int res = OK;
  if (SIGN(str[i + 1])) res = ERROR;
  return res;
}

int check_mod(char *str, int i) {
  int res = OK;
  if (str[i - 1] == '(' || str[i - 1] == '.' || str[i - 1] == '*' ||
      str[i - 1] == '/' || str[i - 1] == '+' || str[i - 1] == '-' ||
      str[i - 1] == '^' || str[i - 1] == 'd')
    res = ERROR;
  if (str[i + 3] == ')' || str[i + 3] == '.' || str[i + 3] == '*' ||
      str[i + 3] == '/' || str[i + 3] == '+' || str[i + 3] == '-' ||
      str[i + 3] == '^' || str[i + 3] == 'm')
    res = ERROR;
  return res;
}

int check_func(char *str, int *i) {
  int res = OK;
  if (strncmp(&str[*i], "ln(", 3) == 0) {
    res = OK;
    *i = *i + 2;
  } else if ((strncmp(&str[*i], "sin(", 4)) == 0 ||
             (strncmp(&str[*i], "cos(", 4)) == 0 ||
             (strncmp(&str[*i], "tan(", 4)) == 0 ||
             (strncmp(&str[*i], "log(", 4)) == 0) {
    res = OK;
    *i = *i + 3;
  } else if ((strncmp(&str[*i], "asin(", 5)) == 0 ||
             (strncmp(&str[*i], "acos(", 5)) == 0 ||
             (strncmp(&str[*i], "atan(", 5)) == 0 ||
             (strncmp(&str[*i], "sqrt(", 5)) == 0) {
    res = OK;
    *i = *i + 4;
  } else
    res = ERROR;
  return res;
}

int is_func(char *str, int i) {
  int res = ERROR;
  if (str[i] == 's' || str[i] == 'c' || str[i] == 't' || str[i] == 'a' ||
      str[i] == 'l')
    res = OK;
  return res;
}

int parser_token(char *input_expression, double x, token **lexeme) {
  int res = OK;
  int size = strlen(input_expression);
  for (int i = 0; i < size; i++) {
    if (DIGIT(input_expression[i]) || input_expression[i] == '.') {
      double number = 0.0;
      if (!reading_number(input_expression, &i, &number)) {
        res = ERROR;
        clear(lexeme);
        printf("ERROR\n");
        break;
      } else
        push(lexeme, number, NUMBER, 0);
    } else if (input_expression[i] == 'x' || input_expression[i] == 'X') {
      push(lexeme, x, X, 0);
    } else if (input_expression[i] == '(') {
      push(lexeme, 0.0, OPENING_BRACKET, -1);
    } else if (input_expression[i] == ')') {
      push(lexeme, 0.0, CLOSING_BRACKET, -1);
    } else if (input_expression[i] == '+') {
      if (check_un_sign(input_expression, i))
        push(lexeme, 0.0, UN_PLUS, 2);
      else
        push(lexeme, 0.0, ADD, 1);
    } else if (input_expression[i] == '-') {
      if (check_un_sign(input_expression, i))
        push(lexeme, 0.0, UN_MINUS, 2);
      else
        push(lexeme, 0.0, SUB, 1);
    } else if (input_expression[i] == '*') {
      push(lexeme, 0.0, MUL, 2);
    } else if (input_expression[i] == '/') {
      push(lexeme, 0.0, DIV, 2);
    } else if (input_expression[i] == 'm') {
      push(lexeme, 0.0, MOD, 2);
      i += 2;
    } else if (input_expression[i] == '^') {
      push(lexeme, 0.0, POW, 3);
    } else if (input_expression[i] == 's' && input_expression[i + 1] == 'i' &&
               input_expression[i + 2] == 'n') {
      push(lexeme, 0.0, SIN, 4);
      i += 2;
    } else if (input_expression[i] == 'c' && input_expression[i + 1] == 'o' &&
               input_expression[i + 2] == 's') {
      push(lexeme, 0.0, COS, 4);
      i += 2;
    } else if (input_expression[i] == 't' && input_expression[i + 1] == 'a' &&
               input_expression[i + 2] == 'n') {
      push(lexeme, 0.0, TAN, 4);
      i += 2;
    } else if (input_expression[i] == 'a' && input_expression[i + 1] == 's' &&
               input_expression[i + 2] == 'i' &&
               input_expression[i + 3] == 'n') {
      push(lexeme, 0.0, ARCSIN, 4);
      i += 3;
    } else if (input_expression[i] == 'a' && input_expression[i + 1] == 'c' &&
               input_expression[i + 2] == 'o' &&
               input_expression[i + 3] == 's') {
      push(lexeme, 0.0, ARCCOS, 4);
      i += 3;
    } else if (input_expression[i] == 'a' && input_expression[i + 1] == 't' &&
               input_expression[i + 2] == 'a' &&
               input_expression[i + 3] == 'n') {
      push(lexeme, 0.0, ARCTAN, 4);
      i += 3;
    } else if (input_expression[i] == 's' && input_expression[i + 1] == 'q' &&
               input_expression[i + 2] == 'r' &&
               input_expression[i + 3] == 't') {
      push(lexeme, 0.0, SQRT, 4);
      i += 3;
    } else if (input_expression[i] == 'l' && input_expression[i + 1] == 'n') {
      push(lexeme, 0.0, LN, 4);
      i += 1;
    } else if (input_expression[i] == 'l' && input_expression[i + 1] == 'o' &&
               input_expression[i + 2] == 'g') {
      push(lexeme, 0.0, LOG, 4);
      i += 2;
    }
  }
  return res;
}

int reading_number(char *str, int *i, double *number) {
  int res = OK;
  char str_number[BUFF] = {0};
  int j = 0;
  int count_dot = 0;

  while (DIGIT(str[*i]) || str[*i] == '.') {
    if (str[*i] == '.') count_dot++;
    if (count_dot > 1) {
      res = ERROR;
      break;
    }
    str_number[j] = str[*i];
    j++;
    *i = *i + 1;
  }
  *number = atof(str_number);
  *i = *i - 1;
  return res;
}

int check_un_sign(char *str, int i) {
  int res = ERROR;
  if (i == 0 || str[i - 1] == '(') res = OK;
  return res;
}

void invers_stack(token **input_string, token **output_string) {
  while (!is_empty(*input_string)) {
    push(output_string, (*input_string)->data, (*input_string)->type,
         (*input_string)->priority);
    pop(input_string);
  }
}

void reverse_polish_notacion(token **str, token **numbers, token **operators) {
  while (!is_empty(*str)) {
    if ((*str)->priority == 0) {
      push(numbers, (*str)->data, (*str)->type, (*str)->priority);
      pop(str);
    } else if ((*str)->type == OPENING_BRACKET) {
      push(operators, (*str)->data, (*str)->type, (*str)->priority);
      pop(str);
    } else if ((*str)->type == CLOSING_BRACKET) {
      while ((*operators)->type != OPENING_BRACKET) {
        push(numbers, (*operators)->data, (*operators)->type,
             (*operators)->priority);
        pop(operators);
      }
      pop(operators);
      pop(str);
    } else if (((*str)->priority != 0) || ((*str)->priority != -1)) {
      if (is_empty(*operators) || ((*str)->priority > (*operators)->priority)) {
        push(operators, (*str)->data, (*str)->type, (*str)->priority);
        pop(str);
      } else {
        while ((!is_empty(*operators)) &&
               (((*str)->priority == (*operators)->priority) ||
                ((*str)->priority < (*operators)->priority))) {
          push(numbers, (*operators)->data, (*operators)->type,
               (*operators)->priority);
          pop(operators);
        }
        push(operators, (*str)->data, (*str)->type, (*str)->priority);
        pop(str);
      }
    }
  }
  while (!is_empty(*operators)) {
    push(numbers, (*operators)->data, (*operators)->type,
         (*operators)->priority);
    pop(operators);
  }
}

int calculate(token **rpn, token **result) {
  int res = OK;
  while (!is_empty(*rpn)) {
    if ((*rpn)->priority == 0) {
      push(result, (*rpn)->data, (*rpn)->type, (*rpn)->priority);
      pop(rpn);
    } else if ((*rpn)->priority != 0) {
      if (((*rpn)->priority != 4) && ((*rpn)->type != UN_PLUS) &&
          ((*rpn)->type != UN_MINUS)) {
        if (!bin_expression((*rpn)->type, &(*result)->next->data,
                            &(*result)->data)) {
          res = ERROR;
          clear(rpn);
          clear(result);
          printf("ERROR: DIVISION BY ZERO!\n");
          break;
        } else {
          pop(rpn);
          pop(result);
        }
      } else if (!un_expression((*rpn)->type, &(*result)->data)) {
        res = ERROR;
        clear(rpn);
        clear(result);
        break;
      } else {
        pop(rpn);
      }
    }
  }
  return res;
}

int bin_expression(token_type type, double *data1, double *data2) {
  int res = OK;
  if (type == ADD)
    *data1 = *data1 + *data2;
  else if (type == SUB)
    *data1 = *data1 - *data2;
  else if (type == MUL)
    *data1 = *data1 * *data2;
  else if (type == DIV) {
    if (*data2 == 0)
      res = ERROR;
    else
      *data1 = *data1 / *data2;
  } else if (type == POW)
    *data1 = pow(*data1, *data2);
  else if (type == MOD) {
    if (*data2 == 0)
      res = ERROR;
    else
      *data1 = fmod(*data1, *data2);
  }
  return res;
}

int un_expression(token_type type, double *data) {
  int res = OK;
  if (type == UN_PLUS)
    *data = *data;
  else if (type == UN_MINUS)
    *data = *data * (-1);
  else if (type == COS)
    *data = cos(*data);
  else if (type == SIN)
    *data = sin(*data);
  else if (type == TAN)
    *data = tan(*data);
  else if (type == ARCCOS)
    *data = acos(*data);
  else if (type == ARCSIN)
    *data = asin(*data);
  else if (type == ARCTAN)
    *data = atan(*data);
  else if (type == SQRT) {
    if (*data < 0) {
      res = ERROR;
      printf("ERROR: NEGATIVE ARGUMENT.\n");
    } else
      *data = sqrt(*data);
  } else if (type == LN) {
    if (*data <= 0) {
      res = ERROR;
      printf("ERROR\n");
    } else
      *data = log(*data);
  } else if (type == LOG) {
    if (*data <= 0) {
      res = ERROR;
      printf("ERROR\n");
    } else
      *data = log10(*data);
  }

  return res;
}

// int main() {
//   char *str = "sqrt(-5)";
//   double x = 0.0;
//   double answer = 0.0;
//   calculator(str, x, &answer);
//   printf("%f\n", answer);
//   return 0;
// }