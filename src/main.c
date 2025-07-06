#include <stdio.h>

#include "../mods/clu/header.h"
#include "../mods/clu/header.h"
#include "../mods/macros/assert.h"
#include "../mods/macros/time.h"
#include "../mods/number/lib/sig/header.h"
#include "../mods/number/lib/num/header.h"
#include "../mods/number/lib/num/struct.h"

#include "../lib/pi/header.h"
#include "../lib/jumpstart/header.h"



// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    // uint64_t size = 50000;
    // TIME_SETUP_1
    // TIME_START_1
    // fix_num_t fix_pi = pi_threads(size);
    // TIME_END_1(t1)
    // printf("\ntime: %.2f", t1 / 1e9);
    // fix_num_free(fix_pi);

    uint64_t size = 10000;
    uint64_t index_max = 32 * size + 4;
    TIME_SETUP
    for(uint64_t i=0; i<index_max; i+=1000)
    {
        printf("\n%lu", i);
        TIME_START
        fix_num_t fix = jumpstart_ass_2(i, size);
        TIME_END(t1)
        fix_num_free(fix);
        printf(",\t %10.3f", t1 / 1e9);
    }

    printf("\n");
    return 0;
}
