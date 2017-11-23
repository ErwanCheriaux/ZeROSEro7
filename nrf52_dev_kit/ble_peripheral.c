#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble_hci.h"
#include "ble_gap.h"
#include "ble_err.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "ble_srv_common.h"
#include "ble_conn_params.h"
#include "ble_db_discovery.h"
#include "ble_ans_c.h"
#include "app_timer.h"
#include "bsp_btn_ble.h"
#include "peer_manager.h"
#include "fds.h"
#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "rtt.h"




// BLE Params
#define APP_BLE_OBSERVER_PRIO 3 /**< Application's BLE observer priority. You shouldn't need to modify this value. */
#define APP_BLE_CONN_CFG_TAG 1 /**< A tag identifying the SoftDevice BLE configuration. */

// Advertising Params
// Advertising is in slow mode since no power management is required in the dev kit
#define DEVICE_NAME "Gatt Parrot"
#define APP_ADV_FAST_INTERVAL 40 /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_FAST_TIMEOUT 150 /**< The duration of the fast advertising period (in seconds). */



/**< GATT module instance. */
BLE_ADVERTISING_DEF(advertising_instance);



// Advertising
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    ret_code_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_DIRECTED:
            rtt_write_string("Directed advertising\n");
            break;

        case BLE_ADV_EVT_FAST:
            rtt_write_string("Fast advertising\n");
            /*err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);*/
            break;

        case BLE_ADV_EVT_SLOW:
            rtt_write_string("Slow advertising\n");
            break;

        case BLE_ADV_EVT_IDLE:
            break;

        default:
            break;
    }
}

ble_advertising_init_t advertising_conf ;
static void advertising_init() {
    ret_code_t err_code;

    advertising_conf.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advertising_conf.advdata.include_appearance      = true;
    advertising_conf.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advertising_conf.advdata.uuids_complete.uuid_cnt = 0;
    advertising_conf.advdata.uuids_complete.p_uuids  = NULL;

    advertising_conf.config.ble_adv_fast_enabled  = true;
    advertising_conf.config.ble_adv_fast_interval = APP_ADV_FAST_INTERVAL;
    advertising_conf.config.ble_adv_fast_timeout  = APP_ADV_FAST_TIMEOUT;

    advertising_conf.evt_handler = on_adv_evt;

	err_code = ble_advertising_init(&advertising_instance, &advertising_conf) ;
    APP_ERROR_CHECK(err_code);
}


// BLE Basic
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context) {
    rtt_write_string("BLE Event\n") ;
}

static void ble_stack_init()
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

void ble_init(void) {
    ble_stack_init();
    advertising_init();
}

void ble_start_advertising() {
    ret_code_t err_code = ble_advertising_start(&advertising_instance, BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);
}
