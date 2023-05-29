#include "FE310.h"
#include "OsAPIs.h"
#include "riscv-csr.h"

void mcal_led_toggle(void);

void mcal_led_toggle(void) { }

void TOGGLE_BLUE_LED(void);

void TOGGLE_BLUE_LED(void)
{
  OS_DisableAllInterrupts();
  csr_clr_bits_mip((uint32)-1);
  GPIO0->output_val.bit.pin5 ^= 1;
  OS_EnableAllInterrupts();
}

void TURNON_BLUE_LED(void);

void TURNON_BLUE_LED(void)
{
  OS_DisableAllInterrupts();
  csr_clr_bits_mip((uint32) -1);
  GPIO0->output_val.bit.pin5 = 1;
  OS_EnableAllInterrupts();
}
