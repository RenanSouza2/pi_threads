#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/uint.h"
#include "../../mods/number//header.h"

#include"../union/header.h"



#ifdef DEBUG
#endif



fxd_num_t pi_v1(uint64_t size)
{
    uint64_t index_max = 32 * size + 4;

    fxd_num_t fxd_1 = fxd_num_wrap(1, size);
    fxd_num_t fxd_m_1_2 = fxd_num_div(
        fxd_num_wrap(-1, size),
        fxd_num_wrap( 2, size)
    );

    fxd_num_t fxd_b = fxd_num_wrap(6, size);
    fxd_num_t fxd_pi = fxd_num_wrap(3, size);
    for(uint64_t i=1; i<index_max; i++)
    {
        fxd_num_t fxd_a = fxd_num_div(
            fxd_num_wrap(2 * i - 3, size),
            fxd_num_wrap(8 * i, size)
        );
        fxd_b = fxd_num_mul(fxd_b, fxd_a);

        fxd_num_t fxd_c = fxd_num_div(
            fxd_num_copy(fxd_1),
            fxd_num_wrap(2 * i + 1, size)
        );
        fxd_c = fxd_num_add(fxd_c, fxd_num_copy(fxd_m_1_2));
        fxd_num_t fxd_d = fxd_num_mul(fxd_num_copy(fxd_b), fxd_c);

        fxd_pi = fxd_num_add(fxd_pi, fxd_d);
    }

    fxd_num_free(fxd_b);
    fxd_num_free(fxd_1);
    fxd_num_free(fxd_m_1_2);

    return fxd_pi;
}

fxd_num_t pi_v2(uint64_t size)
{
    uint64_t index_max = 32 * size + 4;

    fxd_num_t fxd_a = fxd_num_wrap(6, size);
    fxd_num_t fxd_pi = fxd_num_wrap(3, size);
    for(uint64_t i=1; i<index_max; i++)
    {
        fxd_a = fxd_num_mul_sig(fxd_a, sig_num_wrap((int64_t)2 * i - 3));
        fxd_a = fxd_num_div_sig(fxd_a, sig_num_wrap((int64_t)8 * i));

        fxd_num_t fxd_b = fxd_num_copy(fxd_a);
        fxd_b = fxd_num_mul_sig(fxd_b, sig_num_wrap((int64_t)1 - 2 * i));
        fxd_b = fxd_num_div_sig(fxd_b, sig_num_wrap((int64_t)4 * i + 2));

        fxd_pi = fxd_num_add(fxd_pi, fxd_b);
    }
    fxd_num_free(fxd_a);

    return fxd_pi;
}





void binary_splitting_join(
    union_num_t out[],
    union_num_t res_1[3],
    union_num_t res_2[3]
)
{
    union_num_t u_r_1 = union_num_mul(res_1[2], union_num_copy(res_2[1]));
    union_num_t u_r_2 = union_num_mul(res_2[2], union_num_copy(res_1[0]));

    union_num_t out_0 = union_num_mul(res_1[0], res_2[0]);
    union_num_t out_1 = union_num_mul(res_1[1], res_2[1]);
    union_num_t out_2 = union_num_add(u_r_1, u_r_2);

    out[0] = out_0;
    out[1] = out_1;
    out[2] = out_2;
}

// out vector length 3, returns P, Q, R in that order
void binary_splitting(
    union_num_t out[],
    uint64_t size,
    uint64_t i_0,
    uint64_t i_max,
    uint64_t depth,
    uint64_t max
)
{
    // if(depth == 7 && max)
    //     fprintf(stderr, "\n%.1f %%", 100.0 * i_0 / max);

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
    binary_splitting(res_1, size, i_0       , i_half, depth + 1, max);
    binary_splitting(res_2, size, i_half + 1, i_max , depth + 1, max);
    binary_splitting_join(out, res_1, res_2);
}

flt_num_t pi_v3(uint64_t size)
{
    uint64_t index_max = 32 * size + 4;

    union_num_t res[3];
    binary_splitting(res, size, 1, index_max, 0, index_max);
    union_num_free(res[0]);

    flt_num_t flt_q = union_num_unwrap_flt(res[1]);
    flt_num_t flt_r = union_num_unwrap_flt(res[2]);

    flt_num_t flt_pi = flt_r;
    flt_pi = flt_num_mul_sig(flt_pi, sig_num_wrap(6));
    flt_pi = flt_num_div(flt_pi, flt_q);
    flt_pi = flt_num_add(flt_pi, flt_num_wrap(3, size));
    return flt_pi;
}
