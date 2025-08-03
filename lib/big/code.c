#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/uint.h"

#include "../union/header.h"
#include "../bilinear/header.h"



#ifdef DEBUG
#endif



// out vector length 3, returns P, Q, R in that order
void binary_splitting_file(
    union_num_t out[],
    uint64_t size,
    uint64_t i_0,
    uint64_t i_max,
    uint64_t depth,
    uint64_t max
)
{
    assert(i_0 <= i_max);
    if(i_0 == i_max)
    {
        int64_t p = 2 * i_0 - 3;
        int64_t q = 8 * i_0;
        int64_t u = 1 - 2 * i_0;
        int64_t v = 4 * i_0 + 2;

        sig_num_t sig_p = sig_num_wrap(p * v);
        sig_num_t sig_q = sig_num_wrap(q * v);
        sig_num_t sig_r = sig_num_wrap(p * u);

        out[0] = union_num_wrap_sig(sig_p, size);
        out[1] = union_num_wrap_sig(sig_q, size);
        out[2] = union_num_wrap_sig(sig_r, size);

        return;
    }

    uint64_t i_half = (i_0 + i_max) / 2;
    union_num_t res_1[3], res_2[3];
    binary_splitting_file(res_1, size, i_0       , i_half, depth + 1, max);
    binary_splitting_file(res_2, size, i_half + 1, i_max , depth + 1, max);
    binary_splitting_join(out, res_1, res_2);
}
