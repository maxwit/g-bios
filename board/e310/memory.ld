MEMORY
{
    RAM (rwx): ORIGIN = 0x80000000, LENGTH = 16K
}

__stack_base = ORIGIN(RAM) + LENGTH(RAM);
