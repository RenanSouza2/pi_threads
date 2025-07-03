#include <stdio.h>
#include <time.h>

#include "../mods/clu/header.h"
#include "../mods/clu/header.h"
#include "../mods/macros/assert.h"
#include "../mods/number/lib/sig/header.h"
#include "../mods/number/lib/num/header.h"
#include "../mods/number/lib/num/struct.h"

#include "../lib/pi/header.h"
#include "../lib/jumpstart/header.h"



uint64_t get_time()
{
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return time.tv_sec * (uint64_t)1e9 + time.tv_nsec;
}

#define TIME_SETUP uint64_t _time_begin, _time_end;

#define TIME_START _time_begin = get_time();

#define TIME_END(TIME_VAR)                          \
    _time_end = get_time();                         \
    uint64_t TIME_VAR = _time_end - _time_begin;    \

// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);
    TIME_SETUP

    uint64_t size = 2;

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
    uint64_t i = i_max / 2;

    // for(uint64_t i=3; i<i_max; i++)
    {
        printf("\n\ni: %lu", i);

        TIME_START
        fix_num_t fix_1 = jumpstart_standard(i, size, 3);
        TIME_END(time_1)
        printf("\ntime_standard\t: %.2f", time_1 / 1e9);

        TIME_START
        fix_num_t fix_2 = jumpstart_div_during(i, size, 3);
        TIME_END(time_2)
        printf("\ntime_div\t: %.2f", time_2 / 1e9);

        TIME_START
        fix_num_t fix_3 = jumpstart_ass_1(i, size, 3);
        TIME_END(time_3)
        printf("\ntime_ass\t: %.2f", time_3 / 1e9);

        TIME_START
        fix_num_t fix_4 = jumpstart_ass_2(i, size, 3);
        TIME_END(time_4)
        printf("\ntime_ass\t: %.2f", time_4 / 1e9);

        TIME_START
        fix_num_t fix_5 = jumpstart_thread(i, size, layer_count, 0);
        TIME_END(time_5)
        printf("\ntime_thread\t: %.2f", time_5 / 1e9);

        printf("\n\n");
        printf("\n");fix_num_display(fix_1);
        printf("\n");fix_num_display(fix_2);
        printf("\n");fix_num_display(fix_3);
        printf("\n");fix_num_display(fix_4);
        printf("\n");fix_num_display(fix_5);

        fix_2 = fix_num_sub(fix_num_copy(fix_1), fix_2);
        fix_3 = fix_num_sub(fix_num_copy(fix_1), fix_3);
        fix_4 = fix_num_sub(fix_num_copy(fix_1), fix_4);
        fix_5 = fix_num_sub(fix_num_copy(fix_1), fix_5);

        printf("\n\n");
        printf("\n");fix_num_display(fix_2);
        printf("\n");fix_num_display(fix_3);
        printf("\n");fix_num_display(fix_4);
        printf("\n");fix_num_display(fix_5);
    }

    printf("\n");
    return 0;
}
