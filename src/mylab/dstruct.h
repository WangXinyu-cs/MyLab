#ifndef _DSTRUCT_H_
#define _DSTRUCT_H_

#include <string>
#include <vector>
#include <memory>
#include "matrix.h"

struct Variable;

enum Express_Type
{
    ET_UNKNOWN,
    ET_CONST_FLOAT,
    ET_INTEGER,
    ET_FLOAT,
    ET_STRING,
    ET_VECTOR,
    ET_VECTOR_DIFF,
    ET_MATRIX,
    ET_FUNCTION,
    ET_VARIABLE,
    ET_UMINUS,  // unary minus
    ET_PLUS,
    ET_MINUS,
    ET_MULTIPLY,
    ET_RIGHT_DIVIDE,
    ET_LEFT_DIVIDE,
    ET_LOGINOT,
    ET_EQUAL
};

struct Express
{
    // storage after evaluation
    enum Express_Type  m_type;
    std::string     m_why;
    long    m_integer;
    double  m_float;
    char    *m_funcname;
    char    *m_varname;
    std::string m_string;    
    std::shared_ptr<Vector> m_vector;
    std::shared_ptr<Matrix> m_matrix;

    // storage before evaluation
    std::vector<Express*>   *m_explist;
    std::vector<std::vector<Express*>*> *m_exparray;
    Express *m_left, *m_right;  // duality operation

    Express();
    virtual ~Express();

    inline bool is_unknown()  { return m_type == Express_Type::ET_UNKNOWN; }
    inline bool is_integer()  { return m_type == Express_Type::ET_INTEGER; }
    inline bool is_float()    { return m_type == Express_Type::ET_FLOAT || m_type == Express_Type::ET_CONST_FLOAT; }
    inline bool is_string()   { return m_type == Express_Type::ET_STRING; }
    inline bool is_vector()   { return m_type == Express_Type::ET_VECTOR; }
    inline bool is_matrix()   { return m_type == Express_Type::ET_MATRIX; }
    inline bool is_function() { return m_type == Express_Type::ET_FUNCTION; }
    inline bool is_variable() { return m_type == Express_Type::ET_VARIABLE; }
    inline bool is_uminus()   { return m_type == Express_Type::ET_UMINUS; }
    inline bool is_plus()     { return m_type == Express_Type::ET_PLUS; }
    inline bool is_minus()    { return m_type == Express_Type::ET_MINUS; }
    inline bool is_multiply() { return m_type == Express_Type::ET_MULTIPLY; }
    inline bool is_r_divide() { return m_type == Express_Type::ET_RIGHT_DIVIDE; }
    inline bool is_l_divide() { return m_type == Express_Type::ET_LEFT_DIVIDE; }
    inline bool is_vector_diff() { return m_type == Express_Type::ET_VECTOR_DIFF; }
    inline bool is_loginot()  { return m_type == Express_Type::ET_LOGINOT; }
    inline bool is_equal()  { return m_type == Express_Type::ET_EQUAL; }

    void set_unknown(const std::string&);
    void set_integer(long);
    void set_float(double);
    void set_string(char*);
    void set_vector(Vector*);
    void set_vector(std::shared_ptr<Vector> &);
    void set_matrix(Matrix*);
    void set_matrix(std::shared_ptr<Matrix> &);
    void set_variable(Variable*);
    void set_varname(char*);

    void print_after_eval();    // print after it is evaluated
};

enum Stmt_Type
{
    ST_UNKNOWN,
    ST_EOF,
    ST_ERROR,
    ST_EXPRESS,
    ST_ASSIGN,
    ST_COMMAND
    // if
    // while
    // etc...
};

struct Statement
{
    enum Stmt_Type  m_type;
    std::string     m_error;
    char    *m_leftvar;
    char    *m_command;
    Express *m_express;
    std::vector<std::string> *m_namelist;

    Statement();
    virtual ~Statement();
};

struct Variable
{
    Express_Type m_type;
    long    m_integer;
    double  m_float;
    std::string m_string;
    std::shared_ptr<Vector> m_vector;
    std::shared_ptr<Matrix> m_matrix;

    Variable();
    explicit Variable(Express*);
    explicit Variable(Vector*);
    explicit Variable(Matrix*);
    Variable(double, bool = false);
    Variable(const Variable &other);

    Variable& operator=(const Variable &other);
    const char* category() const;
    const char* type() const;
};

void delete_explist(std::vector<Express*>*);
void delete_mtxrows(std::vector<std::vector<Express*>*>*);

#endif
