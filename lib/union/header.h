#ifndef __UNION_H__
#define __UNION_H__

#include "struct.h"

union_num_t union_num_wrap_sig(sig_num_t sig, uint64_t size);
flt_num_t union_num_unwrap_flt(union_num_t u);
union_num_t union_num_copy(union_num_t u);
void union_num_free(union_num_t u);

union_num_t union_num_mul(union_num_t u_1, union_num_t u_2);
union_num_t union_num_add(union_num_t u_1, union_num_t u_2);

#endif
