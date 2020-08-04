MEMORY
{
    RAM (rwx): ORIGIN = 0x402007F8, LENGTH = 64K
}

__stack_base = ORIGIN(RAM) + LENGTH(RAM);
