#ifndef BLE_KPANEL_H__
#define BLE_KPANEL_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief   Macro for defining ble_kpanel instance
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define BLE_kpanel_DEF(_name) \
static ble_kpanel_t _name; \
NRF_SDH_BLE_OBSERVER(_name ## _obs, \
                     BLE_HRS_BLE_OBSERVER_PRIO, \
                     ble_kpanel_on_ble_evt, &_name)

// kpanel Service UUID Base
// 0626cf96-cbce-419c-92d7-8965e276b5d2
// c5635a47-50eb-4805-a66a-27095f6ec7ae
#define KPANEL_SERVICE_UUID_BASE {{0xAE, 0xC7, 0x6E, 0x5F, 0x09, 0x27, 0x6A, 0xA6, 0x05, 0x48, 0xEB, 0x50, 0x47, 0x5A, 0x63, 0xC5}}

#define KPANEL_SERIVCE_UUID     0x7062

#define KPANEL_PANEL_CHAR_UUID  0x7063

/**@brief Custom Service init structure.
 *        This contains all options and data needed for
 *        initialization of the service.
 */
typedef struct
{
    // Initial security level for Custom characteristics attribute
    ble_srv_cccd_security_mode_t  panel_char_attr_md;
} ble_kpanel_init_t;

/**@brief Custom Service structure.
 *        This contains various status information for the service.
 */
struct ble_kpanel_s
{
    // Handle of Custom Service (as provided by the BLE stack)
    uint16_t                      service_handle;

    // Handles related to the Custom Value characteristic.
    ble_gatts_char_handles_t      panel_value_handles;

    // Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).
    uint16_t                      conn_handle;
    uint8_t                       uuid_type;
};

typedef struct ble_kpanel_s ble_kpanel_t;

/**@brief Function for initializing the Custom Service.
 *
 * @param[out]  p_kpanel       Custom Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_kpanel_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_kpanel_init(ble_kpanel_t * p_kpanel, const ble_kpanel_init_t * p_kpanel_init);


/**@brief Function for adding panel control characteristic
 *
 * @param[in]   p_kpanel        Custom Service structure.
 * @param[in]   p_kpanel_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
uint32_t panel_char_add(ble_kpanel_t * p_kpanel, const ble_kpanel_init_t * p_kpanel_init);


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Service
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 * @param[in]   p_context  Custom Service structure.
 */
void ble_kpanel_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);

#ifdef __cplusplus
}
#endif

#endif // BLE_KPANEL_H__

/** @} */