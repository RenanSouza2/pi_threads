#include <stdio.h>

#include "../mods/clu/header.h"
#include "../mods/macros/assert.h"
#include "../mods/macros/time.h"

#include "../lib/pi/header.h"
#include "../lib/linear/header.h"



// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    // uint64_t size = 10000;

    // TIME_SETUP
    // pi_v2(size);
    // TIME_END(t2)
    // printf("\nt2: %.2f", t2/1e9);

    for(uint64_t i=100; i <= 2000; i+=100)
    {
        TIME_SETUP
        pi_v3(i);
        TIME_END(t3)
        printf("\n%lu,  %.2f", i, t3/1e9);
    }

    printf("\n");
    return 0;
}
