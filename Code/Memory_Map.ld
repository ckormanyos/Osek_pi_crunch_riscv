/******************************************************************************************
  Filename    : Memory_Map.ld
  
  Core        : RV32IMAC
  
  MCU         : FE310-G002 (SiFive)
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 04.06.2021
  
  Description : Linker description file for FE310 SiFive
  
******************************************************************************************/

/******************************************************************************************
ELF Entrypoint
******************************************************************************************/
ENTRY(_start)

INPUT(libc.a libgcc.a)

/******************************************************************************************
 Globals
******************************************************************************************/

__FLASH_START_ADDRESS = 0x20000000;
__FLASH_SIZE          = 4M;

__STACK_START_ADDRESS = 0x80000000;
__STACK_SIZE = 4K;

__RAM_START_ADDRESS = 0x80000000 + __STACK_SIZE;
__RAM_SIZE =  16K - __STACK_SIZE;

/******************************************************************************************
 Memory configuration
******************************************************************************************/

MEMORY
{
  ROM  (rx)   : ORIGIN = __FLASH_START_ADDRESS, LENGTH = __FLASH_SIZE
  STACK(rw)   : ORIGIN = __STACK_START_ADDRESS, LENGTH = __STACK_SIZE
  RAM  (rwx)  : ORIGIN = __RAM_START_ADDRESS,   LENGTH = __RAM_SIZE
}

SECTIONS
{
  . = 0x20000000;

  /* Program code (text) */
  .boot : ALIGN(4)
  {
    PROVIDE(__BOOT_BASE_ADDRESS = .);
    *boot.o(.text)
    . = ALIGN(4);
  } > ROM

  /* Program code (text) */
  .text : ALIGN(0x100)
  {
    PROVIDE(__CODE_BASE_ADDRESS = .);
    _ctors_begin = .;
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array*))
    _ctors_end = .;
    *(.progmem*)
    . = ALIGN(4);
    *(.text)
    . = ALIGN(4);
    *(.text*)
    . = ALIGN(4);
    *(.rodata)
    . = ALIGN(4);
    *(.rodata*)
    . = ALIGN(4);
    *(.glue_7)
    . = ALIGN(4);
    *(.glue_7t)
    . = ALIGN(4);
  } > ROM

  /* vectored IVT */
  .vivt : ALIGN(64)
  {
    PROVIDE(__VIVT_BASE_ADDRESS = .);
    *(*vivt*)
    . = ALIGN(4);
  } > ROM

  .riscv.extab :
  {
    . = ALIGN(4);
    *(.riscv.extab)
    *(.gnu.linkonce.riscvextab.*)
    . = ALIGN(4);
  } > ROM

  .exidx :
  {
    . = ALIGN(4);
    PROVIDE(__exidx_start = .);
    *(.riscv.exidx*)
    . = ALIGN(4);
    PROVIDE(__exidx_end = .);
  } > ROM

  .riscv.attributes : ALIGN(4)
  {
    *(.riscv.attributes)
    . = ALIGN(4);
  } > ROM

  . = __STACK_START_ADDRESS;

  /* stack definition */
  .stack :
  {
    . = ALIGN(MAX(__STACK_SIZE , .), 8);
    PROVIDE(__STACK_TOP = .) ;
  } > STACK

  . = __RAM_START_ADDRESS;
  . = ALIGN(4);

  /* The ROM-to-RAM initialized data section */
  .data : ALIGN(4)
  {
    _data_begin = .;
    *(.data)
    . = ALIGN(4);
    KEEP (*(.data))
    *(.data*)
    . = ALIGN(4);
    KEEP (*(.data*))
    _data_end = .;
  } > RAM AT > ROM

  /* The uninitialized (zero-cleared) data section */
  .bss : ALIGN(4)
  {
    _bss_begin = .;
    *(.bss)
    . = ALIGN(4);
    KEEP (*(.bss))
    *(.bss*)
    . = ALIGN(4);
    KEEP (*(.bss*))
    _bss_end = .;
  } > RAM

  PROVIDE(end = .);
  PROVIDE(_fini = .);

  _rom_data_begin = LOADADDR(.data);
}
