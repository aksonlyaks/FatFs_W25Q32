#ifndef   APP_LOG_H__
#define   APP_LOG_H__
#include "stdio.h"
#include "stdint.h"
#include "app_util.h"
#include "app_log_config.h"

#define APP_LOG_OFF                0U
#define APP_LOG_LEVEL_ERROR        1U
#define APP_LOG_LEVEL_WARNING      2U
#define APP_LOG_LEVEL_INFO         3U
#define APP_LOG_LEVEL_DEBUG        4U

#define APP_LOG_COLOR_CODE_DEFAULT "\x1B[0m"
#define APP_LOG_COLOR_CODE_BLACK   "\x1B[1;30m"
#define APP_LOG_COLOR_CODE_RED     "\x1B[1;31m"
#define APP_LOG_COLOR_CODE_GREEN   "\x1B[1;32m"
#define APP_LOG_COLOR_CODE_YELLOW  "\x1B[1;33m"
#define APP_LOG_COLOR_CODE_BLUE    "\x1B[1;34m"
#define APP_LOG_COLOR_CODE_MAGENTA "\x1B[1;35m"
#define APP_LOG_COLOR_CODE_CYAN    "\x1B[1;36m"
#define APP_LOG_COLOR_CODE_WHITE   "\x1B[1;37m"

#define APP_LOG_COLOR_0            APP_LOG_COLOR_CODE_DEFAULT
#define APP_LOG_COLOR_1            APP_LOG_COLOR_CODE_BLACK
#define APP_LOG_COLOR_2            APP_LOG_COLOR_CODE_RED
#define APP_LOG_COLOR_3            APP_LOG_COLOR_CODE_GREEN
#define APP_LOG_COLOR_4            APP_LOG_COLOR_CODE_YELLOW
#define APP_LOG_COLOR_5            APP_LOG_COLOR_CODE_BLUE
#define APP_LOG_COLOR_6            APP_LOG_COLOR_CODE_MAGENTA
#define APP_LOG_COLOR_7            APP_LOG_COLOR_CODE_CYAN
#define APP_LOG_COLOR_8            APP_LOG_COLOR_CODE_WHITE

#define APP_LOG_COLOR_DECODE(N)   CONCAT_2(APP_LOG_COLOR_, N)
#if APP_LOG_USES_COLORS > 0
#define APP_LOG_ERROR_COLOR_CODE   APP_LOG_COLOR_DECODE(APP_LOG_ERROR_COLOR)
#define APP_LOG_WARNING_COLOR_CODE APP_LOG_COLOR_DECODE(APP_LOG_WARNING_COLOR)
#define APP_LOG_INFO_COLOR_CODE    APP_LOG_COLOR_DECODE(APP_LOG_INFO_COLOR)
#define APP_LOG_DEBUG_COLOR_CODE   APP_LOG_COLOR_DECODE(APP_LOG_DEBUG_COLOR)
#else // APP_LOG_USES_COLORS
#define APP_LOG_ERROR_COLOR_CODE
#define APP_LOG_WARNING_COLOR_CODE
#define APP_LOG_INFO_COLOR_CODE
#define APP_LOG_DEBUG_COLOR_CODE
#endif // APP_LOG_USES_COLORS


#if  defined(APP_LOG_USE_TIMESTAMP) && (APP_LOG_USE_TIMESTAMP > 0)
#define APP_LOG_TIMESTAMP_STRING    "[%8d]"
#define APP_LOG_TIMESTAMP_VALUE     APP_TIMESTAMP()
#else
#define APP_LOG_TIMESTAMP_STRING    "[%1d]" 
#define APP_LOG_TIMESTAMP_VALUE      0
#endif

#define APP_LOG_BREAK      " "

