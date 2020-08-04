unsigned long read_cpu_id(void)
{
    extern unsigned long __stack_base;

    return __stack_base;
}
