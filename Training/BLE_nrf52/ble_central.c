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

/**@brief Variable length data encapsulation in terms of length and pointer to data. */
typedef struct
{
    uint8_t *p_data;   /**< Pointer to data. */
    uint16_t data_len; /**< Length of data. */
} data_t;

// GAP Handler

static void (*ble_on_notice_phone)();  // Handler from main
static void (*ble_on_phone_connected)();  // Handler from main
static void (*ble_on_phone_disconnected)();  // Handler from main
static void (*ble_on_phone_write)(uint8_t *buff, int length);  // Handler from main

static char *   phone_expected_name = "ZeROSEro7 phone";
static uint32_t parse_advdata(data_t const *const adv_data)
{
    uint32_t field_index = 0;
    uint8_t *p_data;

    p_data = adv_data->p_data;

    while(field_index < adv_data->data_len) {
        uint8_t field_length = p_data[field_index];
        uint8_t field_type   = p_data[field_index + 1];
        rtt_write_string("Data field: ");
        rtt_printf(0, "Type = 0x%#02X\n", field_type);

        data_t field_data;

        field_data.p_data   = &p_data[field_index + 2];
        field_data.data_len = field_length - 1;
        rtt_write_string("Data: ");

        if(field_type == BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME || field_type == BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME) {
            rtt_write_buffer(0, field_data.p_data, field_data.data_len);
            rtt_write_string("\n");
            if(!memcmp(phone_expected_name, (char *)field_data.p_data, sizeof(phone_expected_name))) {  // TODO Shifting codes via pairing.
                ble_on_notice_phone();
            }

        } else {
            rtt_write_buffer_hexa(field_data.p_data, field_data.data_len);
            rtt_write_string("\n");
        }

        field_index += field_length + 1;
    }
    return NRF_SUCCESS;
}

// Called when an advertisement is detected
static void on_adv_report(const ble_evt_t *const p_ble_evt)
{
    rtt_write_string("\n => Advertisement detected\n");
    ret_code_t err_code;

    ble_gap_evt_t const *gap_evt = &p_ble_evt->evt.gap_evt;

    ble_gap_addr_t const *direct_addr = &gap_evt->params.adv_report.direct_addr;
    rtt_printf(0, "Address: Type = 0x%#02X, GAP = ", direct_addr->addr_type);
    rtt_write_buffer_hexa(direct_addr->addr, BLE_GAP_ADDR_LEN);
    rtt_write_string("\n");

    uint8_t const adv_type = gap_evt->params.adv_report.type;
    rtt_printf(0, "Advertisement Type: %#02X\n", adv_type);

    data_t adv_data;
    adv_data.p_data   = (uint8_t *)gap_evt->params.adv_report.data;
    adv_data.data_len = gap_evt->params.adv_report.dlen;
    rtt_write_string("Contents :\n");

    parse_advdata(&adv_data);
}

// Scanning Parameters

#define SCAN_INTERVAL 0x00A0 /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW 0x0050   /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_TIMEOUT 0x0000  /**< Timout when scanning. 0x0000 disables timeout. */

static ble_gap_scan_params_t const scan_conf =
    {
        .active         = 1,
        .interval       = SCAN_INTERVAL,
        .window         = SCAN_WINDOW,
        .timeout        = SCAN_TIMEOUT,
        .use_whitelist  = 0,
        .adv_dir_report = 1,  // Enables printing private addresses not peered
};

// BLE Handler
static void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context)
{
    ret_code_t err_code;

    static ble_gap_conn_params_t new_conn_params;
    static ble_gatts_evt_write_t write_evt;

    switch(p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_ADV_REPORT:
            on_adv_report(p_ble_evt);
            break;

        case BLE_GAP_EVT_CONNECTED:
            rtt_printf(0,"Connected with: %08x\n",p_ble_evt->evt.gap_evt.params.connected.peer_addr);
            ble_on_phone_connected();
            break ;
        case BLE_GAP_EVT_DISCONNECTED:
            rtt_printf(0,"Disconnected because HCI reason:%04x\n",
                p_ble_evt->evt.gap_evt.params.disconnected.reason
            );
            ble_on_phone_disconnected();
            break;
        case BLE_GAP_EVT_CONN_PARAM_UPDATE :
            new_conn_params = p_ble_evt->evt.gap_evt.params.conn_param_update.conn_params;
            rtt_printf(0,"Params Update\nmin_conn_interval:%u\nmax_conn_interval:%u\nslave_latency:%u\nconn_sup_timeout:%u\n",
                new_conn_params.min_conn_interval*5/4,
                new_conn_params.max_conn_interval*5/4,
                new_conn_params.slave_latency,
                new_conn_params.conn_sup_timeout*10
            );
            break ;
        case BLE_GAP_EVT_TIMEOUT:
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
        case BLE_GAP_EVT_SEC_INFO_REQUEST:
        case BLE_GAP_EVT_AUTH_KEY_REQUEST:
        case BLE_GAP_EVT_PASSKEY_DISPLAY:
        case BLE_GAP_EVT_AUTH_STATUS:
        case BLE_GAP_EVT_CONN_SEC_UPDATE:
        case BLE_GAP_EVT_SEC_REQUEST:
        case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
            rtt_printf(0,"GAP EVT : %u\n",p_ble_evt->header.evt_id - BLE_GAP_EVT_CONNECTED);
        break;

        case BLE_GATTS_EVT_WRITE:
            write_evt = p_ble_evt->evt.gatts_evt.params.write;
            rtt_printf(0,"GATT Write\n");
            // TODO Could check if it is indeed the uart_service handle
            ble_on_phone_write(write_evt.data,write_evt.len);
            rtt_write_buffer_hexa(write_evt.data,write_evt.len);
            rtt_write_string("\n");
            break;

        // REVIEW Bonus Bonding events

        default:
            break;
    }
}

// BLE Parameters

#define APP_BLE_CONN_CFG_TAG 1  /**< A tag identifying the SoftDevice BLE configuration. */
#define APP_BLE_OBSERVER_PRIO 3 /**< Application's BLE observer priority. You shouldn't need to modify this value. */

void ble_stack_init()
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    uint32_t ram_start = 0;
    err_code           = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

// Exports
void ble_handler_init(
    void (*phone_noticed_handler)(),
    void (*phone_connected_handler)(),
    void (*phone_disconnected_handler)(),
    void (*phone_write_handler)(uint8_t *buff, int length)
) {
    if(phone_noticed_handler == NULL ||
        phone_connected_handler == NULL ||
        phone_disconnected_handler == NULL ||
        phone_write_handler == NULL
    ) {
        APP_ERROR_CHECK(NRF_ERROR_INVALID_PARAM);
    }
    ble_on_notice_phone = phone_noticed_handler;
    ble_on_phone_connected = phone_connected_handler;
    ble_on_phone_disconnected = phone_disconnected_handler;
    ble_on_phone_write = phone_write_handler;
}

void ble_start_observing()
{
    ret_code_t err_code;
    (void)sd_ble_gap_scan_stop();

    err_code = sd_ble_gap_scan_start(&scan_conf);
    APP_ERROR_CHECK(err_code);
}

void ble_stop_observing()
{
    sd_ble_gap_scan_stop();
}
