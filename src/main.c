#include <stdio.h>

#include "../mods/clu/header.h"
#include "../mods/macros/assert.h"
#include "../mods/macros/time.h"
#include "../mods/number/lib/sig/header.h"
#include "../mods/number/lib/num/header.h"
#include "../mods/number/lib/num/struct.h"

#include "../lib/pi/header.h"



// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    // clu_log_enable(true);

    uint64_t size = 10000;
    uint64_t i=16;
    for(uint64_t j=1; j<=16; j++)
    {
        TIME_SETUP
        TIME_START
        fix_num_t fix_pi = pi_threads(size, i);
        TIME_END(t1)
        printf("\n%lu, %.2f, %.2f", i, t1 / 1e9, i * (t1 / 1e9));
        fix_num_free(fix_pi);
    }

    // uint64_t size = 40000;
    // uint64_t index_max = 32 * size + 4;
    // TIME_SETUP
    // for(uint64_t i=0; i<index_max; i+=1000)
    // {
    //     printf("\n%lu", i);
    //     TIME_START
    //     fix_num_t fix = jumpstart_ass_2(i, size);
    //     TIME_END(t1)
    //     fix_num_free(fix);
    //     printf(",\t %10.3f", t1 / 1e9);
    // }

    printf("\n");
    return 0;
}

// // no pool | div 1
// 16, 7.00, 112.06
// 16, 6.95, 111.21
// 16, 6.99, 111.86
// 16, 7.02, 112.30
// 16, 6.95, 111.12
// 16, 7.04, 112.72
// 16, 7.01, 112.19
// 16, 7.02, 112.33
// 16, 7.02, 112.28
// 16, 7.01, 112.19
// 16, 7.04, 112.72
// 16, 7.03, 112.47
// 16, 7.11, 113.77
// 16, 7.09, 113.50
// 16, 7.10, 113.62
// 16, 7.05, 112.84