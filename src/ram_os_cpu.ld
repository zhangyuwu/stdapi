
SECTIONS
{
	.text : {
		_start = .;
		Image_TXT_Base = .;
		cstartup_flash.o(.text) main.o(.text)
		Image_TXT_Limit = .;
	}

	.rodata : {
		Image_RO_Base = .;
		*(.rodata)
		Image_RO_Limit = .;
	}

	.data : {
		Image_RW_Base = .;
		*(.data)
		os_cpu_a.o(.text)
		Image_RW_Limit = .;
	}
	.bss : {
		Image_ZI_Base = .;
		*(.bss)
		Image_ZI_Limit = .;
	}
	end = .;
}

