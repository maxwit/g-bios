MEMORY
{
    RAM (rwx): ORIGIN = 0x0C000000, LENGTH = 64K
}

__stack_base = ORIGIN(RAM) + LENGTH(RAM);
