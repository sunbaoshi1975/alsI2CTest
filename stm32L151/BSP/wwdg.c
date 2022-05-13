#include "wwdg.h"
#include "stm32l1xx.h"

// Window Watchdog
#define WWDG_COUNTER                    0x7F
#define WWDG_WINDOW                     0x7F
/* IWDG: [0, 0x0FFF]
  0.5s @ IWDG_Prescaler_4, 
    1s @ IWDG_Prescaler_8, 
    2s @ IWDG_Prescaler_16, 
    4s @ IWDG_Prescaler_32, 
    8s @ IWDG_Prescaler_64, 
   16s @ IWDG_Prescaler_128,
   32s @ IWDG_Prescaler_256
 */
#define IWDG_RELOAD_VALUE               0xFFF       // ´óÔ¼4 @32K LSI with IWDG_Prescaler_32

uint8_t feedingDog = 0;

// Initialize Window Watchdog
void wwdg_init() {
#ifndef DEBUG_NO_WWDG
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	//WWDG_DeInit();
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(WWDG_WINDOW);
	WWDG_SetCounter(WWDG_COUNTER);
	feedingDog = 0;
	WWDG_Enable(WWDG_COUNTER);
#else
    // Use IWDG instead
    iwdg_init();
#endif  
}

// Feed the Window Watchdog
void feed_wwdg(void) {
#ifndef DEBUG_NO_WWDG
 	if(feedingDog == 1) return;
	feedingDog = 1;
  	uint8_t cntValue = (WWDG->CR) & WWDG_COUNTER;
	if( cntValue < WWDG_WINDOW ) {
		WWDG_SetCounter(WWDG_COUNTER);
	}
	feedingDog = 0;
#else
    // Use IWDG instead
    feed_iwdg();
#endif  
}

void sys_restart(void)
{
    //Serial_PutString("restart...");
#ifdef DEBUG_IWDG
    __set_FAULTMASK(1);
    NVIC_SystemReset();
#else
	WWDG->CR = 0x80;
#endif
}

// Initialize Indepent Watchdog
void iwdg_init(void)
{
#ifdef DEBUG_IWDG
    // Enable write access to IWDG registers.
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    // IWDG timer clock will be (LSI / 32).
    IWDG_SetPrescaler(IWDG_Prescaler_32);
    // Reload the IWDG counter (kick the dog for once!).
    IWDG_SetReload(IWDG_RELOAD_VALUE);
    IWDG_ReloadCounter();
    // Enable IWDG (LSI will be enabled by hardware).
    IWDG_Enable();        
#endif
}

void feed_iwdg(void)
{
#ifdef DEBUG_IWDG
    // Reload the IWDG counter
    IWDG_ReloadCounter();
#endif
}