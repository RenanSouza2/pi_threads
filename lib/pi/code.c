#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
// #include "../../mods/macros/time.h"
#include "../../mods/macros/uint.h"
#include "../../mods/number/lib/sig/header.h"

#include "../pear/header.h"
#include "../linear/header.h"



#ifdef DEBUG
#endif



fix_num_t jumpstart(uint64_t index_0, uint64_t size)
{
    uint64_t layer_count = 3;

    if(index_0 == 0)
        return fix_num_wrap(6, size - 1);

    assert(index_0 > 3);
    uint64_t index_max = index_0 / 2;
    uint64_t i_max = (index_max + layer_count - 2) / layer_count;
    uint64_t delta = (index_0 + 1) / 2;
    uint64_t pos = size < index_0 / 32 + 2 ? 2 : size - index_0 / 32;

    float_num_t flt_1 = float_num_wrap(-6, pos);
    float_num_t flt_2 = float_num_wrap( 1, pos);
    for(uint64_t i=0; i<i_max; i++)
    {
        uint64_t index = 2 + layer_count * i;

        sig_num_t sig_1 = sig_num_wrap(2 * (index + delta) - 3);
        sig_num_t sig_2 = sig_num_wrap(index);
        for(uint64_t k=1; (k<layer_count) && index + k <= index_0; k++)
        {
            sig_1 = sig_num_mul(sig_1, sig_num_wrap(2 * (index + k + delta) - 3));
            sig_2 = sig_num_mul(sig_2, sig_num_wrap(index + k));
        }

        flt_1 = float_num_mul_sig(flt_1, sig_1);
        flt_2 = float_num_mul_sig(flt_2, sig_2);
    }
    flt_1 = float_num_shr(flt_1, 7 * index_0 / 2);
    flt_1 = float_num_div(flt_1, flt_2);
    return fix_num_wrap_float(flt_1, size);
}



STRUCT(thread_pi_args)
{
    uint64_t id, thread_count;
    uint64_t index_0, index_max;
    uint64_t size;
    pthread_t *tid;
    bool launched;
    union_num_t res[3];
};

handler_p thread_pi(handler_p _args)
{
    thread_pi_args_p args = (thread_pi_args_p)_args;

    uint64_t id = args->id;
    uint64_t thread_count = args->thread_count;
    uint64_t index_0 = args->index_0;
    uint64_t index_max = args->index_max;
    uint64_t size = args->size;
    union_num_t res[3];

    // printf("\nargs: %lu %lu %lu", index_0, index_max, size);
    binary_splitting(res, size, index_0, index_max, 0, id ? 0 : index_max);

    while(!args->launched);
    for(uint64_t mask = 1; (mask < thread_count) && ((mask & id) == 0); mask *= 2)
    {
        pthread_join_treat(args->tid[id + mask]);
        binary_splitting_join(res, res, args[mask].res);
    }

    args->res[0] = res[0];
    args->res[1] = res[1];
    args->res[2] = res[2];
    return NULL;
}

void split_work(uint64_t index[], uint64_t size, uint64_t thread_count)
{
    uint64_t index_max = 32 * size + 4;

    double_t tmp[thread_count + 1];
    for(uint64_t i=0; i<thread_count; i++)
    {
        double y = (double)i / thread_count;
        tmp[i] = 1 - sqrt(1.0 - y);
    }
    tmp[thread_count] = 1;

    for(uint64_t i=0; i<thread_count; i++)
        tmp[i] = tmp[i+1] - tmp[i];

    for(uint64_t i=0; i<thread_count/4; i++)
        tmp[i] *= 2 / (1 + (double)i / thread_count);

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
}

float_num_t pi_threads(uint64_t size, uint64_t thread_count, uint64_t thread_0)
{
    uint64_t index_max = 32 * size + 4;
    uint64_t index[thread_count + 1];
    index[0] = 0;
    index[thread_count] = index_max;
    for(uint64_t i=1; i<thread_count; i++)
    {
        index[i] = index_max * i / thread_count;
    }
    // for(uint64_t i=0;  i<=thread_count; i++)
    //     printf("\nindex[%lu]: %lu", i, index[i]);

    thread_pi_args_t args[thread_count];
    pthread_t tid[thread_count];
    for(uint64_t i=0;i<thread_count; i++)
    {
        args[i] = (thread_pi_args_t)
        {
            .id = i,
            .thread_count = thread_count,
            .index_0 = index[i] + 1,
            .index_max = index[i + 1],
            .size = size,
            .tid = tid
        };
        tid[i] = pthread_create_treat(thread_pi, &args[i]);
        pthread_lock(tid[i], thread_0 + i);
    }

    for(uint64_t i=0;i<thread_count; i++)
        args[i].launched = true;

    pthread_join_treat(tid[0]);
    union_num_t *res = args[0].res;
    union_num_free(res[0]);

    float_num_t flt_q = union_num_unwrap_float(res[1]);
    float_num_t flt_r = union_num_unwrap_float(res[2]);

    float_num_t flt_pi = flt_r;
    flt_pi = float_num_mul_sig(flt_pi, sig_num_wrap(6));
    flt_pi = float_num_div(flt_pi, flt_q);
    flt_pi = float_num_add(flt_pi, float_num_wrap(3, size));
    return flt_pi;
}
