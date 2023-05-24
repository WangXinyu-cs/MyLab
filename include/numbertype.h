#ifndef _NUMBERTYPE_H_
#define _NUMBERTYPE_H_

#include <algorithm>

enum Number_Type
{
    nt_wrong,
	nt_double,
	nt_single,
	nt_int8,
	nt_int16,
	nt_int32,
	nt_int64,
	nt_uint8,
	nt_uint16,
	nt_uint32,
	nt_uint64
};

extern const char* WRONG;
extern const char* DOUBLE;
extern const char* SINGLE;
extern const char* INT8;
extern const char* INT16;
extern const char* INT32;
extern const char* INT64;
extern const char* UINT8;
extern const char* UINT16;
extern const char* UINT32;
extern const char* UINT64;

extern Number_Type name_to_nt(const char* name);
extern const char* nt_to_name(Number_Type nt);
extern bool is_integer(Number_Type nt);
extern bool is_float(Number_Type nt);

template<class T>
bool comp_less(T &a, T &b) { return a < b; }

template<class T>
bool comp_great(T &a, T &b) { return a > b; }

#endif
