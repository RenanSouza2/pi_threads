#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define SKIP_TIME

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/time.h"
#include "../../mods/macros/U64.h"
#include "../../mods/number/lib/fix/header.h"
#include "../../mods/number/lib/sig/header.h"
#include "../../mods/number/lib/num/header.h"
// #include "../../mods/number/lib/num/struct.h"

#include "../pear/header.h"



#ifdef DEBUG
#endif



fix_num_t jumpstart(uint64_t index_max, uint64_t size)
{
    uint64_t layer_count = 3;

    if(index_max == 0)
        return fix_num_wrap(6, size - 1);

    assert(index_max > 3);
    uint64_t index_max_prod = index_max / 2;
    uint64_t i_max = (index_max_prod + layer_count - 2) / layer_count;
    uint64_t delta = (index_max + 1) / 2;
    uint64_t pos = size - index_max / 32;
    if(pos > size + 2) pos = 2;

    float_num_t flt_1 = float_num_wrap(-6, pos);
    float_num_t flt_2 = float_num_wrap( 1, pos);
    for(uint64_t i=0; i<i_max; i++)
    {
        uint64_t index = 2 + layer_count * i;

        sig_num_t sig_1 = sig_num_wrap(2 * (index + delta) - 3);
        sig_num_t sig_2 = sig_num_wrap(index);
        for(uint64_t k=1; (k<layer_count) && index + k <= index_max_prod; k++)
        {
            sig_1 = sig_num_mul(sig_1, sig_num_wrap(2 * (index + k + delta) - 3));
            sig_2 = sig_num_mul(sig_2, sig_num_wrap(index + k));
        }

        flt_1 = float_num_mul_sig(flt_1, sig_1);
        flt_2 = float_num_mul_sig(flt_2, sig_2);
    }
    flt_1 = float_num_shr(flt_1, 7 * index_max / 2);
    flt_1 = float_num_div(flt_1, flt_2);
    return fix_num_wrap_float(flt_1, size);
}



STRUCT(thread_pi_args)
{
    uint64_t size;
    uint64_t index_0;
    uint64_t index_max;
    fix_num_t fix_res;
};

handler_p thread_pi(handler_p _args)
{
    thread_pi_args_p args = (thread_pi_args_p)_args;

    uint64_t size = args->size;
    uint64_t index_0 = args->index_0;
    uint64_t index_max = args->index_max;

    fix_num_t fix_a = jumpstart(index_0 - 1, size);
    fix_num_t fix_res = fix_num_wrap(0, size - 1);
    for(uint64_t i=index_0; i<index_max; i++)
    {
        fix_a = fix_num_mul_sig(fix_a, sig_num_wrap((int64_t)2 * i - 3));
        fix_a = fix_num_div_sig(fix_a, sig_num_wrap((int64_t)8 * i));

        fix_num_t fix_b = fix_num_copy(fix_a);
        fix_b = fix_num_mul_sig(fix_b, sig_num_wrap((int64_t)1 - 2 * i));
        fix_b = fix_num_div_sig(fix_b, sig_num_wrap((int64_t)4 * i + 2));

        fix_res = fix_num_add(fix_res, fix_b);

        if(i%1000 == 0 && index_0 == 1)
            fprintf(stderr, "\n%lu / %lu", i / 1000, index_max / 1000);
    }
    fix_num_free(fix_a);

    args->fix_res = fix_res;
    return NULL;
}

fix_num_t pi_threads(uint64_t size, uint64_t thread_count)
{
    thread_pi_args_t args[thread_count];
    pthread_t tid[thread_count];

    uint64_t index_max = 32 * size + 4;
    uint64_t index[thread_count + 1];

    double_t tmp[thread_count + 1];
    for(uint64_t i=0; i<thread_count; i++)
    {
        double gamma = 0.5;
        double y = (double)i / thread_count;
        tmp[i] = 1 - pow(1.0 - y, gamma);
    }
    tmp[thread_count] = 1;

    for(uint64_t i=0; i<thread_count; i++)
        tmp[i] = tmp[i+1] - tmp[i];

    for(uint64_t i=0; i<thread_count/4; i++)
        tmp[i] *= 2 / (1 +  (double)i / thread_count);

    double total = 0;
    for(uint64_t i=0; i<thread_count; i++)
        total += tmp[i];

    double cum = 0;
    for(uint64_t i=0; i<=thread_count; i++)
    {
        double delta = tmp[i];
        tmp[i] = cum / total;
        cum += delta;
    }

    index[0] = 1;
    for(uint64_t i=1; i<thread_count; i++)
    {
        index[i] = index_max * tmp[i];
    }
    index[thread_count] = index_max;

    for(uint64_t i=0; i<thread_count; i++)
    {
        args[i] = (thread_pi_args_t)
        {
            .size = size,
            .index_0 = index[i],
            .index_max = index[i+1]
        };
        tid[i] = pthread_create_treat(thread_pi, &args[i]);
        pthread_lock(tid[i], 8 + i);
    }

    fix_num_t fix_pi = fix_num_wrap(3, size - 1);
    for(uint64_t i=0; i<thread_count; i++)
    {
        pthread_join_treat(tid[i]);
        fix_pi = fix_num_add(fix_pi, args[i].fix_res);
    }

    return fix_pi;
}
