/******************************************************************************************
  Filename    : OsAsm.s

  Core        : RISC-V RV32IMAC

  Author      : Chalandi Amine

  Owner       : Chalandi Amine

  Date        : 19.12.2022

  Description : Context switch and ISR category 2 wrapper

******************************************************************************************/

#if ((defined(__GNUC__)  && (__GNUC__ > 11)) && defined(__riscv))
.option arch, +zicsr
#endif

.file "OsAsm.s"

.equ OS_CPU_CONTEXT_USED_REGISTERS, 31

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  OsSaveCpuContext                                                                                        */
/*                                                                                                                 */
/* \descr  Macro that save the current context                                                                     */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*                                                                                                                 */
/* \return void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.macro OsSaveCpuContext
  addi sp, sp, -OS_CPU_CONTEXT_USED_REGISTERS*4
  sw x1,   0*4(sp)
  sw x3,   1*4(sp)
  sw x4,   2*4(sp)
  sw x5,   3*4(sp)
  sw x6,   4*4(sp)
  sw x7,   5*4(sp)
  sw x8,   6*4(sp)
  sw x9,   7*4(sp)
  sw x10,  8*4(sp)
  sw x11,  9*4(sp)
  sw x12, 10*4(sp)
  sw x13, 11*4(sp)
  sw x14, 12*4(sp)
  sw x15, 13*4(sp)
  sw x16, 14*4(sp)
  sw x17, 15*4(sp)
  sw x18, 16*4(sp)
  sw x19, 17*4(sp)
  sw x20, 18*4(sp)
  sw x21, 19*4(sp)
  sw x22, 20*4(sp)
  sw x23, 21*4(sp)
  sw x24, 22*4(sp)
  sw x25, 23*4(sp)
  sw x26, 24*4(sp)
  sw x27, 25*4(sp)
  sw x28, 26*4(sp)
  sw x29, 27*4(sp)
  sw x30, 28*4(sp)
  sw x31, 29*4(sp)
  csrr x1, mepc
  sw x1,  30*4(sp)
.endm

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  OsRestoreCpuContext                                                                                     */
/*                                                                                                                 */
/* \descr  Macro that restore the saved context                                                                    */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*                                                                                                                 */
/* \return void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.macro OsRestoreCpuContext
  lw x1,  30*4(sp)
  csrrw zero, mepc, x1
  lw x1,   0*4(sp)
  lw x3,   1*4(sp)
  lw x4,   2*4(sp)
  lw x5,   3*4(sp)
  lw x6,   4*4(sp)
  lw x7,   5*4(sp)
  lw x8,   6*4(sp)
  lw x9,   7*4(sp)
  lw x10,  8*4(sp)
  lw x11,  9*4(sp)
  lw x12, 10*4(sp)
  lw x13, 11*4(sp)
  lw x14, 12*4(sp)
  lw x15, 13*4(sp)
  lw x16, 14*4(sp)
  lw x17, 15*4(sp)
  lw x18, 16*4(sp)
  lw x19, 17*4(sp)
  lw x20, 18*4(sp)
  lw x21, 19*4(sp)
  lw x22, 20*4(sp)
  lw x23, 21*4(sp)
  lw x24, 22*4(sp)
  lw x25, 23*4(sp)
  lw x26, 24*4(sp)
  lw x27, 25*4(sp)
  lw x28, 26*4(sp)
  lw x29, 27*4(sp)
  lw x30, 28*4(sp)
  lw x31, 29*4(sp)
  addi sp, sp, OS_CPU_CONTEXT_USED_REGISTERS*4
.endm

/*-----------------------------------------------------------------------------------------------------------------*/
/* \brief  OsDispatchHandler                                                                                       */
/*                                                                                                                 */
/* \descr  Context switcher                                                                                        */
/*                                                                                                                 */
/* \param  void                                                                                                    */
/*                                                                                                                 */
/* \return void                                                                                                    */
/*-----------------------------------------------------------------------------------------------------------------*/
.section ".text"
.align 4
.globl  OsDispatchHandler
.type   OsDispatchHandler, % function
.extern OsDispatcher

OsDispatchHandler:
                   OsSaveCpuContext
                   mv a0, sp
                   la a1, 0x2000000
                   sw zero, 0(a1)
                   jal OsDispatcher
                   mv sp, a0
                   OsRestoreCpuContext
                   mret

.size OsDispatchHandler, .-OsDispatchHandler

