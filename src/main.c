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

// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    uint64_t size = 165000000;
    // flt_num_t flt_pi = pi_big(size);
    // // printf("\n\n");flt_num_display_dec(flt_pi);
    // flt_num_free(flt_pi);

    // prepare(size, 20, 1950, 5035);
    // prepare(size, 25, 57, 157);
    prepare(size, 26, 30, 78);

    printf("\n");
    return 0;
}
