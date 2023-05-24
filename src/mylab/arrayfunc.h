#ifndef _ARRAY_FUNC_H_
#define _ARRAY_FUNC_H_

#include "dstruct.h"

bool func_init_matrix(Express *exp);
bool func_diag(Express *exp);
bool func_blkdiag(Express *exp);
bool func_cat(Express *exp);
bool func_transpose(Express *exp);
bool func_inv(Express *exp);
bool func_det(Express *exp);
bool func_magic(Express *exp);
bool func_find(Express *exp);
bool func_length(Express *exp);
bool func_linspace(Express *exp);
bool func_logspace(Express *exp);
bool func_max(Express *exp);
bool func_min(Express *exp);
bool func_prod(Express *exp);
bool func_size(Express *exp);
bool func_numel(Express *exp);
bool func_reshape(Express *exp);
bool func_sort(Express *exp);
bool func_sum(Express *exp);
bool func_dot(Express *exp);

#endif