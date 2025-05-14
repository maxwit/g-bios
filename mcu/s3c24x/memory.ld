MEMORY
{
    RAM (rwx): ORIGIN = 0x00000000, LENGTH = 4K
}

__stack_base = ORIGIN(RAM) + LENGTH(RAM);
