#include "ble_kpanel.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "bsp.h"

#include "sdk_common.h"
#include "ble_srv_common.h"
#include "nrf_gpio.h"
#include "nrf.h"
#include "boards.h"
#include "nrf_log.h"

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_kpanel       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_kpanel_t * p_kpanel, ble_evt_t const * p_ble_evt)
{
    p_kpanel->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_kpanel       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_kpanel_t * p_kpanel, ble_evt_t const * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_kpanel->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_kpanel       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_kpanel_t * p_kpanel, ble_evt_t const * p_ble_evt)
{
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    if (p_evt_write->handle == p_kpanel->panel_value_handles.value_handle) {
        NRF_LOG_DEBUG("Panel Info %d %d", p_evt_write->data[0], p_evt_write->data[1]);
    }
}

uint32_t ble_kpanel_init(
    ble_kpanel_t * p_kpanel,
    const ble_kpanel_init_t * p_kpanel_init
)
{
    uint32_t      err_code;
    ble_uuid_t    ble_uuid;
    ble_uuid128_t kpanel_base_uuid = KPANEL_SERVICE_UUID_BASE;

    VERIFY_PARAM_NOT_NULL(p_kpanel);
    VERIFY_PARAM_NOT_NULL(p_kpanel_init);

    // Initialize the service structure.
    p_kpanel->conn_handle             = BLE_CONN_HANDLE_INVALID;

    /**@snippet [Adding proprietary Service to S110 SoftDevice] */
    // Add a custom base UUID.
    err_code = sd_ble_uuid_vs_add(&kpanel_base_uuid, &p_kpanel->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_kpanel->uuid_type;
    ble_uuid.uuid = KPANEL_SERIVCE_UUID;

    // Add the service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_kpanel->service_handle);

    VERIFY_SUCCESS(err_code);

    err_code = panel_char_add(p_kpanel, p_kpanel_init);
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}

uint32_t panel_char_add(
    ble_kpanel_t * p_kpanel,
    const ble_kpanel_init_t * p_kpanel_init
)
{
    VERIFY_PARAM_NOT_NULL(p_kpanel);
    VERIFY_PARAM_NOT_NULL(p_kpanel_init);

    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    //ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    static char user_desc[] = "LED Panel";
    char_md.p_char_user_desc  = (uint8_t *) user_desc;
    char_md.char_user_desc_size = strlen(user_desc);
    char_md.char_user_desc_max_size = strlen(user_desc);

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_kpanel_init->panel_char_attr_md.read_perm;
    attr_md.write_perm = p_kpanel_init->panel_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    ble_uuid.type = p_kpanel->uuid_type;
    ble_uuid.uuid = KPANEL_PANEL_CHAR_UUID;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t[2]);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint8_t[2]);

    err_code = sd_ble_gatts_characteristic_add(p_kpanel->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_kpanel->panel_value_handles);
    VERIFY_SUCCESS(err_code);
    return NRF_SUCCESS;
}

void ble_kpanel_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_kpanel_t * p_kpanel = (ble_kpanel_t *) p_context;
    if (p_kpanel == NULL || p_ble_evt == NULL) {
        return;
    }

    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_kpanel, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_kpanel, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
           on_write(p_kpanel, p_ble_evt);
           break;

        default:
            // No implementation needed.
            break;
    }
}