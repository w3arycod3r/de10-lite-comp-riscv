#ifndef HAL_H
#define HAL_H

#include <stdint.h>

#include "RiscvDef.h"
#include "FpgaConfig.h"


#define IRQ_TIMER 0
#define IRQ_BADINSTR 1
#define IRQ_MEMERROR 2

// SysTick Defines
#define SYS_TICK_DUR_MSEC 1
#define SYS_TICK_DUR_TICKS (MSEC_TO_TICKS(SYS_TICK_DUR_MSEC))

typedef void(*VoidFunc)(void);

// Located in Hal.c

void Hal_SetExtIrqHandler(uint32_t irq, VoidFunc callback);
void Hal_SetTimerIrqHandler(VoidFunc callback);
void Hal_SetSoftIrqHandler(VoidFunc callback);
void Hal_EnableInterrupt(uint32_t irq);
void Hal_DisableInterrupt(uint32_t irq);
void Hal_EnableInterrupts(uint32_t mask);
void Hal_DisableInterrupts(uint32_t mask);
void Hal_Delay(uint32_t cycles);
void Hal_TimerStart(uint64_t value);
void Hal_TimerStop();
void Hal_RaiseSoftInterrupt();
void Hal_ClearSoftInterrupt();

// SysTick
uint32_t Hal_SysTickRead(); // "SysTick" timer with 1 msec resolution
void Hal_SysTickInit();
void Hal_SysTickServ(); 	// Service the "SysTick" timer module

// Located in Hal.S

void Hal_EnableMachineInterrupt(uint32_t irq);
void Hal_DisableMachineInterrupt(uint32_t irq);
void Hal_GlobalEnableInterrupts();
void Hal_GlobalEnableInterrupts();
// Rolls over in about 42.9 seconds @ 100 MHz. You can't use this timer for a duration longer than this.
uint32_t Hal_ReadTime32();
uint64_t Hal_ReadTime64();

// Private function called from Crt.S, not to be called directly
	// uintptr_t Hal_Exception(uintptr_t stack, uintptr_t addr, uint32_t irq);

// Macros
#define HAL_HAS_DURATION_PASSED(timer32, dur32) ( (Hal_ReadTime32()-timer32) > dur32 )
#define HAL_HAS_DURATION_PASSED_MSEC(timer32, dur_msec) ( HAL_HAS_DURATION_PASSED(timer32, MSEC_TO_TICKS(dur_msec)) )

#endif // HAL_H
