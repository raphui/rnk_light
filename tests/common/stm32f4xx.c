#include "board-stm32f4xx.h"
#include <utils.h>
#include <init.h>
#include <sizes.h>
#include <armv7m/system.h>
#include <armv7m/mpu.h>

#define PLL_M		8
#define PLL_Q		4
#define PLL_N		84
#define PLL_P		2

void set_sys_clock(void)
{
	/* *****************************************************************************/
	/*             PLL (clocked by HSE) used as System clock source                */
	/* *****************************************************************************/
	/*  Select regulator voltage output Scale 1 mode */
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;

	/*  HCLK = SYSCLK / 1*/
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	/*  PCLK2 = HCLK / 1*/
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

	/*  PCLK1 = HCLK / 2*/
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

	/*  Configure the main PLL */
	RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) | (PLL_Q << 24);

	/*  Enable the main PLL */
	RCC->CR |= RCC_CR_PLLON;

	/*  Wait till the main PLL is ready */
	while((RCC->CR & RCC_CR_PLLRDY) == 0)
		;

	/*  Configure Flash prefetch, Instruction cache, Data cache and wait state */
	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_2WS;

	/*  Select the main PLL as system clock source */
	RCC->CFGR &= (unsigned int)((unsigned int)~(RCC_CFGR_SW));
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	/*  Wait till the main PLL is used as system clock source */
	while ((RCC->CFGR & (unsigned int)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
		;
}

void low_level_init(void)
{
	/* Reset RCC clock */
	RCC->CR |= RCC_CR_HSION;
	RCC->CFGR = 0x00000000;
	/* Reset HSEON, CSSON and PLLON bits */
	RCC->CR &= (unsigned int)0xFEF6FFFF;
	/* Reset PLLCFGR register */
	RCC->PLLCFGR = 0x24003010;
	/* Reset HSEBYP bit */
	RCC->CR &= (unsigned int)0xFFFBFFFF;
	/* Disable all interrupts */
	RCC->CIR = 0x00000000;

	set_sys_clock();

	/* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
	writel(SCB_VTOR, SRAM_BASE | VECT_TAB_OFFSET); /* Vector Table Relocation in Internal SRAM */
#else
	writel(SCB_VTOR, FLASH_BASE | VECT_TAB_OFFSET); /* Vector Table Relocation in Internal FLASH */
#endif
}

int device_init(void)
{
	int ret = 0;

#ifdef CONFIG_SWO_DEBUG
	stm32_pio_set_alternate(GPIOB_BASE, 3, 0x0);
	swo_init(stm32_rcc_get_freq_clk(SYSCLK));
	DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN; // Enable IO trace pins
#endif /* CONFIG_SWO_DEBUG */

	return ret;
}
coredevice_initcall(device_init);
