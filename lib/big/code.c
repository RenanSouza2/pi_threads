#include <stdio.h>
#include <stdlib.h>
#include <stdbit.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/uint.h"

#include "../union/header.h"
#include "../split/header.h"



#ifdef DEBUG
#endif



#define PIECE_SIZE 20

bool file_validate(FILE *fp)
{
    // couldnt jump to end
    if(fseek(fp, 0, SEEK_END))
       return false;
    
    // file too short
    int64_t count = 5;
    int64_t size = ftell(fp);
    if(size < count)
        return false;
    
    // couldnt jump to position
    if(fseek(fp, size - count, SEEK_SET))
        return false;
    
    // couldnt read magic code
    uint64_t magic;
    if(fscanf(fp, "%lx", &magic) != 1)
        return false;

    // magic code wrong
    if(magic != 0xd0bbe)
        return false;

    //couldnt jump to begining
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
void sig_split(sig_num_t out[3], uint64_t i_0, uint64_t span)
{
    if(span == 0)
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

    sig_num_t res_1[3], res_2[3];
    sig_split(res_1, i_0              , span - 1);
    sig_split(res_2, i_0 + B(span - 1), span - 1);
    sig_split_join(out, res_1, res_2);
}

void sig_file_path_set(char file_path[100], uint64_t i_0, uint64_t span)
{
    snprintf(file_path, 100, "pieces/p_%010lx_%02lx.txt", i_0, span);
}

void sig_res_delete(uint64_t i_0, uint64_t span)
{
    char file_path[100];
    sig_file_path_set(file_path, i_0, span);
    remove(file_path);
}

void sig_res_save(sig_num_t res[], uint64_t i_0, uint64_t span)
{
    char file_path[100];
    sig_file_path_set(file_path, i_0, span);
    FILE *fp = fopen(file_path, "w");
    assert(fp);

    for(uint64_t i=0; i<3; i++)
    {
        sig_num_file_write(fp, res[i]);
        fprintf(fp, "\n");
    }

    fprintf(fp, " D0BBE");
    fclose(fp);

    sig_res_delete(i_0              , span - 1);
    sig_res_delete(i_0 + B(span - 1), span - 1);
}

bool sig_res_try_load(sig_num_t out[3], uint64_t i_0, uint64_t span)
{
    char file_path[100];
    sig_file_path_set(file_path, i_0, span);
    FILE *fp = file_try_open(file_path);
    if(fp == NULL)
        return false;

    for(uint64_t i=0; i<3; i++)
    {
        out[i] = sig_num_file_read(fp);
        assert(fscanf(fp, "\n") == 0);
    }

    fclose(fp);
    return true;
}



void union_res_from_sig(union_num_t out[3], sig_num_t res[3], uint64_t size)
{
    out[0] = union_num_wrap_sig(res[0], size);
    out[1] = union_num_wrap_sig(res[1], size);
    out[2] = union_num_wrap_sig(res[2], size);
}

bool sig_res_try_from_union(sig_num_t out[3], union_num_t res[3])
{
    if(
        res[0].type == FLT &&
        res[1].type == FLT &&
        res[2].type == FLT
    )
    {
        return false;
    }

    out[0] = res[0].num.sig;
    out[1] = res[1].num.sig;
    out[2] = res[2].num.sig;
    return true;
}

void union_file_path_set(char file_path[100], uint64_t size, uint64_t i_0, uint64_t span)
{
    uint64_t i_max = i_0 + span;
    snprintf(file_path, 100, "numbers/u_%010lx_%010lx_%02lx_%010lx.txt", size, i_0, span, i_max);
}

void union_res_delete(uint64_t size, uint64_t i_0, uint64_t span)
{
    char file_path[100];
    union_file_path_set(file_path, size, i_0, span);
    remove(file_path);
}

void union_res_save(union_num_t res[3], uint64_t size, uint64_t i_0, uint64_t span)
{
    dprintf("saving %lu %lu %lu", size, i_0, span);

    sig_num_t sig_res[3];
    if(sig_res_try_from_union(sig_res, res))
    {
        char file_path[100];
        sig_file_path_set(file_path, i_0, span);
        sig_res_save(sig_res, i_0, span);

        sig_res_delete(i_0              , span - 1);
        sig_res_delete(i_0 + B(span - 1), span - 1);
        return;
    }

    char file_path[100];
    union_file_path_set(file_path, size, i_0, span);
    FILE *fp = fopen(file_path, "w");
    assert(fp);

    for(uint64_t i=0; i<3; i++)
    {
        union_num_file_write(fp, res[i]);
        fprintf(fp, "\n");
    }

    fprintf(fp, " D0BBE");
    fclose(fp);

    union_res_delete(size, i_0              , span - 1);
    union_res_delete(size, i_0 + B(span - 1), span - 1);
}

bool union_res_try_load(union_num_t out[], uint64_t size, uint64_t i_0, uint64_t span)
{
    sig_num_t sig_res[3];
    if(sig_res_try_load(sig_res, i_0, span))
    {
        union_res_from_sig(out, sig_res, size);
        return true;
    }

    char file_path[100];
    union_file_path_set(file_path, size, i_0, span);
    FILE *fp = file_try_open(file_path);
    if(fp == NULL)
        return false;

    for(uint64_t i=0; i<3; i++)
    {
        out[i] = union_num_file_read(fp);
        assert(fscanf(fp, "\n") == 0);
    }
        
    fclose(fp);
    return true;
}

bool union_res_is_stored(uint64_t size, uint64_t i_0, uint64_t span)
{
    char file_path[100];

    sig_file_path_set(file_path, i_0, span);
    FILE *fp = file_try_open(file_path);
    if(fp)
    {
        fclose(fp);
        return true;
    }

    union_file_path_set(file_path, size, i_0, span);
    fp = file_try_open(file_path);
    if(fp)
    {
        fclose(fp);
        return true;
    }

    return false;
}

// out vector length 3, returns P, Q, R in that order
void union_split_big_span(union_num_t out[], uint64_t size, uint64_t i_0, uint64_t span)
{
    dprintf(" begin | %lx %lu", i_0, span);
    getchar();

    if(union_res_try_load(out, size, i_0, span))
    {
        dprintf("response already there");
        return;
    }

    if(span == 16)
    {
        dprintf("case span is 16")

        sig_num_t res[3];
        sig_split(res, i_0, span);
        sig_res_save(res, i_0, span);
        union_res_from_sig(out, res, size);
        return;
    }

    dprintf("response not there");
    // first half
    if(!union_res_is_stored(size, i_0, span - 1))
    {
        dprintf("first half not there")
        union_num_t res[3];
        union_split_big_span(res, size, i_0, span - 1);
        union_num_free(res[0]);
        union_num_free(res[1]);
        union_num_free(res[2]);
    }
    else
    {
        dprintf("first half was there");
    }

    //second half
    union_num_t res_2[3];
    union_split_big_span(res_2, size, i_0 + B(span - 1), span - 1);

    union_num_t res_1[3];

    dprintf("joining %lx %lu", i_0, span)
    assert(union_res_try_load(res_1, size, i_0, span - 1));
    split_join(out, res_1, res_2);

    union_res_save(out, size, i_0, span);
}



flt_num_t pi_big(uint64_t size)
{
    uint64_t index_max = 32 * size + 4;
    printf("\nindex-max: %lx", index_max);
    index_max = stdc_bit_ceil(index_max + B(16) - 1);
    printf("\nindex-max: %lx", index_max);

    union_num_t res[3];
    union_split_big_span(res, size, 1, stdc_bit_width(index_max));
    union_num_free(res[0]);

    flt_num_t flt_q = union_num_unwrap_flt(res[1]);
    flt_num_t flt_r = union_num_unwrap_flt(res[2]);
    
    flt_num_t flt_pi = flt_r;
    flt_pi = flt_num_mul_sig(flt_pi, sig_num_wrap(6));
    flt_pi = flt_num_div(flt_pi, flt_q);
    flt_pi = flt_num_add(flt_pi, flt_num_wrap(3, size));
    return flt_pi;
}
