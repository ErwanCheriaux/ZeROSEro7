#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_assert.h"
#include "app_error.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advertising.h"
#include "ble_advdata.h"
#include "ble_hids.h"
#include "ble_bas.h"
#include "ble_dis.h"
#include "ble_conn_params.h"
#include "bsp_btn_ble.h"
#include "app_scheduler.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"
#include "nrf_sdh_ble.h"
#include "app_timer.h"
#include "peer_manager.h"
#include "fds.h"
#include "ble_conn_state.h"
#include "nrf_ble_gatt.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define PNP_ID_VENDOR_ID_SOURCE 0x00C3 /**< Vendor ID Source. */
#define PNP_ID_VENDOR_ID 0x1915        /**< Vendor ID. */
#define PNP_ID_PRODUCT_ID 0xEEEE       /**< Product ID. */
#define PNP_ID_PRODUCT_VERSION 0x0001  /**< Product Version. */

#define MANUFACTURER_NAME "adidas ag" /**< Manufacturer. Will be passed to Device Information Service. */

NRF_BLE_GATT_DEF(m_gatt); /**< GATT module instance. */

void device_information_service_init()
{
    ret_code_t       err_code;
    ble_dis_init_t   dis_init_obj;
    ble_dis_pnp_id_t pnp_id;

    pnp_id.vendor_id_source = PNP_ID_VENDOR_ID_SOURCE;
    pnp_id.vendor_id        = PNP_ID_VENDOR_ID;
    pnp_id.product_id       = PNP_ID_PRODUCT_ID;
    pnp_id.product_version  = PNP_ID_PRODUCT_VERSION;

    memset(&dis_init_obj, 0, sizeof(dis_init_obj));

    ble_srv_ascii_to_utf8(&dis_init_obj.manufact_name_str, MANUFACTURER_NAME);
    dis_init_obj.p_pnp_id = &pnp_id;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init_obj.dis_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init_obj.dis_attr_md.write_perm);

    APP_ERROR_CHECK(ble_dis_init(&dis_init_obj));
}

// Copied from Little Brothers
void my_service_init() {
    ble_uuid128_t ms_base_uuid = BLE_UUID_BASE;

    // Initialize the service structure
    p_my_service->conn_handle = BLE_CONN_HANDLE_INVALID;

    // Add a new vendor specific UUID to the softdevice table
    ble_uuid_t ms_uuid;
    ret_code_t err_code =
        sd_ble_uuid_vs_add(&ms_base_uuid, &p_my_service->uuid_type);
    APP_ERROR_CHECK(err_code);
    ms_uuid.type = p_my_service->uuid_type;
    ms_uuid.uuid = BLE_UUID_MY_SERVICE;

    // Add the service to the GATT
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ms_uuid,
                                        &p_my_service->service_handle);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Service UUID: 0x%#04x\n", ms_uuid.uuid);
    NRF_LOG_INFO("Service UUID type: 0x%#02x\n", ms_uuid.type);
    NRF_LOG_INFO("Service handle: 0x%#04x\n", p_my_service->service_handle);

    // UUID
    ble_uuid_t ble_uuid;
    ble_uuid.type = p_my_service->uuid_type;
    ble_uuid.uuid = BLE_UUID_WRITE_CHAR;

    // Client Characteristic Configuration Descriptor Metadata
    ble_gatts_attr_md_t cccd_md = {{0}}; // Double bracket for old gcc version
    // Set GAP connection security to open for read and write permissions
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    // Storing the CCCD value in stack (softdevice) memory
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    // Characteristic Metadata
    ble_gatts_char_md_t char_md = {{0}}; // Double bracket for old gcc version
    char_md.char_props.write = 1;
    char_md.char_props.read = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = &cccd_md;
    char_md.char_props.notify = 1;
    char_md.p_sccd_md = NULL;

    // Attribute Metadata
    ble_gatts_attr_md_t attr_md = {{0}}; // Double bracket for old gcc version
    // Set GAP connection security to open for read and write permissions
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    // Storing the attribute value in stack (softdevice) memory
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 1;

    // Characteristic Value Attribute
    ble_gatts_attr_t attr_char_value = {0};
    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = BLE_MS_MAX_WRITE_CHAR_LEN;

    APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(p_my_service->service_handle,
                                           &char_md, &attr_char_value,
                                           &p_my_service->write_handles));
}

void ble_services_init()
{
    device_information_service_init();
    my_service_init();
}

void ble_gatt_init()
{
    APP_ERROR_CHECK(nrf_ble_gatt_init(&m_gatt, NULL));
}
