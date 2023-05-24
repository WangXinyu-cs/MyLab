#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include "mylab.h"
#include "syntax.tab.h"
#include "mathfunc.h"
#include "arrayfunc.h"
#include "command.h"


using namespace std;

#define YYERROR_SIZE 100
char yyerror_text[YYERROR_SIZE];
map<string, Variable>   g_var;

void evaluate(Express *exp);
void eval_func(Express *);
void eval_binary(Express *);

void yyerror(std::vector<Statement*>* stmtlist, const char* msg)
{
    snprintf(yyerror_text, YYERROR_SIZE, "%s at \"%s\"", msg, yytext);
}

Express* compose_express(int op, Express* left, Express* right)
{
    Express *compose = new Express();
    compose->m_left = left;
    compose->m_right = right;
    switch (op)
    {
    case '+': compose->m_type = ET_PLUS; break;
    case '-': compose->m_type = ET_MINUS; break;
    case '*': compose->m_type = ET_MULTIPLY; break;
    case '/': compose->m_type = ET_RIGHT_DIVIDE; break;
    case '\\': compose->m_type = ET_LEFT_DIVIDE; break;
    case 'U': compose->m_type = ET_UMINUS; break;
    case '~': compose->m_type = ET_LOGINOT; break;
    case EQUAL: compose->m_type = ET_EQUAL; break;
    default: compose->m_type = ET_UNKNOWN;
    }
    return compose;
}

void eval_vector(Express *exp)
{
    Vector *v = NULL;
    Number_Type dt = Number_Type::nt_int64;
    vector<double> data;

    for (size_t i = 0; i < exp->m_explist->size(); ++i)
    {
        Express *e = (*exp->m_explist)[i];
        evaluate(e);
        if (e->is_integer())
            data.push_back(e->m_integer);
        else if (e->is_float())
        {
            data.push_back(e->m_float);
            dt = Number_Type::nt_double;
        }
        else
        {
            char buf[100];
            sprintf(buf, "the unit [%d] is invalid for Vector!\n", i);
            exp->set_unknown(buf);
            return;
        }
    }

    v = vector_new(exp->m_explist->size(), dt);
    vector_assign(v, data.begin(), data.end());
    exp->set_vector(v);
}

void eval_matrix(Express *exp)
{
    Matrix *m = NULL;
    size_t cols;
    Number_Type dt = Number_Type::nt_int64;
    vector<double> data;

    if (!exp->m_exparray->size() || !(cols = (*exp->m_exparray)[0]->size()))
    {
        m = matrix_new(0, 0);
        exp->set_matrix(m);
    }
    else
    {
        for (size_t i = 0; i < exp->m_exparray->size(); ++i)
        {
            vector<Express*> *explist = (*exp->m_exparray)[i];
            if (explist->size() != cols)
            {
                exp->set_unknown("different size of columns!\n");
                return;
            }
            for (size_t j = 0; j < explist->size(); ++j)
            {
                Express *e = (*explist)[j];
                evaluate(e);
                if (e->is_integer())
                    data.push_back(e->m_integer);
                else if (e->is_float())
                {
                    data.push_back(e->m_float);
                    dt = Number_Type::nt_double;
                }
                else
                {
                    char buf[100];
                    sprintf(buf, "the unit [%d %d] is invalid for Matrix!\n", i, j);
                    exp->set_unknown(buf);
                    return;
                }
            }
        }

        m = matrix_new(exp->m_exparray->size(), cols, dt);
        matrix_assign(m, data.begin(), data.end());
        exp->set_matrix(m);
    }
}

void eval_uminus(Express *exp)
{
    Express *left = exp->m_left;
    evaluate(left);

    if (left->is_integer())
        exp->set_integer(left->m_integer * -1);
    else if (left->is_float())
        exp->set_float(left->m_float * -1);
    else if (left->is_vector())
    {
        Vector *v = vector_scalar_multiply(left->m_vector.get(), -1);
        exp->set_vector(v);
    }
    else if (left->is_matrix())
    {
        Matrix *m = matrix_scalar_multiply(left->m_matrix.get(), -1);
        exp->set_matrix(m);
    }
    else
        exp->set_unknown("unknown expression type");
}

