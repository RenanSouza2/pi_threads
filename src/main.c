#include <stdio.h>
#include <time.h>

#include "../mods/clu/header.h"

#include "../lib/pi/header.h"
#include "../lib/jumpstart/header.h"


#include "../mods/number/lib/sig/header.h"

uint64_t get_time()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return time.tv_sec * (uint64_t)1e9 + time.tv_nsec;
}

// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    // fix_num_t fix = pi_threads(20000);
    // printf("\n\n");
    // // fix_num_display_dec(fix);
    // fix_num_free(fix);

    uint64_t size = 2;
    uint64_t max = 32 * size + 4;
    uint64_t layers_count = 3;
    uint64_t i_max = (max / layers_count) + 1;
    // uint64_t mid = max / 2;
    // uint64_t k = (mid / 32) - 1;
    fix_num_t flt_2 = a(i_max / 2, size, layers_count, 8, 0);
    // fix_num_t flt_b = pi_threads(size);
    // printf("\na\t");fix_num_display(flt_1);
    printf("\na\t");fix_num_display(flt_2);
    // printf("\nb\t");fix_num_display(flt_b);

    printf("\n");
    return 0;
}

// size 20000 | time 0m33.542s | 0m21.630s
// size 50000 | time 4m50.052s | 1m47.067s
