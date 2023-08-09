ENTRY(kentry)
OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x100000;
    .kentry :
    {
        __entry_start = .;
        *(.kentry)
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