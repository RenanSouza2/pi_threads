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



typedef int64_t (*element_f)(int64_t i);

STRUCT(thread_mul_sig_args)
{
    uint64_t size;
    uint64_t precision;
    uint64_t layer_count;
    uint64_t i_0, i_max;
    uint64_t id;
    element_f element;
    float_num_t flt;
    pthread_t *tid;
    bool volatile launched;
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

    element_f element = args->element;

    float_num_t flt = float_num_wrap(1, size);
    for(uint64_t i=i_0 + id; i<i_max; i+=4)
    {
        uint64_t index = 1 + layer_count * i;

        sig_num_t sig = sig_num_wrap(element(index));
        for(uint64_t k=1; k<layer_count; k++)
            sig = sig_num_mul(sig, sig_num_wrap(element(index + k)));

        flt = float_num_mul_sig(flt, sig);
    }

    while(!args->launched);

    args->flt = flt;
    if(id&1)
        return &args->flt;

    pthread_join_treat(tid[id+1]);
    flt = float_num_mul(flt, args[1].flt);
    args->flt = flt;
    if(id)
        return &args->flt;

    pthread_join_treat(tid[id+2]);
    flt = float_num_mul(flt, args[2].flt);
    args->flt = flt;  
    return &args->flt;
}

int64_t element_upper(int64_t index)
{
    return 2 * index - 3;
}

int64_t element_lower(int64_t index)
{
    return 8 * index;
}

fix_num_t a(
    uint64_t i_0,
    uint64_t size,
    uint64_t layer_count,
    uint64_t thread_0,
    uint64_t k
)
{
    thread_mul_sig_args_t args_upper[4];
    thread_mul_sig_args_t args_lower[4];

    pthread_t tid_upper[4];
    pthread_t tid_lower[4];
    
    for(uint64_t i=0; i<4; i++)
    {
        args_upper[i] = (thread_mul_sig_args_t)
        {
            .size = size - k,
            .layer_count = layer_count,
            .i_0 = 1,
            .i_max = i_0,
            .id = i,
            .element = element_upper,
            .tid = tid_upper
        };
        tid_upper[i] = pthread_create_treat(thread_mul_sig, &args_upper[i]);
        pthread_lock(tid_upper[i], thread_0 + i);

        args_lower[i] = (thread_mul_sig_args_t)
        {
            .size = size - k,
            .layer_count = layer_count,
            .i_0 = 1,
            .i_max = i_0,
            .id = i,
            .element = element_lower,
            .tid = tid_lower
        };
        tid_lower[i] = pthread_create_treat(thread_mul_sig, &args_lower[i]);
        pthread_lock(tid_lower[i], thread_0 + 4 + i);
    }

    for(uint64_t i=0; i<4; i++)
    {
        args_upper[i].launched = true;
        args_lower[i].launched = true;
    }

    pthread_join_treat(tid_upper[0]);
    float_num_t flt = float_num_mul_sig(args_upper[0].flt, sig_num_wrap(6));

    pthread_join_treat(tid_lower[0]);
    flt = float_num_div(flt, args_lower[0].flt);

    return fix_num_wrap_float(flt, size); 
}




fix_num_t jumpstart(uint64_t i_0, uint64_t size, uint64_t layer_count)
{
    float_num_t flt_1 = float_num_wrap(6, size);
    float_num_t flt_2 = float_num_wrap(1, size);
    // uint64_t tam = 1000;
    for(uint64_t i=1; i<i_0; i++)
    {
        uint64_t index = 1 + layer_count * i;

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
