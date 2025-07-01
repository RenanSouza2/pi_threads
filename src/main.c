#include <stdio.h>

#include "../mods/clu/header.h"

#include "../lib/pi/header.h"

// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    // uint64_t size = 50000;
    // uint64_t i_0 = size * 4 / 15;
    // a_n(i_0, size, 3, 2);

    // clu_set_log(true);

    fix_num_t fix = pi_threads(2, 100);

    printf("\n\n");
    fix_num_display_dec(fix);

    printf("\n");
    return 0;
}
