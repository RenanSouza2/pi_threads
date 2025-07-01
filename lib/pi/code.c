#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/U64.h"
#include "../../mods/number/lib/fix/header.h"
#include "../../mods/number/lib/sig/header.h"
#include "../../mods/number/lib/num/struct.h"

#include "../junc/header.h"
#include "../queue/header.h"
#include "../pear/header.h"



#ifdef DEBUG
#endif



STRUCT(thread_a_args)
{
    uint64_t layer_count;
    uint64_t batch_size;
    uint64_t i_0, i_max;

    uint64_t id;
    queue_p queue_a_b;
    fix_num_t a0;
};

handler_p thread_a(handler_p _args)
{
    thread_a_args_p args = _args;

    uint64_t layer_count = args->layer_count;
    uint64_t batch_size = args->batch_size;
    uint64_t i_0 = args->i_0;
    uint64_t i_max = args->i_max;

    uint64_t id = args->id;
    queue_p queue_a_b = args->queue_a_b;
    fix_num_t fix_a = args->a0;
    fix_num_t fix_a_batch[batch_size];
    for(uint64_t i=i_0; i<i_max; i++)
    {
        for(uint64_t j=0; j<batch_size; j++)
        {
            uint64_t index = id + layer_count * (1 + j + batch_size * i);

            sig_num_t sig_1 = sig_num_wrap((int64_t)2 * index - 3);
            sig_num_t sig_2 = sig_num_wrap((int64_t)index);
            for(uint64_t k=1; k<layer_count; k++)
            {
                sig_1 = sig_num_mul(sig_1, sig_num_wrap((int64_t)2 * (index - k) - 3));
                sig_2 = sig_num_mul(sig_2, sig_num_wrap((int64_t)(index - k)));
            }

            fix_a = fix_num_mul_sig(fix_a, sig_1);
            fix_a = fix_num_shr(fix_a, 3 * layer_count);
            fix_a = fix_num_div_sig(fix_a, sig_2);

            fix_a_batch[j] = fix_num_copy(fix_a);
        }

        queue_send(queue_a_b, &fix_a_batch, NULL);
    }
    fix_num_free(fix_a);

    return NULL;
}

STRUCT(thread_b_args)
{
    uint64_t layer_count;
    uint64_t batch_size;
    uint64_t i_0, i_max;

    uint64_t id;
    queue_p queue_a_b;
    queue_p queue_b_c;
};

handler_p thread_b(handler_p _args)
{
    thread_b_args_p args = _args;

    uint64_t layer_count = args->layer_count;
    uint64_t batch_size = args->batch_size;
    uint64_t i_0 = args->i_0;
    uint64_t i_max = args->i_max;

    uint64_t id = args->id;
    queue_p queue_a_b = args->queue_a_b;
    queue_p queue_b_c = args->queue_b_c;

    fix_num_t fix_a_batch[batch_size], fix_b_batch[batch_size];
    for(uint64_t i=i_0; i<i_max; i++)
    {
        queue_recv(queue_a_b, &fix_a_batch, NULL);

        for(uint64_t j=0; j<batch_size; j++)
        {
            uint64_t index = id + layer_count * (1 + j + batch_size * i);

            fix_num_t fix_b = fix_num_mul_sig(fix_a_batch[j], sig_num_wrap((int64_t)1 - 2 * index));
            fix_b = fix_num_div_sig(fix_b, sig_num_wrap((int64_t)4 * index + 2));
            fix_b_batch[j] = fix_b;
        }

        queue_send(queue_b_c, &fix_b_batch, NULL);
    }

    return NULL;
}

STRUCT(thread_c_args)
{
    uint64_t num_size;
    uint64_t layer_count;
    uint64_t batch_size;
    uint64_t i_0, i_max;

    junc_p junc_b_c;
    fix_num_t res;
};

handler_p thread_c(handler_p _args)
{
    thread_c_args_p args = _args;

    uint64_t num_size = args->num_size;
    uint64_t layer_count = args->layer_count;
    uint64_t batch_size = args->batch_size;
    uint64_t i_0 = args->i_0;
    uint64_t i_max = args->i_max;

    junc_p junc_b_c = args->junc_b_c;
    
    fix_num_t fix_b_batch[batch_size];
    fix_num_t fix_c = fix_num_wrap(0, num_size);
    for(uint64_t i=i_0; i<i_max; i++)
    {
        for(uint64_t j=0; j<layer_count; j++)
        {
            junc_get(junc_b_c, &fix_b_batch,NULL);
            for(uint64_t k=0; k<batch_size; k++)
                fix_c = fix_num_add(fix_c, fix_b_batch[k]);
        }
    }

    args->res = fix_c;
    return NULL;
}



