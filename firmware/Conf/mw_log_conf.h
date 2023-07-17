#ifndef __MW_LOG_CONF_H__
#define __MW_LOG_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MW_LOG_ENABLED
#include "stm32_adv_trace.h"
#define MW_LOG(TS,VL, ...)   do{ {UTIL_ADV_TRACE_COND_FSend(VL, T_REG_OFF, TS, __VA_ARGS__);} }while(0)
#else  /* MW_LOG_ENABLED */
#define MW_LOG(TS,VL, ...)
#endif /* MW_LOG_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /*__MW_LOG_CONF_H__ */
