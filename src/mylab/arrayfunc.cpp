#include "dstruct.h"
#include <cmath>

using namespace std;

Matrix* matrix_api(const char* fn, long p1)
{
    if (!strcmp(fn, "zeros"))
        return matrix_zeros(p1);
    else if (!strcmp(fn, "ones"))
        return matrix_ones(p1);
    else if (!strcmp(fn, "eye"))
        return matrix_eye(p1);
    else if (!strcmp(fn, "rand"))
        return matrix_rand(p1);
    else
        return NULL;
}

Matrix* matrix_api(const char* fn, long p1, long p2)
{
    if (!strcmp(fn, "zeros"))
        return matrix_zeros(p1, p2);
    else if (!strcmp(fn, "ones"))
        return matrix_ones(p1, p2);
    else if (!strcmp(fn, "eye"))
        return matrix_eye(p1, p2);
    else if (!strcmp(fn, "rand"))
        return matrix_rand(p1, p2);
    else
        return NULL;
}

Matrix* matrix_api(const char* fn, long p1, Number_Type p2)
{
    if (!strcmp(fn, "zeros"))
        return matrix_zeros(p1, p2);
    else if (!strcmp(fn, "ones"))
        return matrix_ones(p1, p2);
    else if (!strcmp(fn, "eye"))
        return matrix_eye(p1, p2);
    else if (!strcmp(fn, "rand"))
        return matrix_rand(p1, p2);
    else
        return NULL;
}

Matrix* matrix_api(const char* fn, long p1, long p2, Number_Type p3)
{
    if (!strcmp(fn, "zeros"))
        return matrix_zeros(p1, p2, p3);
    else if (!strcmp(fn, "ones"))
        return matrix_ones(p1, p2, p3);
    else if (!strcmp(fn, "eye"))
        return matrix_eye(p1, p2, p3);
    else if (!strcmp(fn, "rand"))
        return matrix_rand(p1, p2, p3);
    else
        return NULL;
}

bool func_init_matrix(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    const char* fn = exp->m_funcname;
    Number_Type dt;

    if (num == 1)
    {
        if (pm[0]->is_integer() && pm[0]->m_integer >= 0)
        {
            exp->set_matrix(matrix_api(fn, pm[0]->m_integer));
            return true;
        }
    }
    else if (num == 2)
    {
        if (pm[0]->is_integer() && pm[0]->m_integer >= 0)
        {
            if (pm[1]->is_integer() && pm[1]->m_integer >= 0)
            {
                exp->set_matrix(matrix_api(fn, pm[0]->m_integer, pm[1]->m_integer));
                return true;
            }
            else if (pm[1]->is_string() && (dt = name_to_nt(pm[1]->m_string.c_str())))
            {
                exp->set_matrix(matrix_api(fn, pm[0]->m_integer, dt));
                return true;
            }
        }
    }
    else if (num == 3)
    {
        if (pm[0]->is_integer() && pm[0]->m_integer >= 0
            && pm[1]->is_integer() && pm[1]->m_integer >= 0
            && pm[2]->is_string() && (dt = name_to_nt(pm[2]->m_string.c_str())))
        {
            exp->set_matrix(matrix_api(fn, pm[0]->m_integer, pm[1]->m_integer, dt));
            return true;
        }
    }
    return false;
}

bool func_diag(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    bool ret = false;

    if (num == 1)
    {
        if (pm[0]->is_vector())
        {
            exp->set_matrix(matrix_diag(pm[0]->m_vector.get()));
            ret = true;
        }
        else if (pm[0]->is_matrix())
        {
            exp->set_vector(matrix_diag(pm[0]->m_matrix.get()));
            ret = true;
        }
    }
    else if (num == 2)
    {
        if (pm[0]->is_vector() && pm[1]->is_integer())
        {
            exp->set_matrix(matrix_diag(pm[0]->m_vector.get(), pm[1]->m_integer));
            ret = true;
        }
        else if (pm[0]->is_matrix() && pm[1]->is_integer())
        {
            exp->set_vector(matrix_diag(pm[0]->m_matrix.get(), pm[1]->m_integer));
            ret = true;
        }
    }
    return ret;
}

