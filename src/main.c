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

    fix_num_t pi = pi_threads(3);
    printf("\n\n");
    fix_num_display_dec(pi);

    printf("\n");
    return 0;
}
