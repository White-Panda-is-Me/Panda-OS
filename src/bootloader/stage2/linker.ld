ENTRY(entry)

SECTIONS
{
    . = 0x7E00;
    .text ALIGN(0x1000) :
    {
        *(.text)
        *(.rodata)
    }

    .data ALIGN(0x1000) :
    {
        *(.data)
    }
}