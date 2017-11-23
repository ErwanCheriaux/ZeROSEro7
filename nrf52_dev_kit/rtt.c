#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "app_error.h"
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_gap.h"

#include "rtt.h"

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) {
    error_info_t * error_info = (error_info_t *) info ;
    rtt_write_string("\n******** SOFT DEVICE ERROR OCCURED ********\n") ;
    rtt_printf(0, "Error code %#08X ", error_info->err_code) ;

    /*│1462     * @retval ::NRF_SUCCESS The BLE stack has started advertising.                                                                                          │
     │1463     * @retval ::NRF_ERROR_INVALID_ADDR Invalid pointer supplied.                                                                                            │
     │1464     * @retval ::NRF_ERROR_INVALID_STATE Invalid state to perform operation.                                                                                 │
     │1465     * @retval ::NRF_ERROR_CONN_COUNT The limit of available connections has been reached; connectable advertiser cannot be started.                         │
     │1466     * @retval ::NRF_ERROR_INVALID_PARAM Invalid parameter(s) supplied, check the accepted ranges and limits.                                                │
     │1467     * @retval ::BLE_ERROR_GAP_INVALID_BLE_ADDR Invalid Bluetooth address supplied.                                                                          │
     │1468     * @retval ::BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST Discoverable mode and whitelist incompatible.                                                     │
     │1469     * @retval ::NRF_ERROR_RESOURCES*/
    switch(error_info->err_code) {
        case NRF_ERROR_INVALID_ADDR:
            rtt_write_string("NRF_ERROR_INVALID_ADDR\n") ;
            break ;
        case NRF_ERROR_INVALID_STATE:
            rtt_write_string("NRF_ERROR_INVALID_STATE\n") ;
            break ;
        case NRF_ERROR_CONN_COUNT:
            rtt_write_string("NRF_ERROR_CONN_COUNT\n") ;
            break ;
        case NRF_ERROR_INVALID_PARAM:
            rtt_write_string("NRF_ERROR_INVALID_PARAM\n") ;
            break ;
        case BLE_ERROR_GAP_INVALID_BLE_ADDR:
            rtt_write_string("BLE_ERROR_GAP_INVALID_BLE_ADDR\n") ;
            break ;
        case BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST:
            rtt_write_string("BLE_ERROR_GAP_DISCOVERABLE_WITH_WHITELIST\n") ;
            break ;
        case NRF_ERROR_RESOURCES:
            rtt_write_string("NRF_ERROR_RESOURCES\n") ;
            break ;
        default:
            break ;
    }
    rtt_printf(0, "See components/librairies/util/sdk_errors.h for more details on the Error Code \n") ;
    rtt_write_string("Stopping execution.\n") ;

    app_error_save_and_stop(id, pc, info) ;
}
