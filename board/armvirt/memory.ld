MEMORY
{
    RAM (rwx): ORIGIN = 0x40000000, LENGTH = 1M
}

__stack_base = ORIGIN(RAM) + LENGTH(RAM);
