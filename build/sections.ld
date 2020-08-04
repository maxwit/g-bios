ENTRY(_start)

SECTIONS
{
	.text :
	{
		*(.text)
	} >RAM

	.data ALIGN(4):
	{
		*(.data)
		. = ALIGN(4);
	} >RAM

	.rodata ALIGN(4):
	{
		*(.rodata)
		. = ALIGN(4);
	} >RAM

	.bss ALIGN(4):
	{
		bss_start = .;
		*(.bss)
		bss_end = .;
	} >RAM

	.os_loader ALIGN(4):
	{
		g_loader_begin = .;
		*(.os_loader)
		g_loader_end = ALIGN(4);
	} >RAM

	.gbios_init ALIGN(4):
	{
		*(.code.init)
		*(.data.init)
		init_call_begin = ALIGN(4);
		*(.Level0.gbios_init)
		*(.Level1.gbios_init)
		*(.Level2.gbios_init)
		*(.Level3.gbios_init)
		*(.Level4.gbios_init)
		*(.Level5.gbios_init)
		init_call_end = .;
	}

	_end = ALIGN(4);
}
