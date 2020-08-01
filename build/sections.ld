ENTRY(_start)

SECTIONS
{
	.text :
	{
		*(.text)
	} >SRAM

	.data ALIGN(4):
	{
		*(.data)
		. = ALIGN(4);
	} >SRAM

	.rodata ALIGN(4):
	{
		*(.rodata)
		. = ALIGN(4);
	} >SRAM

	.bss ALIGN(4):
	{
		bss_start = .;
		*(.bss)
		bss_end = .;
	} >SRAM

	.os_loader ALIGN(4):
	{
		g_loader_begin = .;
		*(.os_loader)
		g_loader_end = ALIGN(4);
	} >SRAM

	_end = ALIGN(4);
}
