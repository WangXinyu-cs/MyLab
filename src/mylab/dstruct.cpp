#include "dstruct.h"


using namespace std;

void delete_explist(vector<Express*>* explist)
{
    for (size_t i = 0; i < explist->size(); ++i)
        delete (*explist)[i];
    delete explist;
}

void delete_mtxrows(vector<vector<Express*>*> *rows)
{
    for (size_t i = 0; i < rows->size(); ++i)
    {
        vector<Express*> *explist = (*rows)[i];
        for (size_t j = 0; j < explist->size(); ++j)
            delete (*explist)[j];
        delete explist;
    }
    delete rows;
}

Express::Express()
{
    m_type = Express_Type::ET_UNKNOWN;
    m_varname = m_funcname = NULL;
    m_left = m_right = NULL;
    m_explist = NULL;
    m_exparray = NULL;
    m_why = "nil";
}

Express::~Express()
{
    if (m_varname)
        free(m_varname);

    if (m_funcname)
        free(m_funcname);

    if (m_left)
        delete m_left;

    if (m_right)
        delete m_right;

    if (m_explist)
        delete_explist(m_explist);

    if (m_exparray)
        delete_mtxrows(m_exparray);
}

void Express::set_unknown(const string &why)
{
    m_type = Express_Type::ET_UNKNOWN;
    m_why = why;
}

void Express::set_integer(long v)
{
    m_type = Express_Type::ET_INTEGER;
    m_integer = v;
}

void Express::set_float(double v)
{
    m_type = Express_Type::ET_FLOAT;
    m_float = v;
}

void Express::set_string(char *s)
{
    m_type = Express_Type::ET_STRING;
    m_string = s;
}

void Express::set_vector(Vector *v)
{
    m_type = Express_Type::ET_VECTOR;
    m_vector.reset(v);  // own bare pointer
}

void Express::set_vector(std::shared_ptr<Vector> &v)
{
    m_type = Express_Type::ET_VECTOR;
    m_vector = v;
}

void Express::set_matrix(Matrix *m)
{
    m_type = Express_Type::ET_MATRIX;
    m_matrix.reset(m);  // own bare pointer
}

void Express::set_matrix(std::shared_ptr<Matrix> &m)
{
    m_type = Express_Type::ET_MATRIX;
    m_matrix = m;
}

void Express::set_variable(Variable* v)
{
    m_type = v->m_type;
    if (is_integer())
        m_integer = v->m_integer;
    else if (is_float())
        m_float = v->m_float;
    else if (is_string())
        m_string = v->m_string;
    else if (is_vector())
        m_vector = v->m_vector;
    else if (is_matrix())
        m_matrix = v->m_matrix;
    else
        m_why = "unknown";
}

void Express::set_varname(char* var)
{
    free(m_varname);
    m_varname = strdup(var);
}

void Express::print_after_eval()
{
    if (is_integer())
        printf("%ld\n", m_integer);
    else if (is_float())
        printf("%lf\n", m_float);
    else if (is_string())
        printf("%s\n", m_string.c_str());
    else if (is_vector())
        vector_print(m_vector.get());
    else if (is_matrix())
        matrix_print(m_matrix.get());
    else if (!m_why.empty())
        printf("%s\n", m_why.c_str());
}

Statement::Statement()
{
    m_type = Stmt_Type::ST_UNKNOWN;
    m_leftvar = NULL;
    m_express = NULL;
    m_command = NULL;
    m_namelist = NULL;
}

Statement::~Statement()
{
    if (m_leftvar)
        free(m_leftvar);

    if (m_command)
        free(m_command);

    if (m_express)
        delete m_express;

    if (m_namelist)
        delete m_namelist;
}

Variable::Variable()
{
    m_type = Express_Type::ET_UNKNOWN;
}

Variable::Variable(Express *exp)
{
    if (!exp)
        m_type = Express_Type::ET_UNKNOWN;

    m_type = exp->m_type;
    if (exp->is_integer())
        m_integer = exp->m_integer;
    else if (exp->is_float())
        m_float = exp->m_float;
    else if (exp->is_string())
        m_string = exp->m_string;
    else if (exp->is_vector())
        m_vector = exp->m_vector;
    else if (exp->is_matrix())
        m_matrix = exp->m_matrix;
}

Variable::Variable(Vector *v) : m_vector(v)
{
    if (v)
        m_type = Express_Type::ET_VECTOR;
    else
        m_type = Express_Type::ET_UNKNOWN;
}

Variable::Variable(Matrix *m) : m_matrix(m)
{
    if (m)
        m_type = Express_Type::ET_MATRIX;
    else
        m_type = Express_Type::ET_UNKNOWN;
}

Variable::Variable(double d, bool constant) : m_float(d)
{
    constant ? m_type = ET_CONST_FLOAT : m_type = ET_FLOAT;
}

Variable::Variable(const Variable &other) : m_string(other.m_string),
    m_vector(other.m_vector), m_matrix(other.m_matrix)
{
    m_type = other.m_type;
    m_integer = other.m_integer;
    m_float = other.m_float;
}

Variable& Variable::operator=(const Variable &other)
{
    if (m_type == ET_CONST_FLOAT)
    {
        printf("warning: cann't change constant.\n");
        return *this;
    }

    m_type = other.m_type;
    if (m_type == Express_Type::ET_INTEGER)
        m_integer = other.m_integer;
    else if (m_type == Express_Type::ET_FLOAT || m_type == Express_Type::ET_CONST_FLOAT)
        m_float = other.m_float;
    else if (m_type == Express_Type::ET_STRING)
        m_string = other.m_string;
    else if (m_type == Express_Type::ET_VECTOR)
        m_vector = other.m_vector;
    else if (m_type == Express_Type::ET_MATRIX)
        m_matrix = other.m_matrix;
    return *this;
}

const char* Variable::category() const
{
    switch (m_type)
    {
    case ET_CONST_FLOAT:
    case ET_FLOAT: return "scalar";
    case ET_INTEGER: return "scalar";
    case ET_STRING: return "text";
    case ET_VECTOR: return "vector";
    case ET_MATRIX: return "matrix";
    default: return "unknown";
    }
}

const char* Variable::type() const
{
    switch (m_type)
    {
    case ET_CONST_FLOAT:
    case ET_FLOAT: return "double";
    case ET_INTEGER: return "integer";
    case ET_STRING: return "string";
    case ET_VECTOR: return nt_to_name(m_vector->get_type());
    case ET_MATRIX: return nt_to_name(m_matrix->get_type());
    default: return "unknown";
    }
}