/** @file
 *
 * @defgroup app_error Common application error handler
 * @{
 * @ingroup app_common
 *
 * @brief Common application error handler and macros for utilizing a common error handler.
 */

#ifndef APP_ERROR_H__
#define APP_ERROR_H__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif 

#define  APP_ERROR_RANGE_START          0x00 /**< The start of the range of error*/

#define  APP_SUCCESS                    APP_ERROR_RANGE_START + 0
#define  APP_ERROR_NULL                 APP_ERROR_RANGE_START + 1
#define  APP_ERROR_NO_MEM               APP_ERROR_RANGE_START + 2
#define  APP_ERROR_NOT_FOUND            APP_ERROR_RANGE_START + 3 
#define  APP_ERROR_INVALID_LENGTH       APP_ERROR_RANGE_START + 4
#define  APP_ERROR_INVALID_PARAM        APP_ERROR_RANGE_START + 5




#define APP_FAULT_ID_RANGE_START 0x00004000 /**< The start of the range of error IDs */

/**@defgroup APP_ERROR_FAULT_IDS Fault ID types
 * @{ */
#define APP_FAULT_ID_ERROR       APP_FAULT_ID_RANGE_START + 1 /**< An error stemming from a call to @ref APP_ERROR_CHECK or @ref APP_ERROR_CHECK_BOOL. The info parameter is a pointer to an @ref error_info_t variable. */
#define APP_FAULT_ID_ASSERT      APP_FAULT_ID_RANGE_START + 2 /**< An error stemming from a call to ASSERT (nrf_assert.h). The info parameter is a pointer to an @ref assert_info_t variable. */
/**@} */
 
typedef uint32_t ret_code_t;  
  
  
  
  
/**@brief Structure containing info about an error of the type @ref NRF_FAULT_ID_SDK_ERROR.
 */
typedef struct
{
    uint16_t        line_num;    /**< The line number where the error occurred. */
    uint8_t const * p_file_name; /**< The file in which the error occurred. */
    uint32_t        err_code;    /**< The error code representing the error that occurred. */
} error_info_t;

/**@brief Structure containing info about an error of the type @ref NRF_FAULT_ID_SDK_ASSERT.
 */
typedef struct
{
    uint16_t        line_num;    /**< The line number where the error occurred. */
    uint8_t const * p_file_name; /**< The file in which the error occurred. */
} assert_info_t;


void app_assert_handler(uint16_t line_num, const uint8_t *file_name);
void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info);

/**@brief Function for error handling, which is called when an error has occurred.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);


#define APP_ASSERT(expr)                                                   \
if(expr)                                                                   \
{                                                                          \
}                                                                          \
else                                                                       \
{                                                                          \
 app_assert_handler((uint16_t)__LINE__, (uint8_t *)__FILE__);              \
}                                                                          \



/**@brief Macro for calling error handler function.
 *
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */
#define APP_ERROR_HANDLER(ERR_CODE)                                    \
    do                                                                 \
    {                                                                  \
      app_error_handler((ERR_CODE), __LINE__, (uint8_t*) __FILE__);  \
    } while (0)

/**@brief Macro for calling error handler function if supplied error code any other than SUCCESS.
 *
 * @param[in] ERR_CODE Error code supplied to the error handler.
 */
#define APP_ERROR_CHECK(ERR_CODE)                           \
    do                                                      \
    {                                                       \
        const uint32_t LOCAL_ERR_CODE = (ERR_CODE);         \
        if (LOCAL_ERR_CODE != APP_SUCCESS)                  \
        {                                                   \
            APP_ERROR_HANDLER(LOCAL_ERR_CODE);              \
        }                                                   \
    } while (0)

/**@brief Macro for calling error handler function if supplied boolean value is false.
 *
 * @param[in] BOOLEAN_VALUE Boolean value to be evaluated.
 */
#define APP_ERROR_CHECK_BOOL(BOOLEAN_VALUE)                   \
    do                                                        \
    {                                                         \
        const uint32_t LOCAL_BOOLEAN_VALUE = (BOOLEAN_VALUE); \
        if (!LOCAL_BOOLEAN_VALUE)                             \
        {                                                     \
            APP_ERROR_HANDLER(0);                             \
        }                                                     \
    } while (0)


#ifdef __cplusplus
}
#endif

#endif // APP_ERROR_H__

/** @} */