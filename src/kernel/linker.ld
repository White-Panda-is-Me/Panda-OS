ENTRY(kentry)
OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x7E00;
    .entry :
    {
        *(.entry)
    }
    .text :
    {
        *(.text)
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        *(.bss)
    }
}