MEMORY
{
    RAM (rwx): ORIGIN = 0x40304348, LENGTH = 64K
}

__stack_base = ORIGIN(RAM) + LENGTH(RAM);
