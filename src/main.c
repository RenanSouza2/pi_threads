#include <stdio.h>

#include "../mods/clu/header.h"
#include "../mods/macros/assert.h"
#include "../mods/macros/time.h"

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

    uint64_t size = 40;

    // flt_num_t flt = flt_num_div(
    //     flt_num_wrap(1, size),
    //     flt_num_wrap(7, size)
    // );
    // union_num_t u = union_num_wrap_flt(flt, size);
    // union_num_save("numbers/del.txt", u);
    // union_num_free(u);
    
    union_num_t u2;
    bool valid = union_num_load("numbers/del.txt", &u2);
    printf("\nvalid: %d", valid);
    if(valid)
    {
        // printf("\n");union_num_display(u2);
        
        u2 = union_num_mul(u2,
            union_num_wrap_flt(
                flt_num_wrap(7, size), size
            )
        );


        printf("\n");
        // union_num_display_full("tag", u2);
        printf("\n");union_num_display(u2);
    }

    printf("\n");
    return 0;
}
