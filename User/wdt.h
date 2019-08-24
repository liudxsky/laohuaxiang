#ifndef TM_WATCHDOG_H
#define TM_WATCHDOG_H 120


/**
 * @defgroup TM_WATCHDOG_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief Watchdog timeout settings
 */
typedef enum {
	TM_WATCHDOG_Timeout_5ms = 0x00,   /*!< System reset called every 5ms */
	TM_WATCHDOG_Timeout_10ms = 0x01,  /*!< System reset called every 10ms */
	TM_WATCHDOG_Timeout_15ms = 0x02,  /*!< System reset called every 15ms */
	TM_WATCHDOG_Timeout_30ms = 0x03,  /*!< System reset called every 30ms */
	TM_WATCHDOG_Timeout_60ms = 0x04,  /*!< System reset called every 60ms */
	TM_WATCHDOG_Timeout_120ms = 0x05, /*!< System reset called every 120ms */
	TM_WATCHDOG_Timeout_250ms = 0x06, /*!< System reset called every 250ms */
	TM_WATCHDOG_Timeout_500ms = 0x07, /*!< System reset called every 500ms */
	TM_WATCHDOG_Timeout_1s = 0x08,    /*!< System reset called every 1s */
	TM_WATCHDOG_Timeout_2s = 0x09,    /*!< System reset called every 2s */
	TM_WATCHDOG_Timeout_4s = 0x0A,    /*!< System reset called every 4s */
	TM_WATCHDOG_Timeout_8s = 0x0B,    /*!< System reset called every 8s */
	TM_WATCHDOG_Timeout_16s = 0x0C,   /*!< System reset called every 16s */
	TM_WATCHDOG_Timeout_32s = 0x0D    /*!< System reset called every 32s. This is maximum value allowed with IWDG timer */
} TM_WATCHDOG_Timeout_t;

/**
 * @}
 */

/**
 * @defgroup TM_WATCHDOG_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initialize Watchdog timer
 * @note   After you initialize it, you can't disable it unless reset occur.
 * @param  timeout. Timeout value when watchdog reset happen if not reset by user.
 *            This parameter can be a value of @ref TM_WATCHDOG_Timeout_t enumeration
 * @retval Value if system was reset because of watchdog timer
 *            - 1: Reset happen because of watchdog
 *            - 0: Otherwise
 */
int TM_WATCHDOG_Init(TM_WATCHDOG_Timeout_t timeout);

/**
 * @brief  Reset Watchdog timer
 * @note   This function have to be called periodically to reset watchdog timer, or your system will reset
 * @param  None
 * @retval None
 * @note   Defined as macro for faster execution
 */
#define TM_WATCHDOG_Reset()     (IWDG->KR = 0xAAAA)



#endif
