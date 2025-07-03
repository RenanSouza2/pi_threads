#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/struct.h"
#include "../../mods/macros/U64.h"
#include "../../mods/number/lib/float/header.h"
#include "../../mods/number/lib/sig/header.h"

#include "../pear/header.h"



#ifdef DEBUG
#endif

#include <unistd.h>



int64_t element_upper(int64_t index)
{
    return 2 * index - 3;
}

int64_t element_lower(int64_t index)
{
    return index;
}


typedef int64_t (*element_f)(int64_t i);

STRUCT(thread_mul_sig_args)
{
    uint64_t size;
    uint64_t precision;
    uint64_t layer_count;
    uint64_t i_0, i_max;
    uint64_t id;
    float_num_t flt;
    pthread_t *tid;
    bool volatile launched;
    uint64_t split;
};

handler_p thread_mul_sig(handler_p _args)
{
    thread_mul_sig_args_p args = (thread_mul_sig_args_p)_args;
    
    uint64_t size = args->size;
    uint64_t layer_count = args->layer_count;
    uint64_t i_0 = args->i_0;
    uint64_t i_max = args->i_max;
    uint64_t id = args->id;
    pthread_t *tid = args->tid;
    uint64_t split = args->split;

    float_num_t flt = float_num_wrap(1, size);
    for(uint64_t i=i_0 + id; i<i_max; i+=split)
    {
        uint64_t index = 1 + layer_count * i;

        sig_num_t sig_1 = sig_num_wrap(element_upper(index));
        sig_num_t sig_2 = sig_num_wrap(element_lower(index));
        for(uint64_t k=1; k<layer_count; k++)
        {
            sig_1 = sig_num_mul(sig_1, sig_num_wrap(element_upper(index + k)));
            sig_2 = sig_num_mul(sig_2, sig_num_wrap(element_lower(index + k)));
        }

        flt = float_num_mul_sig(flt, sig_1);
    }

    while(!args->launched);

    for(uint64_t mask = 1; ((mask & id) == 0) && (mask < split); mask *= 2)
    { 
        pthread_join_treat(tid[id+mask]);
        flt = float_num_mul(flt, args[mask].flt);
    }

    args->flt = flt;  
    return &args->flt;
}

fix_num_t jumpstart_thread(
    uint64_t i_0,
    uint64_t size,
    uint64_t layer_count,
    uint64_t thread_0,
    uint64_t k
)
{
    assert(i_0%2 == 0);
    uint64_t split = 8;

    thread_mul_sig_args_t args[split];
    pthread_t tid[split];
    
    for(uint64_t i=0; i<split; i++)
    {
        args[i] = (thread_mul_sig_args_t)
        {
            .size = size - k,
            .layer_count = layer_count,
            .i_0 = 1,
            .i_max = i_0,
            .id = i,
            .tid = tid,
            .split = split
        };
        tid[i] = pthread_create_treat(thread_mul_sig, &args[i]);
        pthread_lock(tid[i], thread_0 + i);
    }

    for(uint64_t i=0; i<split; i++)
    {
        args[i].launched = true;
    }

    pthread_join_treat(tid[0]);
    float_num_t flt = float_num_mul_sig(args[0].flt, sig_num_wrap(6));
    flt = float_num_shr(flt, 3 * i_0 * layer_count);

    return fix_num_wrap_float(flt, size); 
}




fix_num_t jumpstart_standard(uint64_t i_0, uint64_t size, uint64_t layer_count)
{
    float_num_t flt_1 = float_num_wrap(6, size);
    float_num_t flt_2 = float_num_wrap(1, size);
    for(uint64_t i=1; i<i_0; i++)
    {
        uint64_t index = 1 + layer_count * (i - 1);

        sig_num_t sig_1 = sig_num_wrap((int64_t)2 * index - 3);
        sig_num_t sig_2 = sig_num_wrap((int64_t)8 * index);
        for(uint64_t k=1; k<layer_count; k++)
        {
            sig_1 = sig_num_mul(sig_1, sig_num_wrap((int64_t)2 * (index + k) - 3));
            sig_2 = sig_num_mul(sig_2, sig_num_wrap((int64_t)8 * (index + k)));
        }

        flt_1 = float_num_mul_sig(flt_1, sig_1);
        flt_2 = float_num_mul_sig(flt_2, sig_2);
    }
    flt_1 = float_num_div(flt_1, flt_2);
    return fix_num_wrap_float(flt_1, size);
}
