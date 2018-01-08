#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "nrf_pwr_mgmt.h"
#include "app_timer.h"
#include "boards.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_db_discovery.h"
#include "ble_lbs_c.h"
#include "nrf_ble_gatt.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "rtt.h"

#define BLE_DEVICE_NAME                  "BLE Parrot"
#define APP_ADV_FAST_INTERVAL           40                                          /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_FAST_TIMEOUT            30                                          /**< The duration of the fast advertising period (in seconds). */
#define APP_BLE_CONN_CFG_TAG            1                                           /**< A tag identifying the SoftDevice BLE configuration. */

#define MIN_CONN_INTERVAL                   MSEC_TO_UNITS(7.5, UNIT_1_25_MS)           /**< Minimum connection interval (7.5 ms) */
#define MAX_CONN_INTERVAL                   MSEC_TO_UNITS(30, UNIT_1_25_MS)            /**< Maximum connection interval (30 ms). */
#define SLAVE_LATENCY                       6                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                    MSEC_TO_UNITS(430, UNIT_10_MS)             /**< Connection supervisory timeout (430 ms). */


static void (*on_phone_connection)();  // Handler from main

BLE_ADVERTISING_DEF(m_advertising); /**< Advertising module instance. */
static ble_advertising_t * const advertising;
static ble_advertising_init_t advertising_conf;

static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    ret_code_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_DIRECTED:
            NRF_LOG_INFO("Directed advertising");
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING_DIRECTED);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_ADV_EVT_FAST:
            NRF_LOG_INFO("Fast advertising");
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_ADV_EVT_SLOW:
            NRF_LOG_INFO("Slow advertising");
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING_SLOW);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_ADV_EVT_IDLE:
            break;

        default:
            break;
    }
}

static ble_gap_conn_params_t gap_conn_params;
static ble_gap_conn_sec_mode_t sec_mode;

static void gap_params_init() {

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    APP_ERROR_CHECK(sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)BLE_DEVICE_NAME,
                                          strlen(BLE_DEVICE_NAME)));

    APP_ERROR_CHECK(sd_ble_gap_appearance_set(BLE_APPEARANCE_HID_KEYBOARD));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    APP_ERROR_CHECK(sd_ble_gap_ppcp_set(&gap_conn_params));
}

static void advertising_params_init() {
    advertising_conf.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advertising_conf.advdata.include_appearance      = true;
    advertising_conf.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advertising_conf.advdata.uuids_complete.uuid_cnt = 0;
    advertising_conf.advdata.uuids_complete.p_uuids  = NULL;

    // We use fast advertising because advertising doesn't last long. TODO Could use Directed for fast recovery
    advertising_conf.config.ble_adv_fast_enabled  = true;
    advertising_conf.config.ble_adv_fast_interval = APP_ADV_FAST_INTERVAL;
    advertising_conf.config.ble_adv_fast_timeout  = APP_ADV_FAST_TIMEOUT;

    advertising_conf.evt_handler = on_adv_evt;
}

void ble_peripheral_advertising_init(void (*phone_connected_handler)()) {
    on_phone_connection = phone_connected_handler;

    gap_params_init();
    advertising_params_init();
    APP_ERROR_CHECK(ble_advertising_init(&m_advertising, &advertising_conf));

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

void ble_peripheral_start_advertising() {
    APP_ERROR_CHECK(ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST));
}

void ble_peripheral_stop_advertising() {
    APP_ERROR_CHECK(ble_advertising_start(&m_advertising, BLE_ADV_MODE_IDLE));
}
