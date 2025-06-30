#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "../../mods/clu/header.h"
#include "../../mods/macros/assert.h"
#include "../../mods/macros/U64.h"
#include "../../mods/number/lib/fix/header.h"
#include "../../mods/number/lib/float/header.h"
#include "../../mods/number/lib/sig/header.h"
#include "../../mods/number/lib/num/struct.h"

#include "../junc/header.h"
#include "../queue/header.h"
#include "../pear/header.h"



#ifdef DEBUG
#endif


uint64_t base(uint64_t size, uint64_t max)
{
    float_num_t flt = float_num_wrap(6, size);
    for(uint64_t i=1; i<max/3; i++)
    {
        sig_num_t sig = sig_num_wrap((int64_t)2 * i - 3);
        for(uint64_t j=0; j<3; j++)
            sig = sig_num_mul(sig, sig_num_wrap((int64_t)2 * (i + j) - 3));

        flt = float_num_mul_sig(flt, sig);
    }

    uint64_t res = 100 * (size + flt.exponent) / size;
    float_num_free(flt);
    return res;
}