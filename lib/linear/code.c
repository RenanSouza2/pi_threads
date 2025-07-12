#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/U64.h"
#include "../../mods/number/lib/float/header.h"
#include "../../mods/number/lib/sig/header.h"



#ifdef DEBUG
#endif



fix_num_t pi_v1(uint64_t size)
{
    uint64_t index_max = 32 * size + 4;

    fix_num_t fix_1 = fix_num_wrap(1, size);
    fix_num_t fix_m_1_2 = fix_num_div(
        fix_num_wrap(-1, size),
        fix_num_wrap( 2, size)
    );

    fix_num_t fix_b = fix_num_wrap(6, size);
    fix_num_t fix_pi = fix_num_wrap(3, size);
    for(uint64_t i=1; i<index_max; i++)
    {
        fix_num_t fix_a = fix_num_div(
            fix_num_wrap(2 * i - 3, size),
            fix_num_wrap(8 * i, size)
        );
        fix_b = fix_num_mul(fix_b, fix_a);

        fix_num_t fix_c = fix_num_div(
            fix_num_copy(fix_1),
            fix_num_wrap(2 * i + 1, size)
        );
        fix_c = fix_num_add(fix_c, fix_num_copy(fix_m_1_2));
        fix_num_t fix_d = fix_num_mul(fix_num_copy(fix_b), fix_c);

        fix_pi = fix_num_add(fix_pi, fix_d);
    }

    fix_num_free(fix_b);
    fix_num_free(fix_1);
    fix_num_free(fix_m_1_2);

    return fix_pi;
}

fix_num_t pi_v2(uint64_t size)
{
    uint64_t index_max = 32 * size + 4;

    fix_num_t fix_a = fix_num_wrap(6, size);
    fix_num_t fix_pi = fix_num_wrap(3, size);
    for(uint64_t i=1; i<index_max; i++)
    {
        fix_a = fix_num_mul_sig(fix_a, sig_num_wrap((int64_t)2 * i - 3));
        fix_a = fix_num_div_sig(fix_a, sig_num_wrap((int64_t)8 * i));

        fix_num_t fix_b = fix_num_copy(fix_a);
        fix_b = fix_num_mul_sig(fix_b, sig_num_wrap((int64_t)1 - 2 * i));
        fix_b = fix_num_div_sig(fix_b, sig_num_wrap((int64_t)4 * i + 2));

        fix_pi = fix_num_add(fix_pi, fix_b);
    }
    fix_num_free(fix_a);

    return fix_pi;
}




// out vector length 4, returns P, Q, V, R in that order
void binomial_spliting(float_num_t out[], uint64_t size, uint64_t i_0, uint64_t i_max)
{
    // if(i_max - i_0 > 100) printf("\ni_0: %lu\t\ti_max: %lu", i_0, i_max);

    assert(i_0 <= i_max);
    if(i_0 == i_max)
    {
        float_num_t flt_p = float_num_wrap((int64_t)2 * i_0 - 3, size);
        float_num_t flt_v = float_num_wrap((int64_t)4 * i_0 + 2, size);
        float_num_t flt_q = float_num_mul(
            float_num_copy(flt_v),
            float_num_wrap((int64_t)8 * i_0, size)
        );
        float_num_t flt_r = float_num_mul(
            float_num_copy(flt_p),
            float_num_wrap((int64_t)1 - 2 * i_0, size)
        );

        out[0] = flt_p;
        out[1] = flt_q;
        out[2] = flt_v;
        out[3] = flt_r;
        return;
    }

    uint64_t i_half = (i_0 + i_max) / 2;
    float_num_t res_1[4], res_2[4];
    binomial_spliting(res_1, size, i_0, i_half);
    binomial_spliting(res_2, size, i_half + 1, i_max);

    float_num_t flt_r_1 = res_1[3];
    flt_r_1 = float_num_mul(flt_r_1, float_num_copy(res_2[1]));
    
    float_num_t flt_r_2 = res_2[3];
    flt_r_2 = float_num_mul(flt_r_2, float_num_copy(res_1[0]));
    flt_r_2 = float_num_mul(flt_r_2, float_num_copy(res_1[2]));

    for(uint64_t i=0; i<3; i++)
        out[i] = float_num_mul(res_1[i], res_2[i]);

    out[3] = float_num_add(flt_r_1, flt_r_2);
}

float_num_t pi_v3(uint64_t size)
{
    uint64_t index_max = 32 * size + 4;

    float_num_t res[4];
    binomial_spliting(res, size, 1, index_max);
    float_num_free(res[0]);
    float_num_free(res[2]);

    float_num_t flt_pi = res[3];
    flt_pi = float_num_mul_sig(flt_pi, sig_num_wrap(6));
    flt_pi = float_num_div(flt_pi, res[1]);
    flt_pi = float_num_add(flt_pi, float_num_wrap(3, size));
    return flt_pi;
}
