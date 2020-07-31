/* Label for program entry point */
ENTRY(reset_handler)

/* Define the end of RAM and limit of stack memory */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/*
 * Set minimum size for stack and heap.
 * The linker will generate an error if there is
 * less than v
 */
_Min_Leftover_RAM_ = 0x400; /* 1KB */


MEMORY{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rxw) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS{
    /* Vector table goes at the start of the flash */
    .vector_table : {
        . = ALIGN(4);
        KEEP(*(.vector_table))
        . = ALIGN(4);
    } >FLASH

    /* text section contains the main program code */
    .text : {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } >FLASH

    /* rodata section contains read-only data */
    .rodata : {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    /*
     * data section contains things which can change.
     * Initialized variables go here.
     */
    _sidata = .;
    .data : AT(_sidata) {
        . = ALIGN(4);
        /* Mark start/end locations of data section */
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
        . = ALIGN(4);
    } >RAM

    /*
     * bss contains uninitialized or 0-initialized
     * global variables. Will be 0-filled by core.s
     */
    .bss : {
        . = ALIGN(4);
        /* Mark start/end of the bss section */
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM

    /*
     * Space set aside for the application's heap/stack.
     * This is useful to prevent a where where there is
     * not enough RAM for the program.
     */
    .dynamic_allocations : {
        . = ALIGN(4);
        _ssystem_ram = .;
        . = . + _Min_Leftover_RAM_;
        . = ALIGN(4);
        _esystem_ram = .;
    } >RAM

    /*.preinit_array : {
        . = ALIGN(4);
        _spreinit_array = .;
        KEEP (*(.preinit_array))
        KEEP (*(.preinit_array*))
        _epreinit_array = .;
        . = ALIGN(4);
    } >FLASH

    .init_array : {
        . = ALIGN(4);
        _sinit_array = .;
        KEEP (*(.init_array))
        KEEP (*(.init_array*))
        _einit_array = .;
        . = ALIGN(4);
    } >FLASH

    .fini_array : {
        . = ALIGN(4);
        _sfini_array = .;
        KEEP (*(.fini_array))
        KEEP (*(.fini_array*))
        _efini_array = .;
        . = ALIGN(4);
    } >FLASH*/
}