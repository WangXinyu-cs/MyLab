#include "numbertype.h"
#include "cstring"

const char* WRONG  = "wrong";
const char* DOUBLE = "double";
const char* SINGLE = "single";
const char* INT8   = "int8";
const char* INT16  = "int16";
const char* INT32  = "int32";
const char* INT64  = "int64";
const char* UINT8  = "uint8";
const char* UINT16 = "uint16";
const char* UINT32 = "uint32";
const char* UINT64 = "uint64";

Number_Type name_to_nt(const char *name)
{
    if (!strcmp(name, DOUBLE))
        return Number_Type::nt_double;
    else if (!strcmp(name, SINGLE))
        return Number_Type::nt_single;
    else if (!strcmp(name, INT8))
        return Number_Type::nt_int8;
    else if (!strcmp(name, INT16))
        return Number_Type::nt_int16;
    else if (!strcmp(name, INT32))
        return Number_Type::nt_int32;
    else if (!strcmp(name, INT64))
        return Number_Type::nt_int64;
    else if (!strcmp(name, UINT8))
        return Number_Type::nt_uint8;
    else if (!strcmp(name, UINT16))
        return Number_Type::nt_uint16;
    else if (!strcmp(name, UINT32))
        return Number_Type::nt_uint32;
    else if (!strcmp(name, UINT64))
        return Number_Type::nt_uint64;
    else
        return Number_Type::nt_wrong;
}

extern const char* nt_to_name(Number_Type nt)
{
    switch (nt)
    {
    case Number_Type::nt_double: return DOUBLE;
    case Number_Type::nt_single: return SINGLE;
    case Number_Type::nt_int8:   return INT8;
    case Number_Type::nt_int16:  return INT16;
    case Number_Type::nt_int32:  return INT32;
    case Number_Type::nt_int64:  return INT64;
    case Number_Type::nt_uint8:  return UINT8;
    case Number_Type::nt_uint16: return UINT16;
    case Number_Type::nt_uint32: return UINT32;
    case Number_Type::nt_uint64: return UINT64;
    default: return WRONG;
    }
}

bool is_integer(Number_Type nt)
{
    return nt == Number_Type::nt_int8 || nt == Number_Type::nt_int16
    || nt == Number_Type::nt_int32 || nt == Number_Type::nt_int64
    || nt == Number_Type::nt_uint8 || nt == Number_Type::nt_uint16
    || nt == Number_Type::nt_uint32 || nt == Number_Type::nt_uint64;
}

bool is_float(Number_Type nt)
{
    return nt, Number_Type::nt_double || nt, Number_Type::nt_single;
}