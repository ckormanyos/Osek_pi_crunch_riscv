// ***************************************************************************************
// Filename    : Startup.c
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 11.03.2020
// 
// Description : C/C++ Runtime Setup (Crt0)
// 
// ***************************************************************************************

//=========================================================================================
// Function prototype
//=========================================================================================
void Startup_RunApplication(void);
void Startup_InitMcuSystem(void);

static void Startup_Unexpected_Exit(void);

//=========================================================================================
// Extern function prototype
//=========================================================================================
extern int main(void) __attribute__((weak));

void FE310_HwInitialization(void) __attribute__((weak));

//-----------------------------------------------------------------------------------------
/// \brief  Startup_RunApplication function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
void Startup_RunApplication(void)
{
  /* check the weak function */
  if((unsigned int) &main != 0)
  {
    /* Call the main function */
    main();
  }

  /* Catch unexpected exit from main or if main does not exist */
  Startup_Unexpected_Exit();
}

//-----------------------------------------------------------------------------------------
/// \brief  Startup_Unexpected_Exit function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
static void Startup_Unexpected_Exit(void)
{
  for(;;);
}
//-----------------------------------------------------------------------------------------
/// \brief  Startup_InitMcuSystem function
///
/// \param  void
///
/// \return void
//-----------------------------------------------------------------------------------------
void Startup_InitMcuSystem(void)
{
  if(0 != (unsigned long) &FE310_HwInitialization)
  {
    FE310_HwInitialization();
  }
}