float_num_t a_n(uint64_t i_0, uint64_t size, uint64_t layer_count, uint64_t batch_size)
{
    float_num_t flt_1 = float_num_wrap(6, size);
    float_num_t flt_2 = float_num_wrap(1, size);
    uint64_t tam = 1000;
    for(uint64_t i=1; i<i_0; i++)
    {
        if(i%tam == 0)printf("\ni: %lu / %lu", i / tam, i_0 / tam);
        for(uint64_t j=0; j<batch_size; j++)
        {
            uint64_t index = (i * batch_size + j) * layer_count;

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
    }
    return float_num_div(flt_1, flt_2);
}



void pi_queue_res_free(handler_p h, uint64_t res_size)
{
    uint64_t batch_size = res_size / sizeof(fix_num_t);
    for(uint64_t i=0; i<batch_size; i++)
        fix_num_free(((fix_num_p)h)[i]);
}

STRUCT(layer)
{
    queue_t queue_a_b;

    thread_a_args_t args_a;
    thread_b_args_t args_b;
    
    pthread_t tid_a;
    pthread_t tid_b;

    fix_num_t a0;
};

STRUCT(group)
{
    uint64_t layer_count;
    layer_p layers;

    junc_t junc_b_c;
    thread_c_args_t args_c;
    pthread_t tid_c;
};

group_p group_create(uint64_t layer_count)
{
    layer_p layers = malloc(sizeof(layer_t));

    group_p g = malloc(sizeof(group_t));
    *g = (group_t)
    {
        .layer_count = layer_count,
        .layers = layers,
    };
    return g;
}

void group_free(group_p g)
{
    free(g->layers);
    free(g);
}

group_p group_launch(uint64_t size, uint64_t i_0, uint64_t i_max)
{
    uint64_t layer_count = 3;
    uint64_t queue_size = 5;
    uint64_t batch_size = 20;
    uint64_t thread_0 = 0;

    group_p g = group_create(layer_count);
    g->junc_b_c = junc_init(
        layer_count,
        queue_size,
        batch_size * sizeof(fix_num_t),
        pi_queue_res_free
    );
    
    fix_num_t fix_a = fix_num_wrap(6, size - 1);
    g->layers[0].a0 = fix_num_copy(fix_a);
    for(uint64_t i=1; i<layer_count; i++)
    {
        fix_a = fix_num_mul_sig(fix_a, sig_num_wrap((int64_t)2 * i - 3));
        fix_a = fix_num_div_sig(fix_a, sig_num_wrap((int64_t)8 * i));
        g->layers[i].a0 = fix_num_copy(fix_a);

        fix_num_t fix_b = fix_num_copy(fix_a);
        fix_b = fix_num_mul_sig(fix_b, sig_num_wrap((int64_t)1 - 2 * i));
        fix_b = fix_num_div_sig(fix_b, sig_num_wrap((int64_t)4 * i + 2));
    }
    fix_num_free(fix_a);

    for(uint64_t i=0; i<layer_count; i++)
    {
        g->layers[i].queue_a_b = queue_init(queue_size, batch_size * sizeof(fix_num_t), pi_queue_res_free);
        g->layers[i].args_a = (thread_a_args_t)
        {
            .layer_count = layer_count,
            .batch_size = batch_size,
            .i_0 = i_0,
            .i_max = i_max,

            .id = i,
            .queue_a_b = &g->layers[i].queue_a_b,
        };
        g->layers[i].tid_a = pthread_create_treat(thread_a, &g->layers[i].args_a);
        pthread_lock(g->layers[i].tid_a, thread_0 + 2 * i);

        g->layers[i].args_b = (thread_b_args_t)
        {
            .layer_count = layer_count,
            .batch_size = batch_size,
            .i_0 = i_0,
            .i_max = i_max,

            .id = i,
            .queue_a_b = &g->layers[i].queue_a_b,
            .queue_b_c = &g->junc_b_c.queues[i]
        };
        g->layers[i].tid_b = pthread_create_treat(thread_b, &g->layers[i].args_b);
        pthread_lock(g->layers[i].tid_b, thread_0 + 2 * i + 1);
    }

    thread_c_args_t args_pi = (thread_c_args_t)
    {
        .num_size = size,
        .layer_count = layer_count,
        .batch_size = batch_size,
        .i_0 = i_0,
        .i_max = i_max,

        .junc_b_c = &g->junc_b_c
    };
    g->tid_c = pthread_create_treat(thread_c, &args_pi);
    pthread_lock(g->tid_c, thread_0 + 6);
    return g;
}

fix_num_t group_join(group_p g)
{
    pthread_join_treat(g->tid_c);
    for(uint64_t i=0; i<g->layer_count; i++)
    {
        assert(queue_get_occupancy(&g->layers[i].queue_a_b) == 0);
        assert(queue_get_occupancy(&g->junc_b_c.queues[i]) == 0);

        pthread_join_treat(g->layers[i].tid_a);
        pthread_join_treat(g->layers[i].tid_b);
        
        queue_free(&g->layers[i].queue_a_b);
    }
    junc_free(&g->junc_b_c);

    
    fix_num_t fix_c = g->args_c.res;
    group_free(g);
    return fix_c;
}
