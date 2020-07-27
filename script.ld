ENTRY(main)

SECTIONS
{
	. = 1M;
	.text :
	{
		*(.multiboot);
		*(.rodata);
		*(.text);
		. = ALIGN(4K);
	}
	.data :
	{
		*(.data);
		. = ALIGN(4K);
	}
	.bss :
	{
		*(COMMON);
		*(.bss);
		. = ALIGN(4K);
	}
}