void eval_vector_diff(Express *exp)
{
    if (exp->m_explist->size() == 3)
    {
        Vector *v = NULL;
        Number_Type dt = Number_Type::nt_int64;
        double d[3];
        for (int i = 0; i < 3; ++i)
        {
            evaluate((*exp->m_explist)[i]);
            if ((*exp->m_explist)[i]->is_float())
            {
                dt = Number_Type::nt_double;
                d[i] = (*exp->m_explist)[i]->m_float;
            }
            else if ((*exp->m_explist)[i]->is_integer())
                d[i] = (*exp->m_explist)[i]->m_integer;
            else
                goto wrong_diff;
        }
        if (dt == Number_Type::nt_int64)
            v = vector_diff<int64_t>(d[0], d[1], d[2], Number_Type::nt_int64);
        else
            v = vector_diff<double>(d[0], d[1], d[2], Number_Type::nt_double);

        if (v)
        {
            exp->set_vector(v);
            return;
        }
    }
wrong_diff:
    exp->set_unknown("invalid arithmetic progression Vector");
}

void evaluate(Express *exp)
{
    if (!exp || exp->is_unknown() || exp->is_integer()
     || exp->is_float() || exp->is_string())
        return;

    if (exp->is_vector())
        eval_vector(exp);
    else if (exp->is_matrix())
        eval_matrix(exp);
    else if (exp->is_vector_diff())
        eval_vector_diff(exp);
    else if (exp->is_variable())
    {
        Variable *v = search_var(exp->m_varname);
        if (v)
            exp->set_variable(v);
        else
            exp->set_unknown(string("no such variable: ") + exp->m_varname);
    }
    else if (exp->is_uminus())
        eval_uminus(exp);
    else if (exp->is_function())
        eval_func(exp);
    else if (exp->is_loginot())
        evaluate(exp->m_left);
    else if (exp->is_equal())
    {
        evaluate(exp->m_left);
        evaluate(exp->m_right);
    }
    else
        eval_binary(exp);
}

