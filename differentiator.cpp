#include <iostream>
#include "differentiator.h"
#include <cstring>
#include <cassert>

void texmaker();

Expression *GetG0(const char *text);

Expression *GetN();

Expression *GetE();

Expression *GetId();

Expression *GetT();

Expression *GetP();

int main() {
    int pos = 0;
    char *buf = "2 + (3*x + 2)";

    Expression *node_1 = GetG0(buf);
    Expression *node_2 = node_1->derivative("x");
    Expression *node_3 = node_2->optimize();
    print_JPEG(node_1);

    //texmaker();
}

void texmaker() {
    FILE *texf = fopen("expression.tex", "w");
    fprintf(texf, "\\documentclass{article}\n"
            "\\begin{document}\n"
            "    Hello, world!\n i am andrew \n"
            "\\end{document}");
    fclose(texf);
    system("pdflatex expression.tex");
    //   system("");
}


Expression *GetG0(const char *text) {
    s = strdup(text);
    p = 0;
    Expression *val = GetE();
    return val;
}

Expression *GetE() {
    Expression *val = GetT();
    skip_str();
    while (s[p] == '+' || s[p] == '-') {
        int op = compare_str_bin(s[p]);
        p++;
        skip_str();
        Expression *val2 = GetT();
        return new Operator_bin(op, val, val2);
    }
}

Expression *GetT() {
    skip_str();
    Expression *val = GetP();

    skip_str();

    while (s[p] == '*' || s[p] == '/') {
        int op = compare_str_bin(s[p]);
        p++;
        skip_str();
        Expression *val2 = GetP();
        return new Operator_bin(op, val, val2);
    }
    return val;
}

Expression *GetP() {
    if (s[p] == '(') {
        p++;
        Expression *val = GetE();
        assert(s[p == ')']);
        p++;
        return val;
    } else {
        int prev_p = p;
        Expression *val = GetN();
        if (prev_p != p)
            return val;
        else
            return GetId();
    }
}


Expression *GetN() {
    int val = 0;
    while ('0' <= s[p] && s[p] <= '9') {
        val = val * 10 + s[p] - '0';
        p++;
    }
    return new Number(val);
}


Expression *GetId() {
    int counter = 0;
    char value[BUF_LENGTH] = {0};

    skip_str();
    while (isalpha(s[p])) {
        value[counter++] = s[p++];
    }
    return new Variable(value);
}


void skip_str() {
    while (isspace(s[p]))
        p++;
}


/*

Expression *create_tree(char *buffer, int *position) {

    skip_s(buffer, position);

    if (buffer[*position] == '(') {
        int counter = 0;
        char value[BUF_LENGTH] = {0};
        (*position)++;
        while (!isspace(buffer[*position])) {
            value[counter++] = buffer[(*position)++];
        }
        int bin_act = compare_str_bin(value);
        if (bin_act != ERROR) {
            skip_s(buffer, position);
            Expression *new_node_l = create_tree(buffer, position);
            skip_s(buffer, position);
            Expression *new_node_r = create_tree(buffer, position);
            skip_s(buffer, position);
            assert(buffer[*position] == ')');
            (*position)++;
            return new Operator_bin(bin_act, new_node_l, new_node_r);
        }

        int un_act = compare_str_un(value);
        if (un_act != ERROR) {
            skip_s(buffer, position);
            Expression *new_node_l = create_tree(buffer, position);
            skip_s(buffer, position);
            assert(buffer[*position] == ')');
            (*position)++;
            return new Operator_un(un_act, new_node_l);
        }
        assert(0);
    }

    int counter = 0;
    char value[BUF_LENGTH] = {0};
    while (!isspace(buffer[*position]) && buffer[*position] != ')')
        value[counter++] = buffer[(*position)++];

    if (isalpha(value[0])) {
        return new Variable(value);
    }

    if (isdigit(value[0])) {
        return new Number(atof(value));
    }
    assert(0);
}

*/

int compare_str_bin(char value) {

    if (value == '+') return ADD;
    if (value == '-') return SUB;
    if (value == '*') return MUL;
    if (value == '/') return DIV;
    return ERROR;

}

int compare_str_un(char *value) {

    if (!strcmp(value, "sin")) return SIN;
    if (!strcmp(value, "cos")) return COS;
    if (!strcmp(value, "sqrt")) return SQRT;
    if (!strcmp(value, "neg")) return UN_MINUS;
    return ERROR;
}


void skip_s(char *buffer, int *position) {
    while (isspace(buffer[*position]))
        (*position)++;
}

Expression::Expression() {
}


void Expression::print_dot_name(FILE *code) {
}

void Operator_un::print_dot(FILE *code) {
    fprintf(code, "\"");
    print_dot_name(code);
    fprintf(code, "\"->\"");
    arg_f->print_dot_name(code);
    fprintf(code, "\"\n");
    arg_f->print_dot(code);
}

void Operator_bin::print_dot(FILE *code) {
    fprintf(code, "\"");
    print_dot_name(code);
    fprintf(code, "\"->\"");
    arg_f->print_dot_name(code);
    fprintf(code, "\"\n");
    arg_f->print_dot(code);

    fprintf(code, "\"");
    print_dot_name(code);
    fprintf(code, "\"->\"");
    arg_s->print_dot_name(code);
    fprintf(code, "\"\n");
    arg_s->print_dot(code);
}

