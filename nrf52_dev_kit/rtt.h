#ifndef RTT_H
#define RTT_H

#include "SEGGER_RTT.h"

#define rtt_init SEGGER_RTT_Init
#define rtt_write_string(a) SEGGER_RTT_WriteString(0,a)

/* Printf signature : int SEGGER_RTT_printf(unsigned BufferIndex, const char * sFormat, ...);
Put 0 as first argument for default channel 0 */
#define rtt_printf SEGGER_RTT_printf


/* Called by APP_ERROR_CHECK(err_code) if err_code != NRF_SUCCESS.
info is actually an error_info_t
*/
void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) ;

#endif
