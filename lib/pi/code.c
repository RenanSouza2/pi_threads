#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/time.h"
#include "../../mods/macros/U64.h"
#include "../../mods/number/lib/fix/header.h"
#include "../../mods/number/lib/sig/header.h"
#include "../../mods/number/lib/num/struct.h"

#include "../jumpstart/header.h"
#include "../junc/header.h"
#include "../queue/header.h"
#include "../pear/header.h"



#ifdef DEBUG
#endif



STRUCT(thread_pi_args)
{
    uint64_t size;
    uint64_t id;
    uint64_t thread_count;
    fix_num_t fix_res;
};

handler_p thread_pi(handler_p _args)
{
    thread_pi_args_p args = (thread_pi_args_p)_args;

    uint64_t size = args->size;
    uint64_t id = args->id;
    uint64_t thread_count = args->thread_count;

    uint64_t index_max_tot = 32 * size + 4;
    uint64_t index_0 = id ?  id * index_max_tot / thread_count : 1;
    uint64_t index_max = (id + 1) * index_max_tot / thread_count;

    // printf("\nid: %lu\tmin: %lu", id, index_0);

    TIME_SETUP
    TIME_START

    fix_num_t fix_a = jumpstart_ass_2(index_0 - 1, size, 3);
    fix_num_t fix_res = fix_num_wrap(0, size - 1);
    for(uint64_t i=index_0; i<index_max; i++)
    {
        if(i%500 == 0)
        {
            TIME_END(t1);
            printf("\n%7lu,\t\t%.3f", i, t1 / 1e9);
            // printf("\n%.3f\t\t%7lu", t1 / 1e9, i);
        }

        fix_a = fix_num_mul_sig(fix_a, sig_num_wrap((int64_t)2 * i - 3));
        fix_a = fix_num_div_sig(fix_a, sig_num_wrap((int64_t)8 * i));

        fix_num_t fix_b = fix_num_copy(fix_a);
        fix_b = fix_num_mul_sig(fix_b, sig_num_wrap((int64_t)1 - 2 * i));
        fix_b = fix_num_div_sig(fix_b, sig_num_wrap((int64_t)4 * i + 2));

        fix_res = fix_num_add(fix_res, fix_b);
    }
    fix_num_free(fix_a);

    args->fix_res = fix_res;
    return NULL;
}

fix_num_t pi_threads(uint64_t size)
{
    uint64_t thread_count = 16;
    thread_pi_args_t args[thread_count];
    pthread_t tid[thread_count];

    for(uint64_t i=0; i<thread_count; i++)
    {
        args[i] = (thread_pi_args_t)
        {
            .size = size,
            .id = i,
            .thread_count = thread_count
        };
        tid[i] = pthread_create_treat(thread_pi, &args[i]);
        pthread_lock(tid[i], i);
    }

    fix_num_t fix_pi = fix_num_wrap(3, size - 1);
    for(uint64_t i=0; i<thread_count; i++)
    {
        pthread_join_treat(tid[i]);
        fix_pi = fix_num_add(fix_pi, args[i].fix_res);
    }

    return fix_pi;
}
