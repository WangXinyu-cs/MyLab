#include <cmath>
#include "dstruct.h"

using namespace std;

typedef long (*MATH_L_L)(long);
typedef double (*MATH_D_D)(double);
typedef double (*MATH_D_D_D)(double, double);

bool math_d_d(Express *exp, MATH_D_D func)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    bool ret = false;

    if (num == 1)
    {
        if (pm[0]->is_integer())
        {
            exp->set_float(func(pm[0]->m_integer));
            ret = true;
        }
        else if (pm[0]->is_float())
        {
            exp->set_float(func(pm[0]->m_float));
            ret = true;
        }
    }
    return ret;
}

bool math_d_d_d(Express *exp, MATH_D_D_D func)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    bool ret = false;

    if (num == 2)
    {
        if (pm[0]->is_integer())
        {
            if (pm[1]->is_integer())
            {
                exp->set_float(func(pm[0]->m_integer, pm[1]->m_integer));
                ret = true;
            }
            else if (pm[1]->is_float())
            {
                exp->set_float(func(pm[0]->m_integer, pm[1]->m_float));
                ret = true;
            }
        }
        else if (pm[0]->is_float())
        {
            if (pm[1]->is_integer())
            {
                exp->set_float(func(pm[0]->m_float, pm[1]->m_integer));
                ret = true;
            }
            else if (pm[1]->is_float())
            {
                exp->set_float(func(pm[0]->m_float, pm[1]->m_float));
                ret = true;
            }
        }
    }
    return ret;
}

bool math_abs(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    bool ret = false;

    if (num == 1)
    {
        if (pm[0]->is_integer())
        {
            exp->set_integer(labs(pm[0]->m_integer));
            ret = true;
        }
        else if (pm[0]->is_float())
        {
            exp->set_float(fabs(pm[0]->m_float));
            ret = true;
        }
    }
    return ret;
}

bool math_sqrt(Express *exp)
{
    return math_d_d(exp, sqrt);
}

bool math_pow(Express *exp)
{
    return math_d_d_d(exp, pow);
}

bool math_exp(Express *expr)
{
    return math_d_d(expr, exp);
}

bool math_ln(Express *exp)
{
    return math_d_d(exp, log);
}

bool math_lg(Express *exp)
{
    return math_d_d(exp, log10);
}

bool math_log(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    bool ret = false;

    if (num == 2)
    {
        if (pm[0]->is_integer())
        {
            if (pm[1]->is_integer())
            {
                exp->set_float(log(pm[1]->m_integer) / log(pm[0]->m_integer));
                ret = true;
            }
            else if (pm[1]->is_float())
            {
                exp->set_float(log(pm[1]->m_float) / log(pm[0]->m_integer));
                ret = true;
            }
        }
        else if (pm[0]->is_float())
        {
            if (pm[1]->is_integer())
            {
                exp->set_float(log(pm[1]->m_integer) / log(pm[0]->m_float));
                ret = true;
            }
            else if (pm[1]->is_float())
            {
                exp->set_float(log(pm[1]->m_float) / log(pm[0]->m_float));
                ret = true;
            }
        }
    }
    return ret;
}

bool math_sin(Express *exp)
{
    return math_d_d(exp, sin);
}

bool math_cos(Express *exp)
{
    return math_d_d(exp, cos);
}

bool math_tan(Express *exp)
{
    return math_d_d(exp, tan);
}
