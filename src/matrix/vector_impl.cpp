#include "vector_impl.h"
#include <cstdio>
#include <cstring>


using namespace std;

/***********************************************************************
 * Specific type of Vector definition
 ***********************************************************************/

Vector_double::Vector_double(size_t m) : Vector_T<double>(m)
{
    m_type = Number_Type::nt_double;
}

Vector_single::Vector_single(size_t m) : Vector_T<float>(m)
{
    m_type = Number_Type::nt_single;
}

Vector_int8::Vector_int8(size_t m) : Vector_T<char>(m)
{
    m_type = Number_Type::nt_int8;
}

Vector_int16::Vector_int16(size_t m) : Vector_T<short>(m)
{
    m_type = Number_Type::nt_int16;
}

Vector_int32::Vector_int32(size_t m) : Vector_T<int>(m)
{
    m_type = Number_Type::nt_int32;
}

Vector_int64::Vector_int64(size_t m) : Vector_T<long>(m)
{
    m_type = Number_Type::nt_int64;
}

Vector_uint8::Vector_uint8(size_t m) : Vector_T<unsigned char>(m)
{
    m_type = Number_Type::nt_uint8;
}

Vector_uint16::Vector_uint16(size_t m) : Vector_T<unsigned short>(m)
{
    m_type = Number_Type::nt_uint16;
}

Vector_uint32::Vector_uint32(size_t m) : Vector_T<unsigned int>(m)
{
    m_type = Number_Type::nt_uint32;
}

Vector_uint64::Vector_uint64(size_t m) : Vector_T<unsigned long>(m)
{
    m_type = Number_Type::nt_uint64;
}

/***********************************************************************
 * API functions
 ***********************************************************************/

Vector* vector_new(size_t m, Number_Type dt)
{
    switch (dt)
    {
    case nt_double:
        return new Vector_double(m);
    case nt_single:
        return new Vector_single(m);
    case nt_int8:
        return new Vector_int8(m);
    case nt_int16:
        return new Vector_int16(m);
    case nt_int32:
        return new Vector_int32(m);
    case nt_int64:
        return new Vector_int64(m);
    case nt_uint8:
        return new Vector_uint8(m);
    case nt_uint16:
        return new Vector_uint16(m);
    case nt_uint32:
        return new Vector_uint32(m);
    case nt_uint64:
        return new Vector_uint64(m);
    default:
        error("wrong Number_Type");
        exit(1);
    }
}

/**
 * API: print()
 */
void vector_print(Vector *v)
{
    v->print();
}

/**
 * copy a Vector along with data
 */
Vector* vector_copy(Vector *v)
{
    if (!v)
        return NULL;

    Vector *n = vector_new(v->get_size(), v->get_type());
    memcpy(n->get_data(), v->get_data(), v->get_unit_size() * v->get_size());
    return n;
}

/**
 * Vector scalar multiply, return a new Vector
 */
Vector* vector_scalar_multiply(Vector *v, double d)
{
    Vector *n = vector_copy(v);
    if (n)
        n->scalar_multiply(d);
    return n;
}

/**
 * Vector scalar division, return a new Vector
 */
Vector* vector_scalar_divide(Vector *v, double d)
{
    Vector *n = vector_copy(v);
    if (n)
        n->scalar_divide(d);
    return n;
}

/**
 * Vector addition
 */
Vector* vector_plus(Vector *one, Vector *two, Number_Type dt)
{
    if (!one || !two)
    {
        error("NULL Vector");
        return NULL;
    }

    if (one->get_size() != two->get_size())
    {
        error("invalid size");
        return NULL;
    }

    Vector *n = vector_new(one->get_size(), dt);
    for (size_t i = 0; i < n->get_size(); ++i)
    {
        n->set_unit(i, one->access_unit(i), one->get_type());
        n->add_unit(i, two->access_unit(i), two->get_type());
    }

    return n;
}

/**
 * Vector addition, the returned Vector take type from one
 */
Vector* vector_plus(Vector *one, Vector *two)
{
    if (!one || !two)
    {
        error("NULL Vector");
        return NULL;
    }

    if (one->get_size() != two->get_size())
    {
        error("invalid size");
        return NULL;
    }

    Vector *n = vector_copy(one);
    for (size_t i = 0; i < n->get_size(); ++i)
        n->add_unit(i, two->access_unit(i), two->get_type());

    return n;
}

/**
 * Vector subtraction
 */
Vector* vector_minus(Vector *one, Vector *two, Number_Type dt)
{
    if (!one || !two)
    {
        error("NULL Vector");
        return NULL;
    }

    if (one->get_size() != two->get_size())
    {
        error("invalid size");
        return NULL;
    }

    Vector *n = vector_new(one->get_size(), dt);
    for (size_t i = 0; i < n->get_size(); ++i)
    {
        n->set_unit(i, one->access_unit(i), one->get_type());
        n->minus_unit(i, two->access_unit(i), two->get_type());
    }

    return n;
}

/**
 * Vector subtraction, the returned Vector take type from one
 */
Vector* vector_minus(Vector *one, Vector *two)
{
    if (!one || !two)
    {
        error("NULL Vector");
        return NULL;
    }

    if (one->get_size() != two->get_size())
    {
        error("invalid size");
        return NULL;
    }

    Vector *n = vector_copy(one);
    for (size_t i = 0; i < n->get_size(); ++i)
        n->minus_unit(i, two->access_unit(i), two->get_type());

    return n;
}

/**
 * compute dot product of the two Vector
**/
double vector_dot(Vector *a, Vector *b)
{
    if (!a || !b)
    {
        error("NULL input vector");
        return 0;
    }

    if (a->get_size() != b->get_size())
    {
        error("invalid size");
        return 0;
    }

    double dp = 0;
    for (size_t i = 0; i < a->get_size(); ++i)
        dp += a->get_unit(i) * b->get_unit(i);
    return dp;
}
