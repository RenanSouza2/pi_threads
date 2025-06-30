#include <stdio.h>

#include "../mods/clu/header.h"

#include "../lib/pi_first/header.h"

// int main(int argc, char** argv)
int main()
{
    setbuf(stdout, NULL);

    pi_threads_first(2, 100);

    printf("\n");
    return 0;
}
