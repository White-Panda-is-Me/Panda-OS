ENTRY(kentry)
OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x100000;
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