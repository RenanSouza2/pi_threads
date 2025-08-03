#include <stdio.h>

#include "../mods/clu/header.h"
#include "../mods/macros/assert.h"
#include "../mods/macros/time.h"

#include "../lib/big/header.h"
#include "../lib/pi/header.h"
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

    uint64_t size = 5000;
    flt_num_t flt = pi_big(size);
    // flt_num_display_dec(flt);

    printf("\nmax: %lu", clu_get_max_occupancy());

    printf("\n");
    return 0;
}
