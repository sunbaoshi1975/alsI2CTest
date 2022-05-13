/**
  ******************************************************************************
  * @file    alsI2CTest/USER/src/main_functions.c
  * @author  SBS@DataTellIt Inc.
  * @version V1.0.1
  * @date    Mar 12, 2022
  * @brief   This is the implementation of main functions of ALS sensor I2C example
  ******************************************************************************
  *
  * <h2><center>&copy; COPYRIGHT 2016-2022 DataTellIt Inc.</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "delay.h"
#include "global.h"
#include "i2c.h"
#include "sensorAls.h"
#include "timer.h"
#include "usart2.h"
#include "wwdg.h"
#include <string.h>

/* Public constants ----------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
volatile uint8_t gCollectFlag = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t m_collectTick = 0;

/* Private function prototypes -----------------------------------------------*/
void tmrProcess(void);

/* --------------------- Start implementation --------------------------------*/
/**
  * @brief  System initialization
  * @param  None
  * @retval 0 - Succeeded
            1 - Init error
  */
uint8_t sys_init(void) {
	
	// Init clock, timer, button etc.
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	/* Set HSI as sys clock: 8M */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	
    delay_init();                      	// 延时函数初始化

	// 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	Timer_Init();						// 定时器初始化

    /* Initialize the i2c bus for the current platform */
	I2C1_Soft_Init();
    
    return 0;
}

/**
  * @brief  Device initialization
  * @param  None
  * @retval 0 - Succeeded
            1 - Init error
  */
uint8_t dev_init(void) {
	// Enable Interrupt in case it was disabled anyhow
	__enable_irq();

	// Init Sensor modules
    /* 初始化光照度传感器 */
	als_init();
    
	return 0;
}

/**
  * @brief  Multi-sensor runtime setup, Arduino compatible
  * @param  None
  * @retval None
  *
  */
void setup() {
    // System Init
    sys_init();
	
	/* serial log */
	Serial_Usart2_Init(9600, FALSE);
	Serial_PutString("\r\n-------------APP START ");
	Serial_PutString("-------------\r\n");
	
	/* Initialize Devices */
	dev_init();
	
	// Install timer handler
	Timer_10ms_handler = tmrProcess;
    
	// Init WatchDog (WWDG or IWDG)
 	wwdg_init();    
}

/**
  * @brief  Multi-sensor main loop, Arduino compatible
  * @param  None
  * @retval reboot if return -1
  *
  */
int loop() {
    if( gCollectFlag ) {
        if( (gCollectFlag & MULTI_COLLECT_FLAG_ALS) == MULTI_COLLECT_FLAG_ALS ) {
            // Collect ALS (lux)
            als_checkData();
        }	
        gCollectFlag = 0;
    }
    
	// Feed the Watchdog
	feed_wwdg();
    
    return 0;
}

// Execute every second
void OneSecondProcess(void)
{
    /* 输出光照度数据 */
    if( als_IsDataReady() ) {
        LogOutput("ALS current lux = %d, delta = %d\r\n", g_ALS_value, g_ALS_delta);
    }
}

/**
* @brief  10ms timer process
* @param  None
* @retval None
*/
void tmrProcess(void)
{
	static uint8_t lv_second_tick = 0;
    
	// Check data-collection timer
	if( gCollectFlag == 0 ) {
        if( ++m_collectTick >= ALS_COLLECT_DURATION_10MS ) {
            m_collectTick = 0;
            gCollectFlag |= MULTI_COLLECT_FLAG_ALS;
        }
	}
    
	// Check other slow timers every one second
	if( ++lv_second_tick >= 100 ) {
		lv_second_tick = 0;
		OneSecondProcess();
	}    
}

/***************(C)COPYRIGHT 2016 - 2022 DataTellIt Inc. *****END OF FILE******/
