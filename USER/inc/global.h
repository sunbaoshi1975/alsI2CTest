#ifndef __GLOBAL_H
#define __GLOBAL_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// MULTI_COLLECT FlagBitmap
#define MULTI_COLLECT_FLAG_ALS		        0x01			// Read ALS

// Put global variables here
extern volatile uint8_t gCollectFlag;

#ifdef __cplusplus
}
#endif

#endif /* __GLOBAL_H */
