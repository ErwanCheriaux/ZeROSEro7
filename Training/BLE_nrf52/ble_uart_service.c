#include <stdint.h>

#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"
#include "sdk_config.h"
#include "nrf_log.h"
#include "mem_manager.h"

#include "ble_central.h"
#include "ble_uart_service.h"
#include "rtt.h"

#define BLE_UUID_BASE                                                         \
    {                                                                         \
        {                                                                     \
            0x23, 0xD1, 0x13, 0xEF, 0x5F, 0x78, 0x23, 0x15, 0xDE, 0xEF, 0x12, \
                0x12, 0x00, 0x00, 0x00, 0x00                                  \
        }                                                                     \
    }
#define BLE_UUID_UART_SERVICE 0xABCD
#define BLE_MS_BLE_OBSERVER_PRIO 2

#define BLE_UUID_WRITE_CHAR 0x1234
#define BLE_UART_MAX_CHAR_LEN BLE_GATTS_VAR_ATTR_LEN_MAX  // Max length of a packet

ble_ms_t uart_characteristic_config;

void* ble_uart_characteristic_value;

// Copied from Little Brothers.
void uart_service_init()
{
    ble_uuid128_t ms_base_uuid = BLE_UUID_BASE;

    // Initialize the service structure
    uart_characteristic_config.conn_handle = BLE_CONN_HANDLE_INVALID;

    // Add a new vendor specific UUID to the softdevice table
    ble_uuid_t ms_uuid;
    ret_code_t err_code =
        sd_ble_uuid_vs_add(&ms_base_uuid, &uart_characteristic_config.uuid_type);
    APP_ERROR_CHECK(err_code);
    ms_uuid.type = uart_characteristic_config.uuid_type;
    ms_uuid.uuid = BLE_UUID_UART_SERVICE;

    // Add the service to the GATT
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ms_uuid,
                                        &uart_characteristic_config.service_handle);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Service UUID: 0x%#04x\n", ms_uuid.uuid);
    NRF_LOG_INFO("Service UUID type: 0x%#02x\n", ms_uuid.type);
    NRF_LOG_INFO("Service handle: 0x%#04x\n", uart_characteristic_config.service_handle);

    // UUID
    ble_uuid_t ble_uuid;
    ble_uuid.type = uart_characteristic_config.uuid_type;
    ble_uuid.uuid = BLE_UUID_WRITE_CHAR;

    // Client Characteristic Configuration Descriptor Metadata
    ble_gatts_attr_md_t cccd_md = {{0}};  // Double bracket for old gcc version
    // Set GAP connection security to open for read and write permissions
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    // Storing the CCCD value in stack (softdevice) memory
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    // Characteristic Metadata
    ble_gatts_char_md_t char_md      = {{0}};  // Double bracket for old gcc version
    char_md.char_props.write         = 1;
    char_md.char_props.write_wo_resp = 1;
    char_md.char_props.read          = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = &cccd_md;
    char_md.char_props.notify        = 1;
    char_md.p_sccd_md                = NULL;

    // Attribute Metadata
    ble_gatts_attr_md_t attr_md = {{0}};  // Double bracket for old gcc version
    // Set GAP connection security to open for read and write permissions
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);  // REVIEW Bonus Security
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc    = BLE_GATTS_VLOC_USER;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    // Characteristic Value Attribute
    ble_gatts_attr_t attr_char_value = {0};
    APP_ERROR_CHECK(nrf_mem_init());
    ble_uart_characteristic_value = nrf_malloc(512);
    if(ble_uart_characteristic_value == NULL) {
        APP_ERROR_CHECK(NRF_ERROR_NO_MEM);
    }
    attr_char_value.p_value   = ble_uart_characteristic_value;
    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = BLE_UART_MAX_CHAR_LEN;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = BLE_UART_MAX_CHAR_LEN;
    APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(uart_characteristic_config.service_handle,
                                                    &char_md, &attr_char_value,
                                                    &uart_characteristic_config.char_handles));
}

static ble_gatts_hvx_params_t notification_params;
static uint16_t hvx_sent_length;

void phone_send_notification(uint8_t* buff, int length)
{
    hvx_sent_length = length;
    notification_params.p_len = &hvx_sent_length;
    notification_params.p_data = buff;
    notification_params.type = BLE_GATT_HVX_NOTIFICATION;
    notification_params.handle = uart_characteristic_config.char_handles.value_handle;
    ret_code_t err_code = sd_ble_gatts_hvx(ble_central_latest_conn,&notification_params);
    if(err_code != 0x3401) {
        APP_ERROR_CHECK(err_code);
    } else {// TODO Should not happen
        rtt_write_string("BLE_ERROR_GATTS_SYS_ATTR_MISSING. CCCD state not defined yet.\n");
        APP_ERROR_CHECK(sd_ble_gatts_sys_attr_set(ble_central_latest_conn, NULL,0, 0));
    }
    if(hvx_sent_length != length) {
        APP_ERROR_CHECK(NRF_ERROR_DATA_SIZE);
    }
}   // TODO check conn handle
