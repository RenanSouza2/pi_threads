#include <stdio.h>
#include <time.h>

#include "../mods/clu/header.h"
#include "../mods/clu/header.h"
#include "../mods/macros/assert.h"
#include "../mods/number/lib/sig/header.h"

#include "../lib/pi/header.h"
#include "../lib/jumpstart/header.h"



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

    // // fix_num_t fix = pi_threads(20000);
    // // printf("\n\n");
    // // // fix_num_display_dec(fix);
    // // fix_num_free(fix);

    // uint64_t size = 20000;
    
    // uint64_t layer_count = 1;
    // uint64_t max = 32 * size + 4;
    // uint64_t i_max = (max / layer_count) + 1;
    // i_max += i_max & 1;
    // printf("\ni_max: %lu", i_max / 2);

    // uint64_t begin = get_time();
    // fix_num_t fix_1 = jumpstart_thread(i_max / 2, size, layer_count, 0, 0);
    // uint64_t end = get_time();
    // uint64_t time_1 = end - begin;
    // printf("\ntime 1: %.2f", time_1 / 1e9);

    // begin = get_time();
    // fix_num_t fix_2 = jumpstart_standard(i_max / 2, size, layer_count);
    // end = get_time();
    // uint64_t time_2 = end - begin;
    // printf("\ntime 2: %.2f", time_2 / 1e9);
    
    // assert(fix_num_cmp(fix_1, fix_2));

    printf("\n\n");
    fix_num_t pi = pi_threads(1000);
    fix_num_display_dec(pi);

    printf("\n");
    return 0;
}

// size 20000 | time 0m33.542s | 0m21.630s
// size 50000 | time 4m50.052s | 1m47.067s