bool func_blkdiag(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    Number_Type dt;

    vector<Matrix*> mx;
    for (size_t i = 0; i < num - 1; ++i)
    {
        if (pm[i]->is_matrix())
            mx.push_back(pm[i]->m_matrix.get());
        else
            return false;
    }

    if (pm[num - 1]->is_matrix())
    {
        mx.push_back(pm[num - 1]->m_matrix.get());
        exp->set_matrix(matrix_blkdiag(mx.begin(), mx.end()));
        return true;
    }
    else if (pm[num - 1]->is_string() && (dt = name_to_nt(pm[num - 1]->m_string.c_str())))
    {
        exp->set_matrix(matrix_blkdiag(mx.begin(), mx.end(), dt));
        return true;
    }
    return false;
}

bool func_cat(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    Number_Type dt;

    vector<Matrix*> mx;
    if (num >= 2 && pm[0]->is_integer() && pm[0]->m_integer > 0)
    {
        for(size_t i = 1; i < num - 1; ++i)
        {
            if (pm[i]->is_matrix())
                mx.push_back(pm[i]->m_matrix.get());
            else
                return false;
        }

        if (pm[num - 1]->is_matrix())
        {
            mx.push_back(pm[num - 1]->m_matrix.get());
            exp->set_matrix(matrix_cat(pm[0]->m_integer, mx.begin(), mx.end()));
            return true;
        }
        else if (pm[num - 1]->is_string() && (dt = name_to_nt(pm[num - 1]->m_string.c_str())))
        {
            exp->set_matrix(matrix_cat(pm[0]->m_integer, mx.begin(), mx.end(), dt));
            return true;
        }
    }
    return false;
}

bool func_transpose(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1 && pm[0]->is_matrix())
    {
        exp->set_matrix(matrix_transpose(pm[0]->m_matrix.get()));
        return true;
    }
    return false;
}

bool func_inv(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1 && pm[0]->is_matrix())
    {
        exp->set_matrix(matrix_inverse(pm[0]->m_matrix.get()));
        return true;
    }
    return false;
}

bool func_det(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1 && pm[0]->is_matrix())
    {
        exp->set_float(matrix_determinant(pm[0]->m_matrix.get()));
        return true;
    }
    return false;
}

bool func_magic(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1 && pm[0]->is_integer() && pm[0]->m_integer > 0)
    {
        exp->set_matrix(matrix_magic(pm[0]->m_integer));
        return true;
    }
    return false;
}

bool func_find(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1)
    {
        if (pm[0]->is_matrix())
        {
            exp->set_vector(matrix_find_not_equal(pm[0]->m_matrix.get(), 0));
            return true;
        }

        if (pm[0]->is_loginot())
        {
            if (pm[0]->m_left->is_matrix())
            {
                exp->set_vector(matrix_find_equal(pm[0]->m_left->m_matrix.get(), 0));
                return true;
            }
        }

        if (pm[0]->is_equal() && pm[0]->m_left->is_matrix())
        {
            if (pm[0]->m_right->is_integer())
            {
                exp->set_vector(matrix_find_equal(pm[0]->m_left->m_matrix.get(), pm[0]->m_right->m_integer));
                return true;
            }
            else if (pm[0]->m_right->is_float())
            {
                exp->set_vector(matrix_find_equal(pm[0]->m_left->m_matrix.get(), pm[0]->m_right->m_float));
                return true;
            }
        }
    }

    return false;
}

bool func_length(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1)
    {
        if (pm[0]->is_vector())
        {
            exp->set_integer(pm[0]->m_vector->get_size());
            return true;
        }
        else if (pm[0]->is_matrix())
        {
            Matrix_Dim md = pm[0]->m_matrix->get_size();
            exp->set_integer(md.row >= md.col ? md.row : md.col);
            return true;
        }
    }

    return false;
}

