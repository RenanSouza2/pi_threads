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

    uint64_t size = 50000;

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

    // uint64_t layer_count = 4;

    printf("\n| layer\t|\t\tsplit\t\t|");
    printf("\n|\t|--------------------------------");
    printf("\n|\t| ");
    for(uint64_t j=1; j <=8; j *= 2)
        printf("%5lu\t| ", j);
    printf("\n-----------------------------------------");

    for(uint64_t i=1; i<20; i++)
    {
        uint64_t n_max = 32 * size + 4;
        uint64_t i_max = (n_max / i) + 1;
        uint64_t i_0 = i_max / 2;

        printf("\n| %5lu\t|", i);
        for(uint64_t j=1; j <=8; j *= 2)
        {
            TIME_START
            jumpstart_thread(i_0, size, i, 0, j);
            TIME_END(time_5)
            printf(" %5.1f\t|", time_5 / 1e9);
        }
    }

    // uint64_t i=3;

    // uint64_t n_max = 32 * size + 4;
    // uint64_t i_max = (n_max / i) + 1;
    // uint64_t i_0 = i_max / 2;
    
    // TIME_START
    // jumpstart_ass_2(i_0, size, i);
    // TIME_END(time_5)
    // printf(" %5.1f\t|", time_5 / 1e9);

    printf("\n");
    return 0;
}
