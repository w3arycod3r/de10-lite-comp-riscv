#include "Hal.h"
#include <stdbool.h>
#include "log_sys.h"
#include <printf/printf.h>

static VoidFunc Hal_ExtIrqCallback[32];
static VoidFunc Hal_TimerIrqCallback;
static VoidFunc Hal_SoftIrqCallback;

// SysTick Vars
static uint32_t sysTick;
static uint32_t sysTickTmr;

void Hal_SetExtIrqHandler(uint32_t irq, VoidFunc callback) {
	Hal_ExtIrqCallback[irq] = callback;
}

void Hal_SetTimerIrqHandler(VoidFunc callback) {
	Hal_TimerIrqCallback = callback;
}

void Hal_SetSoftIrqHandler(VoidFunc callback) {
	Hal_SoftIrqCallback = callback;
}

void Hal_EnableInterrupt(uint32_t irq) {
	Hal_EnableInterrupts(1 << irq);
}

void Hal_DisableInterrupt(uint32_t irq) {
	Hal_DisableInterrupts(1 << irq);
}

void Hal_EnableInterrupts(uint32_t mask) {
	g_InterruptController->enabled |= mask;
}

void Hal_DisableInterrupts(uint32_t mask) {
	g_InterruptController->enabled &= ~mask;
}

void Hal_Delay(uint32_t cycles) {
	uint32_t startTick = Hal_ReadTime32();
	while ((Hal_ReadTime32() - startTick) < cycles);
}

void Hal_TimerStart(uint64_t value) {
	Hal_DisableMachineInterrupt(IRQ_M_TIMER);
	uint64_t mtime;
	mtime = g_InterruptController->mtime;
	mtime |= (uint64_t)g_InterruptController->mtimeh_latch << 32ULL;
	mtime += value;
	g_InterruptController->mtimecmph_latch = (uint32_t)(mtime >> 32ULL);
	g_InterruptController->mtimecmp_latch = (uint32_t)mtime;
	Hal_EnableMachineInterrupt(IRQ_M_TIMER);
}

void Hal_TimerStop() {
	Hal_DisableMachineInterrupt(IRQ_M_TIMER);
}

void Hal_RaiseSoftInterrupt() {
	g_InterruptController->softinterrupt = 1;
}

void Hal_ClearSoftInterrupt() {
	g_InterruptController->softinterrupt = 0;
}

// Forward Declaration for Port.c
extern uintptr_t SystemInterruptHandler(uintptr_t stack, bool softInterrupt);

// Called from Crt.S

uintptr_t Hal_Exception(uintptr_t stack, uintptr_t addr, uint32_t mcause) {

	if ((mcause & 0x80000000) == 0) {
		printf_("TRAP\n    stack    ");

		// UartWriteHex32(g_Uart, stack, true);
		printf_("0x%.8x\n", (unsigned int)stack);

		printf_("    mepc     ");

		// UartWriteHex32(g_Uart, addr, true);
		printf_("0x%.8x\n", (unsigned int)addr);

		printf_("    mcause   ");

		// UartWriteHex32(g_Uart, mcause, true);
		printf_("0x%.8x\n", (unsigned int)mcause);

		printf_("    irq en   ");

		// UartWriteHex32(g_Uart, g_InterruptController->enabled, true);
		printf_("0x%.8x\n", (unsigned int)g_InterruptController->enabled);

		printf_("    irq pen  ");
		// UartWriteHex32(g_Uart, g_InterruptController->pending, true);
		printf_("0x%.8x\n", (unsigned int)g_InterruptController->pending);

		printf_("    mtval    ");
		// UartWriteHex32(g_Uart, read_csr(mtval),true);
		printf_("0x%.8x\n", (unsigned int)read_csr(mtval));

		printf_("    mbadaddr ");
		// UartWriteHex32(g_Uart, read_csr(mbadaddr), true);
		printf_("0x%.8x\n", (unsigned int)read_csr(mbadaddr));

		// Stall
		while(1);
	} else {
		if ((mcause & 0x7FFFFFFF) == IRQ_M_EXT) {
			uint32_t irq = g_InterruptController->pending & g_InterruptController->enabled;
			for (uint32_t i = 0; i < 32; ++i) {
				if ((irq & (1 << i)) && Hal_ExtIrqCallback[i]) {
					Hal_ExtIrqCallback[i]();
				}
			}
		} else if ((mcause & 0x7FFFFFFF) == IRQ_M_TIMER) {
			if (Hal_TimerIrqCallback) {
				Hal_TimerIrqCallback();
			}
		} else if ((mcause & 0x7FFFFFFF) == IRQ_M_SOFT) {
			if (Hal_SoftIrqCallback) {
				Hal_SoftIrqCallback();
			}
		}
	}

	return stack;

}


void Hal_SysTickInit()
{
	sysTick = 0;
	sysTickTmr = Hal_ReadTime32();
}

void Hal_SysTickServ()
{
	if (HAL_HAS_DURATION_PASSED(sysTickTmr, SYS_TICK_DUR_TICKS))
	{
		sysTickTmr = Hal_ReadTime32();
		sysTick++;
	}
	
}

uint32_t Hal_SysTickRead()
{
	return sysTick;
}