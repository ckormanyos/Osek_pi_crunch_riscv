
/*******************************************************************************************************************
** Includes
*******************************************************************************************************************/
#include <stdbool.h>
#include <stdint.h>

#include <OsTcb.h>
#include <OsAPIs.h>

bool pi_result_is_ok = true;

TASK(T1)
{
  extern void pi_led_toggle(void);

  pi_led_toggle();

  const OsEventMaskType OsWaitEventMask = (OsEventMaskType) EVT_BLINK_LED;

  (void) OS_SetRelAlarm(ALARM_BLINK_LED, 0, 997);

  for(;;)
  {
    if(E_OK == OS_WaitEvent(OsWaitEventMask))
    {
      OsEventMaskType Events = (OsEventMaskType) 0U;

      (void) OS_GetEvent((OsTaskType)T1, &Events);

      if((Events & EVT_BLINK_LED) == EVT_BLINK_LED)
      {
        OS_ClearEvent(EVT_BLINK_LED);

        pi_led_toggle();
      }
    }
    else
    {
      OS_TerminateTask(); /* In case of error we switch off the task */
    }

    if(!pi_result_is_ok)
    {
      OS_TerminateTask(); /* In case of error we switch off the task */
    }
  }
}

TASK(Idle)
{
  for(;;)
  {
    for(unsigned i = (unsigned) UINT8_C(0); i < (unsigned) UINT8_C(32); ++i)
    {
      extern void pi_benchmark_toggle(void);

      pi_benchmark_toggle();

      extern int pi_main(void);

      const int next_pi_result = pi_main();

      pi_result_is_ok = ((next_pi_result == 0) && pi_result_is_ok);
    }

    OS_Schedule();

    if(!pi_result_is_ok)
    {
      /* In case of error we switch off the task */
      OS_TerminateTask();
    }
  }
}
