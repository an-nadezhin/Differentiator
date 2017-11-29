//
// Created by andrew on 22.11.17.
//

#ifndef DIFFERENTIATOR_DIFFERENTIATOR_H
#define DIFFERENTIATOR_DIFFERENTIATOR_H



const char* s = NULL;

int p = 0;

const int BUF_LENGTH = 50;

const int ERROR = -1;

enum UnOp {
    SIN, COS, SQRT, UN_MINUS
};

enum BinOp {
    ADD, MUL, SUB, DIV
};

class Expression {
public:
    Expression();
    virtual void print_dot(FILE *code);
    virtual void print_dot_name(FILE *code);
    virtual Expression *derivative(char *);
    virtual Expression *optimize();
};

class Variable : public Expression {
public:
    Variable(char *);
    void print_dot_name(FILE *code);
    ~Variable();
    Expression * derivative(char*);
private:
    char *var_name;
};

class Number : public Expression {
public:
    Number(double);
    void print_dot_name(FILE *code);
    Expression * derivative(char*);
    double num();

private:
    double num_val;
};

class Operator_un : public Expression {
public:
    Operator_un(int, Expression *);
    void print_dot(FILE *code);
    void print_dot_name(FILE *code);
    Expression * derivative(char*);
    Expression *optimize();
private:
    int type_un_op;
    Expression *arg_f;
};

class Operator_bin : public Expression {
public:
    Operator_bin(int, Expression *, Expression *);
    void print_dot(FILE* code);
    void print_dot_name(FILE *code);
    Expression *derivative(char *);
    Expression *optimize();
private:
    int type_bin_op;
    Expression *arg_f;
    Expression *arg_s;
};

void print_JPEG(Expression *diff);

void skip_s(char *buffer, int position);

int compare_str_bin(char value);

int compare_str_un(char *value);

Expression *create_tree(char *buffer, int *position);

void skip_str();


#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
