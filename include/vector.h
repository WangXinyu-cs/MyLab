#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "numbertype.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>


#define error(msg) fprintf(stderr, "[%s:%d] %s\n", __FILE__, __LINE__, msg)
#define warn(msg)  fprintf(stderr, "warning: %s\n", msg)

class Vector
{
public:
    virtual ~Vector() {}

    virtual Number_Type get_type() = 0;
    virtual void* get_data() = 0;
    virtual size_t get_size() = 0;
    virtual size_t get_unit_size() = 0;
    virtual void print() = 0;
    virtual void* access_unit(size_t) = 0;
    virtual double get_unit(size_t) = 0;
    virtual void set_unit(size_t, void*, Number_Type) = 0;
    virtual void set_unit(size_t, double) = 0;
    virtual void add_unit(size_t, void*, Number_Type) = 0;
    virtual void minus_unit(size_t, void*, Number_Type) = 0;
    virtual void scalar_multiply(double) = 0;
    virtual void scalar_divide(double) = 0;
    virtual double get_max() = 0;
    virtual double get_min() = 0;
    virtual double get_product() = 0;
    virtual void sort_ascend() = 0;
    virtual void sort_descend() = 0;
    virtual double get_sum() = 0;
};


template<class T> class Vector_T: public Vector
{
protected:
    T           *m_data;
    size_t      m_size;
    Number_Type   m_type;
    // why not using std::vector<T> and typeof(T) ?

public:
    using value_type = T;

    Vector_T(size_t m)
    {
        m_data = (T*) malloc(sizeof(T) * m);
        bzero(m_data, sizeof(T) * m);
        m_size = m;
    }

    virtual ~Vector_T()
    {
        if (m_data)
        {
            free(m_data);
            m_data = NULL;
        }
    }

    virtual Number_Type get_type() { return m_type; }
    virtual void* get_data() { return m_data; }
    virtual size_t get_size() { return m_size; }
    virtual size_t get_unit_size() { return sizeof(T); }

    virtual void print()
    {
        const char* format;

        if (!m_data || !m_size)
        {
            printf("<>\n");
            return;
        }

        switch (m_type)
        {
        case nt_single:
            format = "  %f";
            break;
        case nt_int8:
        case nt_int16:
        case nt_int32:
            format = "  %d";
            break;
        case nt_int64:
            format = "  %ld";
            break;
        case nt_uint8:
        case nt_uint16:
        case nt_uint32:
            format = "  %u";
            break;
        case nt_uint64:
            format = "  %lu";
            break;
        default:
            format = "  %lf";
        }

        for (size_t i = 0; i < m_size; ++i)
            printf(format, m_data[i]);
        printf("\n");
    }

    virtual void* access_unit(size_t index)
    {
        return index >= m_size ? NULL : &m_data[index];
    }

    virtual double get_unit(size_t index)
    {
        return index >= m_size ? 0.0 : m_data[index];
    }

    virtual void set_unit(size_t n, void* data, Number_Type dt)
    {
        if (n >= m_size || !data)
        {
            error("invalid parameter");
            return;
        }

        switch (dt)
        {
        case Number_Type::nt_single:
            m_data[n] = *(float*)data;
            break;
        case Number_Type::nt_double:
            m_data[n] = *(double*)data;
            break;
        case Number_Type::nt_int8:
            m_data[n] = *(char*)data;
            break;
        case Number_Type::nt_int16:
            m_data[n] = *(short*)data;
            break;
        case Number_Type::nt_int32:
            m_data[n] = *(int*)data;
            break;
        case Number_Type::nt_int64:
            m_data[n] = *(long*)data;
            break;
        case Number_Type::nt_uint8:
            m_data[n] = *(unsigned char*)data;
            break;
        case Number_Type::nt_uint16:
            m_data[n] = *(unsigned short*)data;
            break;
        case Number_Type::nt_uint32:
            m_data[n] = *(unsigned int*)data;
            break;
        case Number_Type::nt_uint64:
            m_data[n] = *(unsigned long*)data;
            break;
        default:
            error("invalid Number_Type");
        }
    }

    virtual void set_unit(size_t m, double v)
    {
        if (m >= m_size)
        {
            error("index out of bounds");
            return;
        }

        m_data[m] = (T)v;
    }

    virtual void add_unit(size_t n, void* data, Number_Type dt)
    {
        if (n >= m_size || !data)
        {
            error("invalid parameter");
            return;
        }

        switch (dt)
        {
        case Number_Type::nt_single:
            m_data[n] += *(float*)data;
            break;
        case Number_Type::nt_double:
            m_data[n] += *(double*)data;
            break;
        case Number_Type::nt_int8:
            m_data[n] += *(char*)data;
            break;
        case Number_Type::nt_int16:
            m_data[n] += *(short*)data;
            break;
        case Number_Type::nt_int32:
            m_data[n] += *(int*)data;
            break;
        case Number_Type::nt_int64:
            m_data[n] += *(long*)data;
            break;
        case Number_Type::nt_uint8:
            m_data[n] += *(unsigned char*)data;
            break;
        case Number_Type::nt_uint16:
            m_data[n] += *(unsigned short*)data;
            break;
        case Number_Type::nt_uint32:
            m_data[n] += *(unsigned int*)data;
            break;
        case Number_Type::nt_uint64:
            m_data[n] += *(unsigned long*)data;
            break;
        default:
            error("invalid Number_Type");
        }
    }

    virtual void minus_unit(size_t n, void* data, Number_Type dt)
    {
        if (n >= m_size || !data)
        {
            error("wrong parameter");
            return;
        }

        switch (dt)
        {
        case Number_Type::nt_single:
            m_data[n] -= *(float*)data;
            break;
        case Number_Type::nt_double:
            m_data[n] -= *(double*)data;
            break;
        case Number_Type::nt_int8:
            m_data[n] -= *(char*)data;
            break;
        case Number_Type::nt_int16:
            m_data[n] -= *(short*)data;
            break;
        case Number_Type::nt_int32:
            m_data[n] -= *(int*)data;
            break;
        case Number_Type::nt_int64:
            m_data[n] -= *(long*)data;
            break;
        case Number_Type::nt_uint8:
            m_data[n] -= *(unsigned char*)data;
            break;
        case Number_Type::nt_uint16:
            m_data[n] -= *(unsigned short*)data;
            break;
        case Number_Type::nt_uint32:
            m_data[n] -= *(unsigned int*)data;
            break;
        case Number_Type::nt_uint64:
            m_data[n] -= *(unsigned long*)data;
            break;
        default:
            error("invalid Number_Type");
        }
    }

    virtual void scalar_multiply(double v)
    {
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] *= (T)v;
    }

    virtual void scalar_divide(double v)
    {
        if (v == 0.0d)
        {
            warn("division by zero");
            return;
        }
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] /= (T)v;
    }

    virtual double get_max()
    {
        if (!m_size)
            return 0;

        T max = m_data[0];
        for (size_t i = 1; i < m_size; ++i)
        {
            if (m_data[i] > max)
                max = m_data[i];
        }
        return max;
    }

    virtual double get_min()
    {
        if (!m_size)
            return 0;

        T min = m_data[0];
        for (size_t i = 1; i < m_size; ++i)
        {
            if (m_data[i] < min)
                min = m_data[i];
        }
        return min;
    }

    virtual double get_product()
    {
        if (!m_size)
            return 0;

        T prod = m_data[0];
        for (size_t i = 1; i < m_size; ++i)
            prod *= m_data[i];
        return prod;
    }

    virtual void sort_ascend()
    {
        std::sort(m_data, m_data + m_size, comp_less<T>);
    }

    virtual void sort_descend()
    {
        std::sort(m_data, m_data + m_size, comp_great<T>);
    }

    virtual double get_sum()
    {
        T sum = 0;
        for (int i = 0; i < m_size; ++i)
            sum += m_data[i];
        return sum;
    }
};


