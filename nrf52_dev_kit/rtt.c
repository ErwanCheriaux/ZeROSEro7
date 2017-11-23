#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "app_error.h"
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_gap.h"
#include "nrf_strerror.h"

#include "rtt.h"

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) {
    error_info_t * error_info = (error_info_t *) info ;
    rtt_write_string("\n******** SOFT DEVICE ERROR OCCURED ********\n") ;
    rtt_printf(0, "Error code %#08X : ", error_info->err_code) ;
    rtt_write_string(nrf_strerror_get(error_info->err_code)) ;
    rtt_write_string("Stopping execution.\n") ;

    app_error_save_and_stop(id, pc, info) ;
}
