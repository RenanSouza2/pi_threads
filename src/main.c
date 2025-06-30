#include <stdio.h>

#include "../mods/clu/header.h"

#include "../lib/pi_first/header.h"
#include "../lib/pi_second/header.h"

// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    // pi_threads_first(2, 100);

    uint64_t size = 10000;
    uint64_t a = size * 2;
    base(size, a);

    printf("\n");
    return 0;
}
