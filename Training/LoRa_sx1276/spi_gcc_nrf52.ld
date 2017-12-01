/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

/* Some space reserved for softdevice for the BLE project */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x23000, LENGTH = 0x5d000
  RAM (rwx) :  ORIGIN = 0x200020e0, LENGTH = 0xdf20

}

SECTIONS
{
}

SECTIONS
{
  . = ALIGN(4);
  .mem_section_dummy_ram :
  {
  }
  .log_dynamic_data :
  {
    PROVIDE(__start_log_dynamic_data = .);
    KEEP(*(.log_dynamic_data))
    PROVIDE(__stop_log_dynamic_data = .);
  } > RAM

} INSERT AFTER .data;

SECTIONS
{
  .mem_section_dummy_rom :
  {
  }
    .log_const_data :
  {
    PROVIDE(__start_log_const_data = .);
    KEEP(*(.log_const_data))
    PROVIDE(__stop_log_const_data = .);
  } > FLASH


} INSERT AFTER .text

INCLUDE "nrf5x_common.ld"
