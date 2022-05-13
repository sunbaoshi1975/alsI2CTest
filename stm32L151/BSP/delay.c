#include "delay.h"
#include "wwdg.h"

static uint16_t fac_us = 0;         // us��ʱ������

// Init clock & delay parameters
// SYSTICK��ʱ�ӹ̶�ΪHCLKʱ��
// SYSCLK:ϵͳʱ��
void delay_init()	 
{
#ifdef SYSCLOCK_DIV_8
	//ѡ���ⲿʱ�ӣ�HCLK/8
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SystemCoreClock / 8000000;	// Ϊϵͳʱ�ӵ�1/8
#else
	// ѡ��ʱ��Դ��ע�⣺��Ҫ��Ƶ������Ӱ��ADC
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	fac_us = SystemCoreClock / 1000000;
#endif
}								    

// ��ʱnus
// nusΪҪ��ʱ��us��.	
void delay_us(const uint32_t nus)
{
	delay_us_no_wwdg(nus);
	feed_wwdg();
}

void delay_us_no_wwdg(const uint32_t nus)
{
 	uint32_t temp;
	SysTick->LOAD = nus * fac_us;                       // ʱ�����	  		 
	SysTick->VAL = 0x00;                                // ��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;          // ��ʼ����	 
	do {
		temp = SysTick->CTRL;
	}
	while(temp&0x01 && !(temp&(1<<16)));                // �ȴ�ʱ�䵽��   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;          // �رռ�����
	SysTick->VAL = 0X00;                                // ��ռ�����
}

// ��ʱnms
// ע��nms�ķ�Χ
// SysTick->LOADΪ24λ�Ĵ���,����, �����ʱΪ:
// nms<=0xffffff*8*1000/SYSCLK
// SYSCLK��λΪHz,nms��λΪms
// ��72M������, nms<=1864
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
