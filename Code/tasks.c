
/*******************************************************************************************************************
** Includes
*******************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>

#include <OsAPIs.h>
#include <OsTcb.h>

static bool pi_result_is_ok = true;

TASK(T1)
{
  pi_result_is_ok = true;

  extern void mcal_led_toggle(void);

  mcal_led_toggle();

  const OsEventMaskType OsWaitEventMask = (OsEventMaskType) EVT_BLINK_LED;

  (void) OS_SetRelAlarm(ALARM_BLINK_LED, (OsTickType) UINT8_C(0), (OsTickType) UINT16_C(997));

  for(;;)
  {
    if(E_OK == OS_WaitEvent(OsWaitEventMask))
    {
      OsEventMaskType Events = (OsEventMaskType) UINT8_C(0);

      (void) OS_GetEvent((OsTaskType) T1, &Events);

      if((Events & EVT_BLINK_LED) == EVT_BLINK_LED)
      {
        OS_ClearEvent(EVT_BLINK_LED);

        mcal_led_toggle();
      }
    }
    else
    {
      OS_TerminateTask();
    }

    if(!pi_result_is_ok)
    {
      OS_TerminateTask();
    }
  }
}

TASK(Idle)
{
  pi_result_is_ok = true;

  for(;;)
  {
    for(unsigned i = (unsigned) UINT8_C(0); i < (unsigned) UINT16_C(1024); ++i)
    {
      extern int pi_main(void);

      const int next_pi_result = pi_main();

      pi_result_is_ok = ((next_pi_result == (int) INT8_C(0)) && pi_result_is_ok);
    }

    (void) OS_Schedule();

    if(!pi_result_is_ok)
    {
      OS_TerminateTask();
    }
  }
}
