#ifndef __LINEAR_H__
#define __LINEAR_H__

#include "../../mods/number/lib/fix/header.h"
#include "../../mods/number/lib/float/header.h"

fix_num_t pi_v1(uint64_t size);
fix_num_t pi_v2(uint64_t size);

void binomial_spliting(float_num_t out[], uint64_t size, uint64_t i_0, uint64_t i_max);
float_num_t pi_v3(uint64_t size);

#endif