#define LOG_ERROR_PREFIX   APP_LOG_ERROR_COLOR_CODE   APP_LOG_TIMESTAMP_STRING APP_LOG_MODULE_NAME APP_LOG_BREAK "[ERROR]"
#define LOG_WARNING_PREFIX APP_LOG_WARNING_COLOR_CODE APP_LOG_TIMESTAMP_STRING APP_LOG_MODULE_NAME APP_LOG_BREAK "[WARNING]"
#define LOG_INFO_PREFIX    APP_LOG_INFO_COLOR_CODE    APP_LOG_TIMESTAMP_STRING APP_LOG_MODULE_NAME APP_LOG_BREAK "[INFO]"
#define LOG_DEBUG_PREFIX   APP_LOG_DEBUG_COLOR_CODE   APP_LOG_TIMESTAMP_STRING APP_LOG_MODULE_NAME APP_LOG_BREAK "[DEBUG]"



#if  APP_LOG_ENABLED > 0

#if  defined(APP_LOG_USE_UART) && (APP_LOG_USE_UART > 0)
#define  APP_LOG_STD_OUT(format,arg...)     printf(format,APP_LOG_TIMESTAMP_VALUE,##arg)
#define  APP_LOG_INIT()                     APP_UART_INIT()
#elif defined(APP_LOG_USE_SEGGER_RTT) && (APP_LOG_USE_SEGGER_RTT > 0)
#define  APP_LOG_STD_OUT(format,arg...)     SEGGER_RTT_printf(0,format,APP_LOG_TIMESTAMP_VALUE,##arg)
#define  APP_LOG_INIT()                     SEGGER_RTT_Init()        

#else 
#error "APP_DEBUG_USE_UART and APP_DEBUG_USE_SEGGER_RTT are all false!!!!! "//���ͬʱΪ�� UART����SEGGER_RTT���
#endif

#define APP_LOG_ERROR(format,arg...)                                                              \
    if ((APP_LOG_MODULE_LEVEL >= APP_LOG_LEVEL_ERROR) &&                                          \
        (APP_LOG_LEVEL_ERROR <= APP_LOG_DEFAULT_LEVEL))                                           \
    {                                                                                             \
      APP_LOG_STD_OUT(LOG_ERROR_PREFIX"func_name:%s line:%d\r\n"format,__func__,__LINE__,##arg);  \
    }
    
#define APP_LOG_WARNING(format,arg...)                                                            \
    if ((APP_LOG_MODULE_LEVEL >= APP_LOG_LEVEL_WARNING) &&                                        \
        (APP_LOG_LEVEL_WARNING <= APP_LOG_DEFAULT_LEVEL))                                         \
    {                                                                                             \
      APP_LOG_STD_OUT(LOG_WARNING_PREFIX"func_name:%s line:%d\r\n"format,__func__,__LINE__,##arg);\
    }
    
#define APP_LOG_INFO(format,arg...)                                                               \
    if ((APP_LOG_MODULE_LEVEL >= APP_LOG_LEVEL_INFO) &&                                           \
        (APP_LOG_LEVEL_INFO <= APP_LOG_DEFAULT_LEVEL))                                            \
    {                                                                                             \
      APP_LOG_STD_OUT(LOG_INFO_PREFIX"func_name:%s line:%d\r\n"format,__func__,__LINE__,##arg);   \
    }
    
#define APP_LOG_DEBUG(format,arg...)                                                              \
    if ((APP_LOG_MODULE_LEVEL >= APP_LOG_LEVEL_DEBUG) &&                                          \
        (APP_LOG_LEVEL_DEBUG <= APP_LOG_DEFAULT_LEVEL))                                           \
    {                                                                                             \
      APP_LOG_STD_OUT(LOG_DEBUG_PREFIX"func_name:%s line:%d\r\n"format,__func__,__LINE__,##arg);  \
    }

#else
#define APP_LOG_ERROR(format,arg...)  
#define APP_LOG_WARNING(format,arg...)   
#define APP_LOG_INFO(format,arg...)  
#define APP_LOG_DEBUG(format,arg...) 
#endif


void app_log_init(void);
void APP_UART_INIT(void);
uint32_t APP_TIMESTAMP(void);
/*********************************************************************
*
*       RTT printf functions (require SEGGER_RTT_printf.c)
*
**********************************************************************
*/
int SEGGER_RTT_printf(unsigned BufferIndex, const char * sFormat, ...);
void         SEGGER_RTT_Init             (void);

#endif