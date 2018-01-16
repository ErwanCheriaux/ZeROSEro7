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

#define BLE_DEVICE_NAME "Connected shoe"
#define APP_ADV_FAST_INTERVAL 40 /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_FAST_TIMEOUT 120 /**< The duration of the fast advertising period (in seconds). */
#define APP_BLE_CONN_CFG_TAG 1   /**< A tag identifying the SoftDevice BLE configuration. */

#define FIRST_CONN_PARAMS_UPDATE_DELAY APP_TIMER_TICKS(5000) /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY APP_TIMER_TICKS(30000) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT 3                       /**< Number of attempts before giving up the connection parameter negotiation. */

#define MIN_CONN_INTERVAL MSEC_TO_UNITS(7.5, UNIT_1_25_MS) /**< Minimum connection interval (7.5 ms) */
#define MAX_CONN_INTERVAL MSEC_TO_UNITS(30, UNIT_1_25_MS)  /**< Maximum connection interval (30 ms). */
#define SLAVE_LATENCY 6                                    /**< Up to Slave latency. */
#define CONN_SUP_TIMEOUT MSEC_TO_UNITS(430, UNIT_10_MS)    /**< Connection supervisory timeout (430 ms). */

static void (*on_phone_connection)();  // Handler from main

BLE_ADVERTISING_DEF(m_advertising); /**< Advertising module instance. */
static ble_advertising_t* const advertising;
static ble_advertising_init_t   advertising_conf;

static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    ret_code_t err_code;

    switch(ble_adv_evt) {
        case BLE_ADV_EVT_DIRECTED:
            rtt_write_string("Directed advertising started\n");
            break;

        case BLE_ADV_EVT_FAST:
            rtt_write_string("Fast advertising started\n");
            // REVIEW Bonus:  Peer and bond
            break;

        case BLE_ADV_EVT_SLOW:
            rtt_write_string("Slow advertising started\n");
            break;

        case BLE_ADV_EVT_IDLE:
            rtt_write_string("Advertising going to idle\n");
            break;

        default:
            break;
    }
}

static ble_gap_conn_params_t    gap_conn_params;
static ble_gap_conn_sec_mode_t  gap_seccurity_mode;
static ble_gap_privacy_params_t gap_privacy_params;

// Disguise as a smart shoe for the first connection, prior to bonding.
void ble_gap_init()
{
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&gap_seccurity_mode);

    APP_ERROR_CHECK(sd_ble_gap_appearance_set(BLE_APPEARANCE_RUNNING_WALKING_SENSOR_IN_SHOE));  // No one will try to find it ;)

    APP_ERROR_CHECK(sd_ble_gap_device_name_set(&gap_seccurity_mode,
                                               (const uint8_t*)BLE_DEVICE_NAME,
                                               strlen(BLE_DEVICE_NAME)));

    // Connection parameters, mostly for low power.
    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    APP_ERROR_CHECK(sd_ble_gap_ppcp_set(&gap_conn_params));
}

// More secure params, including privacy for connecting once bonded
static void gap_secure_params_init()
{
    BLE_GAP_CONN_SEC_MODE_SET_SIGNED_WITH_MITM(&gap_seccurity_mode);

    // Prevents tracking. Identity Resolving Key (IRK) must be shared with peer.
    gap_privacy_params.privacy_mode      = BLE_GAP_PRIVACY_MODE_DEVICE_PRIVACY;
    gap_privacy_params.private_addr_type = BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE;

    APP_ERROR_CHECK(sd_ble_gap_privacy_set(&gap_privacy_params));
}

static void conn_neg_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

static ble_conn_params_init_t conn_neg_init;

void ble_conn_negociation_init()
{
    conn_neg_init.p_conn_params                  = NULL;
    conn_neg_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    conn_neg_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    conn_neg_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    conn_neg_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    conn_neg_init.disconnect_on_fail             = false;
    conn_neg_init.evt_handler                    = NULL;
    conn_neg_init.error_handler                  = conn_neg_error_handler;

    APP_ERROR_CHECK(ble_conn_params_init(&conn_neg_init));
}

static void advertising_params_init()
{
    advertising_conf.advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advertising_conf.advdata.include_appearance      = true;
    advertising_conf.advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;
    advertising_conf.advdata.uuids_complete.uuid_cnt = 0;  // No need to advertise GATT services, obtained after connection
    advertising_conf.advdata.uuids_complete.p_uuids  = NULL;

    // We use fast advertising because advertising doesn't last long. REVIEW Could use Directed for fast recovery
    advertising_conf.config.ble_adv_fast_enabled  = true;
    advertising_conf.config.ble_adv_fast_interval = APP_ADV_FAST_INTERVAL;
    advertising_conf.config.ble_adv_fast_timeout  = APP_ADV_FAST_TIMEOUT;

    advertising_conf.evt_handler = on_adv_evt;
}

void ble_advertise_init()
{
    advertising_params_init();
    APP_ERROR_CHECK(ble_advertising_init(&m_advertising, &advertising_conf));

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);
}

void ble_peripheral_start_advertising()
{
    ret_code_t err_code;
    err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
    if(err_code != NRF_ERROR_INVALID_STATE) {  // Don't restart while started
        APP_ERROR_CHECK(err_code);
    }
}

void ble_peripheral_stop_advertising()
{
    // Stops at the end of current adv. Could use on_adv_evt to know when.
    // APP_ERROR_CHECK(ble_advertising_start(&m_advertising, BLE_ADV_MODE_IDLE));
}