void eval_binary(Express *exp)
{
    Express *left = exp->m_left;
    Express *right = exp->m_right;

    evaluate(left);
    evaluate(right);

    if (left->is_unknown() || right->is_unknown())
        exp->set_unknown("[left]: " + left->m_why + " [right]: " + right->m_why);
    else if (left->is_integer())
    {
        if (right->is_integer())
        {
            if (exp->is_plus())
                exp->set_integer(left->m_integer + right->m_integer);
            else if (exp->is_minus())
                exp->set_integer(left->m_integer - right->m_integer);
            else if (exp->is_multiply())
                exp->set_integer(left->m_integer * right->m_integer);
            else if (exp->is_r_divide() || exp->is_l_divide())
                if (right->m_integer != 0)
                    exp->set_integer(1.0 * left->m_integer / right->m_integer);
                else
                {
                    printf("warning: division by zero\n");
                    exp->set_float(1.0 * left->m_integer / right->m_integer);
                }
        }
        else if (right->is_float())
        {
            if (exp->is_plus())
                exp->set_float(left->m_integer + right->m_float);
            else if (exp->is_minus())
                exp->set_float(left->m_integer - right->m_float);
            else if (exp->is_multiply())
                exp->set_float(left->m_integer * right->m_float);
            else if (exp->is_r_divide() || exp->is_l_divide())
            {
                if (right->m_float == 0.0)
                    printf("warning: division by zero\n");
                exp->set_float(left->m_integer / right->m_float);
            }
        }
        else if (right->is_string())
            exp->set_unknown("number op string is not supported.");
        else if (right->is_vector())
            exp->set_unknown("scalar op vector is not supported.");
        else if (right->is_matrix())
            exp->set_unknown("scalar op matrix is not supported.");
    }
    else if (left->is_float())
    {
        if (right->is_integer())
        {
            if (exp->is_plus())
                exp->set_float(left->m_float + right->m_integer);
            else if (exp->is_minus())
                exp->set_float(left->m_float - right->m_integer);
            else if (exp->is_multiply())
                exp->set_float(left->m_float * right->m_integer);
            else if (exp->is_r_divide() || exp->is_l_divide())
            {
                if (right->m_integer == 0)
                    printf("warning: division by zero\n");
                exp->set_float(left->m_float / right->m_integer);
            }
        }
        else if (right->is_float())
        {
            if (exp->is_plus())
                exp->set_float(left->m_float + right->m_float);
            else if (exp->is_minus())
                exp->set_float(left->m_float - right->m_float);
            else if (exp->is_multiply())
                exp->set_float(left->m_float * right->m_float);
            else if (exp->is_r_divide() || exp->is_l_divide())
            {
                if (right->m_float == 0.0)
                    printf("warning: division by zero\n");
                exp->set_float(left->m_float / right->m_float);
            }
        }
        else if (right->is_string())
            exp->set_unknown("number op string is not supported.");
        else if (right->is_vector())
            exp->set_unknown("scalar op vector is not supported.");
        else if (right->is_matrix())
            exp->set_unknown("scalar op matrix is not supported.");
    }
    else if (left->is_string())
    {
        if (right->is_string() && exp->is_plus())
            exp->m_string = left->m_string + right->m_string;
        else
            exp->set_unknown("string operation is invalid.");
    }
    else if (left->is_vector())
    {
        if (right->is_integer())
        {
            if (exp->is_multiply())
                exp->set_vector(vector_scalar_multiply(left->m_vector.get(), right->m_integer));
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_vector(vector_scalar_divide(left->m_vector.get(), right->m_integer));
            else
                exp->set_unknown("vector operation is invalid.");
        }
        else if (right->is_float())
        {
            if (exp->is_multiply())
                exp->set_vector(vector_scalar_multiply(left->m_vector.get(), right->m_float));
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_vector(vector_scalar_divide(left->m_vector.get(), right->m_float));
            else
                exp->set_unknown("vector operation is invalid.");
        }
        else if (right->is_string())
            exp->set_unknown("vector op string is not supported.");
        else if (right->is_vector())
        {
            if (exp->is_plus())
                exp->set_vector(vector_plus(left->m_vector.get(), right->m_vector.get()));
            else if (exp->is_minus())
                exp->set_vector(vector_minus(left->m_vector.get(), right->m_vector.get()));
            else
                exp->set_unknown("vector operation is invalid.");
        }
        else if (right->is_matrix())
            exp->set_unknown("vector op matrix is invalid.");
    }
    else if (left->is_matrix())
    {
        if (right->is_integer())
        {
            if (exp->is_plus())
                exp->set_matrix(matrix_scalar_plus(left->m_matrix.get(), right->m_integer));
            else if (exp->is_minus())
                exp->set_matrix(matrix_scalar_minus(left->m_matrix.get(), right->m_integer));
            else if (exp->is_multiply())
                exp->set_matrix(matrix_scalar_multiply(left->m_matrix.get(), right->m_integer));
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_matrix(matrix_scalar_divide(left->m_matrix.get(), right->m_integer));   // divide by 0 ? change to float?
        }
        else if (right->is_float())
        {
            if (exp->is_plus())
                exp->set_matrix(matrix_scalar_plus(left->m_matrix.get(), right->m_float));
            else if (exp->is_minus())
                exp->set_matrix(matrix_scalar_minus(left->m_matrix.get(), right->m_float));
            else if (exp->is_multiply())
                exp->set_matrix(matrix_scalar_multiply(left->m_matrix.get(), right->m_float));
            else if (exp->is_r_divide() || exp->is_l_divide())
                exp->set_matrix(matrix_scalar_divide(left->m_matrix.get(), right->m_float));   // divide by 0 ? change to float?
        }
        else if (right->is_string())
            exp->set_unknown("matrix op string is not supported.");
        else if (right->is_matrix())
        {
            if (exp->is_plus())
                exp->set_matrix(matrix_plus(left->m_matrix.get(), right->m_matrix.get()));
            else if (exp->is_minus())
                exp->set_matrix(matrix_minus(left->m_matrix.get(), right->m_matrix.get()));
            else if (exp->is_multiply())
                exp->set_matrix(matrix_multiply(left->m_matrix.get(), right->m_matrix.get()));
            else if (exp->is_r_divide())
                exp->set_matrix(matrix_right_divide(left->m_matrix.get(), right->m_matrix.get()));
            else if (exp->is_l_divide())
                exp->set_matrix(matrix_left_divide(left->m_matrix.get(), right->m_matrix.get()));
        }
    }
}

