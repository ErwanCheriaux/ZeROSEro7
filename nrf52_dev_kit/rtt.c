#include "rtt.h"
#include "app_error.h"

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info) {
    error_info_t * error_info = (error_info_t *) info ;
    rtt_write_string("\n******** SOFT DEVICE ERROR OCCURED ********\n") ;
    rtt_printf(0, "Error code %#04X", error_info->err_code) ;
    rtt_printf(0, "in file %s:%u\n", error_info->p_file_name, error_info->line_num) ;
    rtt_printf(0, "See components/librairies/util/sdk_errors.h for more details on the Error Code \n") ;
    rtt_write_string("Stopping execution.\n") ;

    app_error_save_and_stop(id, pc, info) ;
}