bool func_linspace(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();
    double a1, an;
    int n;

    if (num == 2 || num == 3)
    {
        if (pm[0]->is_integer())
            a1 = pm[0]->m_integer;
        else if (pm[0]->is_float())
            a1 = pm[0]->m_float;
        else
            goto wrong_exit;

        if (pm[1]->is_integer())
            an = pm[1]->m_integer;
        else if (pm[1]->is_float())
            an = pm[1]->m_float;
        else
            goto wrong_exit;

        if (num == 2)
            n = 100;
        else
        {
            if (pm[2]->is_integer() && pm[2]->m_integer > 1)
                n = pm[2]->m_integer;
            else
                goto wrong_exit;
        }

        Vector *v = vector_new(n);
        for (int i = 0; i < n; ++i)
            v->set_unit(i, a1 + i * (an - a1) / (n - 1));
        exp->set_vector(v);
        return true;
    }
wrong_exit:
    return false;
}

bool func_logspace(Express *exp)
{
    if (func_linspace(exp))
    {
        for (int i = 0; i < exp->m_vector->get_size(); ++i)
            exp->m_vector->set_unit(i, pow(10, exp->m_vector->get_unit(i)));
        return true;
    }
    return false;
}

bool func_max(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1)
    {
        if (pm[0]->is_vector())
        {
            double v = pm[0]->m_vector->get_max();
            if (is_integer(pm[0]->m_vector->get_type()))
                exp->set_integer(v);
            else
                exp->set_float(v);
            return true;
        }
        else if (pm[0]->is_matrix())
        {
            exp->set_vector(pm[0]->m_matrix->get_max());
            return true;
        }
    }
    return false;
}

bool func_min(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1)
    {
        if (pm[0]->is_vector())
        {
            double v = pm[0]->m_vector->get_min();
            if (is_integer(pm[0]->m_vector->get_type()))
                exp->set_integer(v);
            else
                exp->set_float(v);
            return true;
        }
        else if (pm[0]->is_matrix())
        {
            exp->set_vector(pm[0]->m_matrix->get_min());
            return true;
        }
    }
    return false;
}

bool func_prod(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1)
    {
        if (pm[0]->is_vector())
        {
            double v = pm[0]->m_vector->get_product();
            if (is_integer(pm[0]->m_vector->get_type()))
                exp->set_integer(v);
            else
                exp->set_float(v);
            return true;
        }
        else if (pm[0]->is_matrix())
        {
            exp->set_vector(pm[0]->m_matrix->get_product());
            return true;
        }
    }
    return false;
}

bool func_size(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1)
    {
        if (pm[0]->is_vector())
        {
            exp->set_integer(pm[0]->m_vector->get_size());
            return true;
        }
        else if (pm[0]->is_matrix())
        {
            Vector *v = vector_new(2, Number_Type::nt_int64);
            Matrix_Dim dim = pm[0]->m_matrix->get_size();
            v->set_unit(0, dim.row);
            v->set_unit(1, dim.col);
            exp->set_vector(v);
            return true;
        }
    }
    return false;
}

bool func_numel(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1)
    {
        if (pm[0]->is_vector())
        {
            exp->set_integer(pm[0]->m_vector->get_size());
            return true;
        }
        else if (pm[0]->is_matrix())
        {
            Matrix_Dim md = pm[0]->m_matrix->get_size();
            exp->set_integer(md.row * md.col);
            return true;
        }
    }
    return false;
}

bool func_reshape(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 2)
    {
        if (pm[0]->is_matrix() && pm[1]->is_vector()
            && pm[1]->m_vector->get_size() == 2 && is_integer(pm[1]->m_vector->get_type()))
        {
            Matrix_Dim dim = pm[0]->m_matrix->get_size();
            long row = pm[1]->m_vector->get_unit(0);
            long col = pm[1]->m_vector->get_unit(1);
            if (dim.row * dim.col == row * col)
            {
                dim.row = row;
                dim.col = col;
                pm[0]->m_matrix->set_size(dim);
                if (pm[0]->m_varname)   // if pm[0] is a variable, transmit it's name to upper expression
                {
                    exp->set_varname(pm[0]->m_varname);
                    exp->set_unknown("");
                }
                else
                    exp->set_matrix(pm[0]->m_matrix);
                return true;
            }
        }
    }
    return false;
}

