#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/U64.h"



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
