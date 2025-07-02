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

    uint64_t size = 50000;
    uint64_t layer_count = 3;

    uint64_t max = 32 * size + 4;
    uint64_t i_max = (max / layer_count) + 1;

    uint64_t begin = get_time();
    a(i_max / 2, size, layer_count, 8, 0);
    uint64_t end = get_time();
    uint64_t time_1 = end - begin;
    printf("\ntime 1: %.2f", time_1 / 1e9);

    begin = get_time();
    pi_threads(size);
    end = get_time();
    uint64_t time_2 = end - begin;
    printf("\ntime 2: %.2f", time_2 / 1e9);

    printf("\n");
    return 0;
}

// size 20000 | time 0m33.542s | 0m21.630s
// size 50000 | time 4m50.052s | 1m47.067s