/***********************************************************************
 * Vector API
************************************************************************/

/**
 * print the Vector
**/
void vector_print(Vector*);

/**
 * create a Vector without assignment
**/
Vector* vector_new(size_t m, Number_Type dt = Number_Type::nt_double);

/**
 * assign a Vector. InputIterator point to double value.
**/
template<class InputIterator>
void vector_assign(Vector* v, InputIterator first, InputIterator last)
{
    if (!v)
    {
        error("NULL Vector");
        return;
    }

    size_t m = v->get_size();
    for (size_t i = 0; i < m && first != last; ++i, ++first)
    {
        v->set_unit(i, *first);
    }
}

/**
 * copy a Vector along with data
**/
Vector* vector_copy(Vector *v);

/**
 * Vector scalar multiply, return a new Vector
**/
Vector* vector_scalar_multiply(Vector *m, double v);

/**
 * Vector scalar division, return a new Vector
**/
Vector* vector_scalar_divide(Vector *m, double v);

/**
 * Vector addition
**/
Vector* vector_plus(Vector *one, Vector *two, Number_Type dt);

/**
 * Vector addition, the returned Vector take type from one
**/
Vector* vector_plus(Vector *one, Vector *two);

/**
 * Vector subtraction
**/
Vector* vector_minus(Vector *one, Vector *two, Number_Type dt);

/**
 * Vector subtraction, the returned Vector take type from one
**/
Vector* vector_minus(Vector *one, Vector *two);

/**
 * create a Vector from the arithmetic progression
**/
template<class T>
Vector* vector_diff(T a1, T diff, T an, Number_Type dt)
{
    int len = (an - a1) / diff + 1;
    if (len < 0)
    {
        error("invalid arithmetic progression");
        return NULL;
    }
    Vector* v = vector_new(len, dt);
    T *data = (T*)v->get_data();
    for (int i = 0; i < len; ++i)
        data[i] = a1 + i * diff;

    return v;
}

/**
 * compute dot product of the two Vector
**/
double vector_dot(Vector *a, Vector *b);

#endif