void Expression::print_dot(FILE *code) {
}

Variable::Variable(char *val) {
    var_name = strdup(val);
}

Variable::~Variable() {
    free(var_name);
}

Number::Number(double val) {
    num_val = val;
}

Operator_un::Operator_un(int val, Expression *arg) {
    assert(val >= SIN && val <= UN_MINUS);
    assert(arg != nullptr);
    type_un_op = val;
    arg_f = arg;
}

Operator_bin::Operator_bin(int val, Expression *arg_l, Expression *arg_r) {
    assert(val >= ADD && val <= DIV);
    assert(arg_l != nullptr && arg_r != nullptr);
    type_bin_op = val;
    arg_f = arg_l;
    arg_s = arg_r;
}

void print_JPEG(Expression *diff) {
    FILE *code = fopen("diff.dot", "w");
    fprintf(code, "digraph G{\n");
    diff->print_dot(code);
    fprintf(code, "}");
    fclose(code);
    system("dot -Tjpeg diff.dot -o diff.jpeg");
}


void Operator_un::print_dot_name(FILE *code) {
    fprintf(code, "(");
    switch (type_un_op) {
        case SIN :
            fprintf(code, "sin");
            break;
        case COS :
            fprintf(code, "cos");
            break;
        case SQRT :
            fprintf(code, "sqrt");
            break;
        case UN_MINUS :
            fprintf(code, "minus");
            break;
    }
    fprintf(code, " ");
    arg_f->print_dot_name(code);
    fprintf(code, ")");
}


void Variable::print_dot_name(FILE *code) {
    fprintf(code, "%s", var_name);
}

void Operator_bin::print_dot_name(FILE *code) {
    arg_f->print_dot_name(code);
    fprintf(code, " ");
    switch (type_bin_op) {
        case ADD :
            fprintf(code, "+");
            break;
        case SUB :
            fprintf(code, "-");
            break;
        case MUL:
            fprintf(code, "*");
            break;
        case DIV :
            fprintf(code, "/");
            break;
    }


    fprintf(code, " ");
    arg_s->print_dot_name(code);

}

void Number::print_dot_name(FILE *code) {
    fprintf(code, "%g", num_val);
}


Expression *Operator_bin::optimize() {
    Expression *x = arg_f->optimize();
    Expression *y = arg_s->optimize();

    switch (type_bin_op) {
        case MUL:
            if (Number *x_0 = dynamic_cast<Number *>(x)) {
                if (x_0->num() == 0) return x_0;
                if (x_0->num() == 1) return y;
            }
            if (Number *y_0 = dynamic_cast<Number *>(y)) {
                if (y_0->num() == 0) return y_0;
                if (y_0->num() == 1) return y;
            }
            break;
        case ADD:
            if (Number *x_0 = dynamic_cast<Number *>(x)) {
                if (x_0->num() == 0) return y;
            }
            if (Number *y_0 = dynamic_cast<Number *>(y)) { ////class num()
                if (y_0->num() == 0) return x;
            }
            break;
    }
    return new Operator_bin(type_bin_op, x, y);
}

Expression *Operator_un::optimize() {
    Expression *x = arg_f->optimize();
    return new Operator_un(type_un_op, x);
}

double Number::num() {
    return num_val;
}


Expression *Expression::optimize() {
    return this;
}

Expression *Expression::derivative(char *x) {
    assert(0);
}

Expression *Number::derivative(char *number) {
    return new Number(0);
}

Expression *Variable::derivative(char *var) {
    if (!strcmp(var_name, var)) return new Number(1);
    return new Number(0);
}

Expression *Operator_un::derivative(char *var) {
    switch (type_un_op) {
        case UN_MINUS:
            return new Operator_un(UN_MINUS, arg_f->derivative(var));
        case SIN:
            return new Operator_bin(MUL,
                                    arg_f->derivative(var),
                                    new Operator_un(COS, arg_f));
        case COS:
            return new Operator_bin(MUL,
                                    new Operator_un(UN_MINUS, arg_f->derivative(var)),
                                    new Operator_un(SIN, arg_f));
        case SQRT:
            return new Operator_bin(DIV,
                                    arg_f->derivative(var),
                                    new Operator_bin(MUL, new Number(2), this));
    }
}


Expression *Operator_bin::derivative(char *var) {
    switch (type_bin_op) {
        case ADD:
            return new Operator_bin(ADD,
                                    arg_f->derivative(var),
                                    arg_s->derivative(var));
        case SUB:
            return new Operator_bin(SUB,
                                    arg_f->derivative(var),
                                    arg_s->derivative(var));
        case MUL:
            return new Operator_bin(ADD,
                                    new Operator_bin(MUL, arg_f->derivative(var), arg_s),
                                    new Operator_bin(MUL, arg_s->derivative(var), arg_f));
        case DIV:
            return new Operator_bin(SUB,
                                    new Operator_bin(DIV, arg_f->derivative(var), arg_s),
                                    new Operator_bin(MUL, this, new Operator_bin(DIV, arg_s->derivative(var), arg_s)));
    }

}
