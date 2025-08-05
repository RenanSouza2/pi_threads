#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/uint.h"

#include "../union/header.h"
#include "../split/header.h"



#ifdef DEBUG
#endif



bool file_validate(FILE *fp)
{
    int64_t count = 5;
    if(fseek(fp, 0, SEEK_END))
        return false;
    int64_t size = ftell(fp);
    if(size < count) return 3;
    if(fseek(fp, size - count, SEEK_SET))
        return false;
    
    uint64_t magic;
    if(fscanf(fp, "%lx", &magic) != 1)
        return false;
    if(magic != 0xd0bbe) return 6;
    if(fseek(fp, 0, SEEK_SET))
        return false;

    return true;
}

FILE* file_try_open(char file_path[100])
{
    FILE *fp = fopen(file_path, "r");
    if(fp == NULL)
        return NULL;

    if(file_validate(fp))
        return fp;

    fclose(fp);
    return NULL;
}

void sig_split_join(
    sig_num_t out[],
    sig_num_t res_1[3],
    sig_num_t res_2[3]
)
{
    sig_num_t sig_r_1 = sig_num_mul(res_1[2], sig_num_copy(res_2[1]));
    sig_num_t sig_r_2 = sig_num_mul(res_2[2], sig_num_copy(res_1[0]));
    
    sig_num_t out_0 = sig_num_mul(res_1[0], res_2[0]);
    sig_num_t out_1 = sig_num_mul(res_1[1], res_2[1]);
    sig_num_t out_2 = sig_num_add(sig_r_1, sig_r_2);

    out[0] = out_0;
    out[1] = out_1;
    out[2] = out_2;
}

// out vector length 3, returns P, Q, R in that order
void sig_split(sig_num_t out[], uint64_t i_0, uint64_t i_max)
{
    assert(i_0 <= i_max);
    if(i_0 == i_max)
    {
        int128_t p = (int128_t)2 * i_0 - 3;
        int128_t q = (int128_t)8 * i_0;
        int128_t u = (int128_t)1 - 2 * i_0;
        int128_t v = (int128_t)4 * i_0 + 2;

        out[0] = sig_num_wrap_int128(p * v);
        out[1] = sig_num_wrap_int128(q * v);
        out[2] = sig_num_wrap_int128(p * u);

        return;
    }

    uint64_t i_half = (i_0 + i_max) / 2;
    union_num_t res_1[3], res_2[3];
    sig_split(res_1, i_0       , i_half);
    sig_split(res_2, i_half + 1, i_max );
    sig_split_join(out, res_1, res_2);
}

void piece_file_path_set(char file_path[100], uint64_t id)
{
    snprintf(file_path, 100, "pieces/p_%060.txt");
}

void res_save(
    sig_num_t res[],
    uint64_t size,
    uint64_t depth,
    uint64_t i_0,
    uint64_t i_max
)
{
    char file_path[100];
    file_path_set(file_path, size, depth, i_0, i_max);
    FILE *fp = fopen(file_path, "w");
    assert(fp);

    for(uint64_t i=0; i<3; i++)
        union_num_file_write(fp, res[i]);

    fprintf(fp, " D0BBE");
    fclose(fp);
}

void piece_generate(uint64_t id)
{
    char file_path[100];
    piece_file_path_set(file_path, id);
    FILE* fp = file_try_open(file_path);
    if(fp)
    {
        fclose(fp);
        return;
    }

    sig_num_t res[3];
    sig_split(res, B(15) * id + 1, B(15) * (id + 1));
}



bool res_load(
    union_num_t res[],
    uint64_t size,
    uint64_t depth,
    uint64_t i_0,
    uint64_t i_max
)
{
    FILE *fp = file_try_open(size, depth, i_0, i_max);
    if(fp == NULL)
        return false;

    for(uint64_t i=0; i<3; i++)
        union_num_file_read(fp, &res[i]);

    fclose(fp);
    return true;
}

void res_delete(uint64_t size, uint64_t depth, uint64_t i_0, uint64_t i_max)
{
    char file_path[100];
    file_path_set(file_path, size, depth, i_0, i_max);
    remove(file_path);
}



// out vector length 3, returns P, Q, R in that order
void split_big(
    union_num_t out[],
    uint64_t size,
    uint64_t depth,
    uint64_t i_0,
    uint64_t i_max
)
{
    // if(i_max - i_0 < 1024)
    if(i_max - i_0 < 65536)
    {
        split(out, size, i_0, i_max);
        res_save(out, size, depth, i_0, i_max);
        return;
    }

    // printf("\nTAG | %lu %lu %lu", depth, i_0, i_max);
    if(res_load(out, size, depth, i_0, i_max))
        return;

    uint64_t i_half = (i_0 + i_max) / 2;

    union_num_t res_1[3];
    FILE *fp = file_try_open(size, depth + 1, i_0, i_half);
    if(fp)
    {
        fclose(fp);
    }
    else
    {
        split_big(res_1, size, depth + 1, i_0, i_half);
        union_num_free(res_1[0]);
        union_num_free(res_1[1]);
        union_num_free(res_1[2]);
    }

    union_num_t res_2[3];
    split_big(res_2, size, depth + 1, i_half + 1, i_max);
    
    res_load(res_1, size, depth + 1, i_0, i_half);

    fprintf(stderr, "\n%lu %lu %lu", depth, i_0, i_max);
    split_join(out, res_1, res_2);

    res_save(out, size, depth, i_0, i_max);
    res_delete(size, depth + 1, i_0       , i_half);
    res_delete(size, depth + 1, i_half + 1, i_max);
}

flt_num_t pi_big(uint64_t size)
{
    uint64_t index_max = 32 * size + 4;

    union_num_t res[3];
    split_big(res, size, 0, 1, index_max);
    union_num_free(res[0]);

    flt_num_t flt_q = union_num_unwrap_flt(res[1]);
    flt_num_t flt_r = union_num_unwrap_flt(res[2]);
    
    flt_num_t flt_pi = flt_r;
    flt_pi = flt_num_mul_sig(flt_pi, sig_num_wrap(6));
    flt_pi = flt_num_div(flt_pi, flt_q);
    flt_pi = flt_num_add(flt_pi, flt_num_wrap(3, size));
    return flt_pi;
}
