#include "delay.h"
#include "wwdg.h"

static uint16_t fac_us = 0;         // us延时倍乘数

// Init clock & delay parameters
// SYSTICK的时钟固定为HCLK时钟
// SYSCLK:系统时钟
void delay_init()	 
{
#ifdef SYSCLOCK_DIV_8
	//选择外部时钟，HCLK/8
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SystemCoreClock / 8000000;	// 为系统时钟的1/8
#else
	// 选择时钟源，注意：不要分频，以免影响ADC
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	fac_us = SystemCoreClock / 1000000;
#endif
}								    

// 延时nus
// nus为要延时的us数.	
void delay_us(const uint32_t nus)
{
	delay_us_no_wwdg(nus);
	feed_wwdg();
}

void delay_us_no_wwdg(const uint32_t nus)
{
 	uint32_t temp;
	SysTick->LOAD = nus * fac_us;                       // 时间加载	  		 
	SysTick->VAL = 0x00;                                // 清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;          // 开始倒数	 
	do {
		temp = SysTick->CTRL;
	}
	while(temp&0x01 && !(temp&(1<<16)));                // 等待时间到达   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;          // 关闭计数器
	SysTick->VAL = 0X00;                                // 清空计数器
}

// 延时nms
// 注意nms的范围
// SysTick->LOAD为24位寄存器,所以, 最大延时为:
// nms<=0xffffff*8*1000/SYSCLK
// SYSCLK单位为Hz,nms单位为ms
// 对72M条件下, nms<=1864
void delay_ms(const uint16_t nms)
{
	/*
	uint32_t temp;
	SysTick->LOAD = 2000 * nms;
	SysTick->VAL = 0X00;
	SysTick->CTRL = 0X01;
	do {
		temp=SysTick->CTRL;
     	feed_wwdg();
	} while((temp&0x01) && (!(temp&(1<<16))));
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0X00;
	*/
	
	uint16_t time_ms = nms;
	while(time_ms--) {
		delay_us_no_wwdg(1000);
     	feed_wwdg();
	}
}
