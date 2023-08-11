ENTRY(kentry)
OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x1000;
    .entry :
    {
        __entry_start = .;
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