#include "app_error.h"
#include "nrf_strerror.h"

#include "rtt.h"

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info)
{
    error_info_t *error_info = (error_info_t *)info;
    if(error_info->err_code == 0xDEADBEEF) {
        rtt_write_string("\n******** USER CUSTOM ERROR ********\nCheck code for further information.\n");
    } else {
        rtt_write_string("\n******** SOFT DEVICE ERROR OCCURED ********\n");
        rtt_printf(0, "Error code 0x%#08X : ", error_info->err_code);
        rtt_write_string(nrf_strerror_get(error_info->err_code));
    }
    rtt_write_string("\nStopping execution.\n");

    app_error_save_and_stop(id, pc, info);
}

void rtt_write_buffer_hexa(uint8_t const *p_buff, int len)
{
    uint8_t const *p_current = p_buff;
    rtt_write_string("0x");
    while(p_current < p_buff + len) {
        SEGGER_RTT_printf(0, "%#02X", *p_current);
        p_current++;
    }
}
