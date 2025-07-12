#include <stdio.h>

#include "../mods/clu/header.h"
// #include "../mods/macros/assert.h"
#include "../mods/macros/time.h"

#include "../lib/pi/header.h"
#include "../lib/linear/header.h"



// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    uint64_t size = 100;

    TIME_SETUP
    fix_num_t fix_pi_1 = pi_v1(size);
    TIME_END(t1)

    fix_num_t fix_pi_2 = pi_v2(size);
    TIME_END(t2)

    // printf("\n\n");
    // fix_num_display_dec(fix_pi_1);
    fix_num_free(fix_pi_1);
    fix_num_free(fix_pi_2);

    printf("\ntime_1: %.2f", t1/1e9);
    printf("\ntime_2: %.2f", t2/1e9);

    printf("\n");
    return 0;
}
