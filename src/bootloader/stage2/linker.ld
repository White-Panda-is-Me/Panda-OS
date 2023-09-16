ENTRY(entry)
OUTPUT_FORMAT("binary")

SECTIONS
{
    . = 0x500;
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