bool func_sort(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1)
    {
        if (pm[0]->is_vector())
            goto vector_1;
        else if (pm[0]->is_matrix())
            goto matrix_1;
        else
            return false;
    }
    else if (num == 2)
    {
        if (pm[0]->is_vector())
        {
            if (pm[1]->is_string())
            {
                if (pm[1]->m_string == "ascend")
                {
vector_1:
                    pm[0]->m_vector->sort_ascend();
                    goto vector_2;
                }
                else if (pm[1]->m_string == "descend")
                {
                    pm[0]->m_vector->sort_descend();
                    goto vector_2;
                }
                else
                    return false;
vector_2:
                if (pm[0]->m_varname)   // if pm[0] is a variable, transmit it's name to upper expression
                {
                    exp->set_varname(pm[0]->m_varname);
                    exp->set_unknown("");
                }
                else
                    exp->set_vector(pm[0]->m_vector);
                return true;
            }
        }
        else if (pm[0]->is_matrix())
        {
            if (pm[1]->is_string())
            {
                if (pm[1]->m_string == "ascend")
                {
matrix_1:
                    pm[0]->m_matrix->sort_ascend();
                    goto matrix_2;
                }
                else if (pm[1]->m_string == "descend")
                {
                    pm[0]->m_matrix->sort_descend();
                    goto matrix_2;
                }
                else
                    return false;
matrix_2:
                if (pm[0]->m_varname)   // if pm[0] is a variable, transmit it's name to upper expression
                {
                    exp->set_varname(pm[0]->m_varname);
                    exp->set_unknown("");
                }
                else
                    exp->set_matrix(pm[0]->m_matrix);
                return true;
            }
        }
    }
    return false;
}

bool func_sum(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 1)
    {
        if (pm[0]->is_vector())
        {
            double s = pm[0]->m_vector->get_sum();
            if (is_integer(pm[0]->m_vector->get_type()))
                exp->set_integer(s);
            else
                exp->set_float(s);
            return true;
        }
        else if (pm[0]->is_matrix())
        {
            Vector *v = pm[0]->m_matrix->get_sum();
            exp->set_vector(v);
            return true;
        }
    }
    return false;
}

bool func_dot(Express *exp)
{
    vector<Express*> &pm = *exp->m_explist;
    const size_t num = pm.size();

    if (num == 2)
    {
        if (pm[0]->is_vector() && pm[1]->is_vector())
        {
            if (pm[0]->m_vector->get_size() == pm[1]->m_vector->get_size())
            {
                double dp = vector_dot(pm[0]->m_vector.get(), pm[1]->m_vector.get());
                if (is_integer(pm[0]->m_vector->get_type()) && is_integer(pm[1]->m_vector->get_type()))
                    exp->set_integer(dp);
                else
                    exp->set_float(dp);
                return true;
            }
        }
        else if (pm[0]->is_matrix() && pm[1]->is_matrix())
        {
            Matrix_Dim d0 = pm[0]->m_matrix->get_size();
            Matrix_Dim d1 = pm[1]->m_matrix->get_size();
            if (d0.row == d1.row && d0.col == d1.col)
            {
                Vector *v = matrix_dot(pm[0]->m_matrix.get(), pm[1]->m_matrix.get());
                if (is_integer(pm[0]->m_matrix->get_type()) && is_integer(pm[1]->m_matrix->get_type()))
                {
                    Vector *iv = vector_new(v->get_size(), Number_Type::nt_int64);
                    for (int i = 0; i < iv->get_size(); ++i)
                        iv->set_unit(i, v->get_unit(i));
                    delete(v);
                    v = iv;
                }
                exp->set_vector(v);
                return true;
            }
        }
    }
    return false;
}