/* ---------------------------------------------------------------------------------------------------------------- */
/* \brief  OsCat2IsrWrapper                                                                                         */
/*                                                                                                                  */
/* \descr  Wrapper for all category 2 interrupts                                                                    */
/*                                                                                                                  */
/* \param  void                                                                                                     */
/*                                                                                                                  */
/* \return void                                                                                                     */
/* ---------------------------------------------------------------------------------------------------------------- */
.section ".text"
.align 4
.globl  OsCat2IsrWrapper
.type   OsCat2IsrWrapper, % function
.extern OsStoreStackPointer
.extern OsRunCat2Isr
.extern OsGetSavedStackPointer
.extern OsIntCallDispatch

OsCat2IsrWrapper:
                   OsSaveCpuContext
                   mv a0, sp
                   jal OsStoreStackPointer
                   jal OsRunCat2Isr
                   jal OsGetSavedStackPointer
                   jal OsIntCallDispatch
                   mv sp, a0
                   OsRestoreCpuContext
                   mret

.size OsCat2IsrWrapper, .-OsCat2IsrWrapper

/* ---------------------------------------------------------------------------------------------------------------- */
/* \brief  OsSysTickIsrWrapper                                                                                      */
/*                                                                                                                  */
/* \descr  Wrapper for System tick interrupt                                                                        */
/*                                                                                                                  */
/* \param  void                                                                                                     */
/*                                                                                                                  */
/* \return void                                                                                                     */
/* ---------------------------------------------------------------------------------------------------------------- */
.section ".text"
.align 4
.globl  OsSysTickIsrWrapper
.type   OsSysTickIsrWrapper, % function
.extern OsStoreStackPointer
.extern OsRunSysTickIsr
.extern OsGetSavedStackPointer
.extern OsIntCallDispatch

OsSysTickIsrWrapper:
                     OsSaveCpuContext
                     mv a0, sp
                     jal OsStoreStackPointer
                     jal OsRunSysTickIsr
                     jal OsGetSavedStackPointer
                     jal OsIntCallDispatch
                     mv sp, a0
                     OsRestoreCpuContext
                     mret

.size OsSysTickIsrWrapper, .-OsSysTickIsrWrapper

/* ------------------------------------------------------------------------------------------------------------------ */
/* / \brief  OsStartNewTask (OsStartNewTask(uint32 StackFramePtr, pFunc TaskFuncPtr))                                 */
/* /                                                                                                                  */
/* / \descr  This function start an OSEK Task for the 1st execution                                                   */
/* /                                                                                                                  */
/* / \param  void                                                                                                     */
/* /                                                                                                                  */
/* / \return void                                                                                                     */
/* ------------------------------------------------------------------------------------------------------------------ */
.section ".text"
.align 4
.globl  OsStartNewTask
.type   OsStartNewTask, % function
.extern osErrTaskExitWithoutTerminate

OsStartNewTask:
                 mv sp,a0
                 csrrw zero, mepc, a1
                 lw x1, osErrTaskExitWithoutTerminate
                 mv x3, x0
                 mv x4, x0
                 mv x5, x0
                 mv x6, x0
                 mv x7, x0
                 mv x8, x0
                 mv x9, x0
                 mv x10,x0
                 mv x11,x0
                 mv x12,x0
                 mv x13,x0
                 mv x14,x0
                 mv x15,x0
                 mv x16,x0
                 mv x17,x0
                 mv x18,x0
                 mv x19,x0
                 mv x20,x0
                 mv x21,x0
                 mv x22,x0
                 mv x23,x0
                 mv x24,x0
                 mv x25,x0
                 mv x26,x0
                 mv x27,x0
                 mv x28,x0
                 mv x29,x0
                 mv x30,x0
                 mv x31,x0
                 mret

.size OsStartNewTask, .-OsStartNewTask

/* ----------------------------------------------------------------------------------------------------------------- */
/*  \brief  OsGetCurrentSP : void OsGetCurrentSP(unsigned int* CurrentSpPtr)                                         */
/*                                                                                                                   */
/*  \descr  Get the current stack pointer register value                                                             */
/*                                                                                                                   */
/*  \param  unsigned int* CurrentSpPtr (out): Current stack pointer register value                                   */
/*                                                                                                                   */
/*  \return void                                                                                                     */
/* ----------------------------------------------------------------------------------------------------------------- */
.section ".text"
.align 4
.globl  OsGetCurrentSP
.type   OsGetCurrentSP, % function

OsGetCurrentSP:
                 mv a0, sp
                 ret

.size OsGetCurrentSP, .-OsGetCurrentSP
