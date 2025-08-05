#include <stdio.h>

#include "../mods/clu/header.h"
#include "../mods/macros/assert.h"
#include "../mods/macros/time.h"
#include "../mods/number/lib/num/struct.h"

#include "../lib/big/header.h"
#include "../lib/pi/header.h"
#include "../lib/split/header.h"
#include "../lib/linear/header.h"
#include "../lib/union/header.h"



void time_1()
{
    for(uint64_t i=1000; i <= 65000; i+=1000)
    {
        printf("\n%lu", i);
        TIME_SETUP
        fxd_num_t flt = pi_v1(i);
        TIME_END(t3)
        printf("\t%.2f", t3/1e9);
        fxd_num_free(flt);
    }
}

void pre_call(uint64_t size, uint64_t depth, uint64_t i_0, uint64_t i_max)
{
    // printf("\n%lu %lu %lu", depth, i_0, i_max);
    if(i_max <= 960000003)
        return;

    if(i_0 >= 1200000004)
        return;

    if(depth == 8)
    {
        union_num_t res[3];
        // printf("\n%lu %lu %lu", depth, i_0, i_max);
        // getchar();
        split_big(res, size, depth, i_0, i_max);
        union_num_free(res[0]);
        union_num_free(res[1]);
        union_num_free(res[2]);
        return;
    }

    uint64_t i_half = (i_0 + i_max) / 2;
    pre_call(size, depth + 1, i_0       , i_half);
    pre_call(size, depth + 1, i_half + 1, i_max);
}

int main(int argc, char** argv)
// int main()
{
    setbuf(stdout, NULL);

    assert(argc > 2);
    uint64_t size = atol(argv[1]);

    // flt_num_t flt = pi_big(size);
    // flt_num_display_dec(flt);
    // flt_num_free(flt);

    // uint64_t depth = 4;
    // uint64_t i_0 = 1800000005;
    // uint64_t i_max = 1920000004;

    uint64_t depth = 0;
    uint64_t i_0 = 1;
    uint64_t i_max = 1920000004;
    pre_call(size, depth, i_0, i_max);
    
    // union_num_t res[3];
    // printf("\n%lu %lu %lu", depth, i_0, i_max);
    // split_big(res, size, depth, i_0, i_max);

    printf("\n");
    return 0;
}
