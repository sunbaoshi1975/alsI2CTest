#include "sensorAls.h"
#include "global.h"

/* Private define ------------------------------------------------------------*/
#define ALS_MA_NUM             			4

/* Public variables ----------------------------------------------------------*/
// ALS value (e.g. lux)
uint16_t g_ALS_value = 0;

// ALS changed from prvious sample
uint16_t g_ALS_delta = 0;

/* Private variables ---------------------------------------------------------*/
// Moving average
uint8_t m_ALS_mvPtr = 0;
uint8_t m_ALS_mvLen = 0;
uint16_t m_mvALSData[ALS_MA_NUM] = {0};
uint32_t m_als_mvSum = 0;

/* --------------------- Start implementation --------------------------------*/
/*
 初始化时调用一次 
*/
uint8_t als_init()
{
	// BH1750 may init
	if( bh1750_init(BH1750_MODE_CONT_H_RES) != 0 ) {
		LogOutput("ALS init failed...\r\n");
		return 1;
	}
	LogOutput("ALS started\r\n");
	return 0;
}

/*
 ALS数据是否就绪。
 如果已就绪(1)，可以通过g_ALS_value获得有效值。
*/
uint8_t als_IsDataReady()
{
	return( m_ALS_mvLen < ALS_MA_NUM ? 0 : 1);
}

void als_AddData(const uint16_t _als)
{
	if( _als != m_mvALSData[m_ALS_mvPtr] ) {
		m_als_mvSum += _als;
		m_als_mvSum -= m_mvALSData[m_ALS_mvPtr];
		m_mvALSData[m_ALS_mvPtr] = _als;
	}
	
	m_ALS_mvPtr = (m_ALS_mvPtr + 1) % ALS_MA_NUM;
	if(m_ALS_mvLen < ALS_MA_NUM) m_ALS_mvLen++;
	
	// Calculate the difference
	uint16_t lv_alsValue = m_als_mvSum / m_ALS_mvLen;
	g_ALS_delta = ABS_RETURN(g_ALS_value, lv_alsValue);
	g_ALS_value = lv_alsValue;
}

/*
 定时调用，在主循环中调用或者定时器中调用。
 参考采集周期：ALS_COLLECT_DURATION_10MS
*/
uint8_t als_checkData()
{
	uint16_t lv_newData = 0;
	// BH1750
	if( bh1750_read(&lv_newData) != 0 ) return 1;
	
	als_AddData(lv_newData);
	return 0;	// OK
}
