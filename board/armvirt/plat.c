#include <init.h>
#include <io.h>

// FIXME
void udelay(unsigned long us)
{
    volatile int i = 0;
    while (i < us * 1000) i++;
}

static int __init armvirt_init(void)
{
    return 0;
}

plat_init(armvirt_init);
