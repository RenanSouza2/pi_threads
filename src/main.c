#include <stdio.h>
#include <time.h>

#include "../mods/clu/header.h"
#include "../mods/clu/header.h"
#include "../mods/macros/assert.h"
#include "../mods/number/lib/sig/header.h"
#include "../mods/number/lib/num/header.h"

#include "../lib/pi/header.h"
#include "../lib/jumpstart/header.h"



uint64_t get_time()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return time.tv_sec * (uint64_t)1e9 + time.tv_nsec;
}

#define TIME_START(TIME_VAR)                \
    uint64_t TIME_VAR;                      \
    {                                       \
        uint64_p _time_var = &TIME_VAR;     \
        uint64_t _time_begin = get_time();

#define TIME_END                                \
        uint64_t _time_end = get_time();        \
        *_time_var = _time_end - _time_begin;   \
    }

// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    uint64_t size = 20000;

    // for(uint64_t i=1; i<100; i++)
    // {
    //     printf("\n\n--------------");
    //     printf("\ni_0: %lu\n", i);
    //     fix_num_t fix_1 = jumpstart_standard(i, size, 3);
    //     fix_num_t fix_2 = jumpstart_thread(i, size, 3, 0, 0);

    //     printf("\n");fix_num_display_dec(fix_1);
    //     printf("\n");fix_num_display_dec(fix_2);

    //     fix_1 = fix_num_sub(fix_1, fix_2);
    //     uint64_t value = num_unwrap(fix_1.sig.num);
    //     printf("\nvalue: %lu", value);
    //     assert(value < 2);
    // }

    uint64_t layer_count = 3;
    uint64_t n_max = 32 * size + 4;
    uint64_t i_max = (n_max / layer_count) + 1;
    uint64_t i_mid = i_max / 2;

    // TIME_START(time_1)
    // jumpstart_standard(i_mid, size, 3);
    // TIME_END
    // printf("\ntime_standard: %.2f", time_1 / 1e9);
    
    // TIME_START(time_2)
    // jumpstart_current(i_mid, size, 3);
    // TIME_END
    // printf("\ntime_thread: %.2f", time_2 / 1e9);

    TIME_START(time_3)
    jumpstart_thread(i_mid, size, 3, 0, 0);
    TIME_END
    printf("\ntime_thread: %.2f", time_3 / 1e9);

    printf("\n");
    return 0;
}