void eval_func(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    const char* fn = exp->m_funcname;
    bool ret;

    for (size_t i = 0; i < num; ++i)
    {
        evaluate(pm[i]);
        if (pm[i]->is_unknown())
        {
            char buf[100];
            sprintf(buf, "parameter %d is invalid.", i + 1);
            exp->set_unknown(buf);
            return;
        }
    }

    if (!strcmp(fn, "zeros") || !strcmp(fn, "ones") || !strcmp(fn, "eye")
        || !strcmp(fn, "rand"))
        ret = func_init_matrix(exp);
    else if (!strcmp(fn, "diag"))
        ret = func_diag(exp);
    else if (!strcmp(fn, "blkdiag"))
        ret = func_blkdiag(exp);
    else if (!strcmp(fn, "cat"))
        ret = func_cat(exp);
    else if (!strcmp(fn, "transpose"))
        ret = func_transpose(exp);
    else if (!strcmp(fn, "inv"))
        ret = func_inv(exp);
    else if (!strcmp(fn, "det"))
        ret = func_det(exp);
    else if (!strcmp(fn, "magic"))
        ret = func_magic(exp);
    else if (!strcmp(fn, "find"))
        ret = func_find(exp);
    else if (!strcmp(fn, "length"))
        ret = func_length(exp);
    else if (!strcmp(fn, "linspace"))
        ret = func_linspace(exp);
    else if (!strcmp(fn, "logspace"))
        ret = func_logspace(exp);
    else if (!strcmp(fn, "max"))
        ret = func_max(exp);
    else if (!strcmp(fn, "min"))
        ret = func_min(exp);
    else if (!strcmp(fn, "prod"))
        ret = func_prod(exp);
    else if (!strcmp(fn, "size"))
        ret = func_size(exp);
    else if (!strcmp(fn, "numel"))
        ret = func_numel(exp);
    else if (!strcmp(fn, "reshape"))
        ret = func_reshape(exp);
    else if (!strcmp(fn, "sort"))
        ret = func_sort(exp);
    else if (!strcmp(fn, "sum"))
        ret = func_sum(exp);
    else if (!strcmp(fn, "dot"))
        ret = func_dot(exp);
    else if (!strcmp(fn, "abs"))
        ret = math_abs(exp);
    else if (!strcmp(fn, "sqrt"))
        ret = math_sqrt(exp);
    else if (!strcmp(fn, "pow"))
        ret = math_pow(exp);
    else if (!strcmp(fn, "exp"))
        ret = math_exp(exp);
    else if (!strcmp(fn, "ln"))
        ret = math_ln(exp);     // natural logarithm of x
    else if (!strcmp(fn, "lg"))
        ret = math_lg(exp);     // base 10 logarithm of x
    else if (!strcmp(fn, "log"))
        ret = math_log(exp);    // base x logarithm of y
    else if (!strcmp(fn, "sin"))
        ret = math_sin(exp);
    else if (!strcmp(fn, "cos"))
        ret = math_cos(exp);
    else if (!strcmp(fn, "tan"))
        ret = math_tan(exp);
    else
    {
        exp->set_unknown(string(fn) + " is unknown function.");
        return;
    }

    if (!ret)
        exp->set_unknown(string(fn) + " takes invalid parameters.");
}

Variable* search_var(const char* vn)
{
    map<string, Variable>::iterator ite = g_var.find(vn);
    if (ite != g_var.end())
        return &(ite->second);
    else
        return NULL;
}
	
void set_var(const char* vn, const Variable &var)
{
    g_var[vn] = var;
}

const char *PI = "pi";
const char *EULER = "euler";
const char *ANS = "ans";

void create_native_varialbe()
{
    g_var[PI] = Variable(3.1415926, true);
    g_var[EULER] = Variable(2.718281, true);
    g_var[ANS] = Variable();
}

void assign(Statement *stmt)
{
    evaluate(stmt->m_express);
    if (stmt->m_express->is_unknown())
        printf("fail to assign: %s\n", stmt->m_express->m_why.c_str());
    else
        set_var(stmt->m_leftvar, Variable(stmt->m_express));
}

void mylab_loop()
{
    vector<Statement*> stmtlist;
    while (1)
    {
        printf(">");
        yyparse(&stmtlist);
        for (int i = 0; i < stmtlist.size(); ++i)
        {
            Statement* stmt = stmtlist[i];
            switch (stmt->m_type)
            {
            case Stmt_Type::ST_ERROR:
                printf("ERROR: %s\n", stmt->m_error.c_str()); break;
            case Stmt_Type::ST_EXPRESS:
                evaluate(stmt->m_express);
                if (!stmt->m_express->m_varname)
                {
                    set_var(ANS, Variable(stmt->m_express));
                    printf("ans = ");
                }
                stmt->m_express->print_after_eval();
                break;
            case Stmt_Type::ST_ASSIGN:
                assign(stmt);
                break;
            case Stmt_Type::ST_COMMAND:
                command(stmt); break;
            case Stmt_Type::ST_EOF:
                printf("good-bye\n");
                exit(0);
            default:
                printf("What's the statement type %d ?\n", stmt->m_type);
            }
            delete stmt;
            stmtlist[i] = NULL;
        }
        stmtlist.clear();
    }
}
