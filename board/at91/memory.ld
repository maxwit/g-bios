MEMORY
{
    RAM (rwx): ORIGIN = 0x00300000, LENGTH = 64K
}

__stack_base = ORIGIN(RAM) + LENGTH(RAM);
