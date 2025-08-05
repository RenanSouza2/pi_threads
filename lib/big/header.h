#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

#include "../../mods/number/header.h"
#include "../union/struct.h"

void sig_res_save(sig_num_t res[], uint64_t i_0, uint64_t span);
bool sig_res_try_load(sig_num_t out[3], uint64_t i_0, uint64_t span);

void union_split_big_span(union_num_t out[], uint64_t size, uint64_t i_0, uint64_t span);
flt_num_t pi_big(uint64_t size);

#endif
