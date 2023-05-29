
/*******************************************************************************************************************
** Includes
*******************************************************************************************************************/
#include <stdbool.h>

#include "OsTcb.h"
#include "OsAPIs.h"
#include "FE310.h"
#include "riscv-csr.h"

bool pi_result_is_ok = true;

void TOGGLE_BLUE_LED(void);
void TURNON_BLUE_LED(void);

TASK(T1)
{
  TURNON_BLUE_LED();

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

        TOGGLE_BLUE_LED();
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
  const OsEventMaskType OsWaitEventMask = (OsEventMaskType) EVT_DUMMY_LED;

  (void) OS_SetRelAlarm(ALARM_DUMMY_LED, 0, 503);

  for(;;)
  {
    if(E_OK == OS_WaitEvent(OsWaitEventMask))
    {
      OsEventMaskType Events = (OsEventMaskType) 0U;

      (void) OS_GetEvent((OsTaskType) Idle, &Events);

      if((Events & EVT_DUMMY_LED) == EVT_DUMMY_LED)
      {
        OS_ClearEvent(EVT_DUMMY_LED);

        extern int pi_main(void);

        const int pi_result = pi_main();

        pi_result_is_ok = (pi_result == 0);

        if(!pi_result_is_ok)
        {
          /* In case of error we switch off the task */
          OS_TerminateTask();
        }
      }
    }
    else
    {
      OS_TerminateTask(); // In case of error we switch off the task
    }
  }
}
