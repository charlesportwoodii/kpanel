#ifndef KPANEL_PANEL_H__
#define KPANEL_PANEL_H__

#include "nrf_drv_gpiote.h"
#include "nrfx_gpiote.h"
#include "nrf_log.h"
#include "panel.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief KPanel settings structure
 */
typedef struct
{
    uint8_t mode;
    bool mode_enabled;
    uint8_t brightness;
    uint8_t temperature;
} kpanel_t;

/**@brief KPanel settings
 */
static volatile kpanel_t kpanel_settings;

/**@brief Gets the PWM settings based upon the brightness and temperature
 * @param[in] brightness    The brightness level (0-255)
 * @param[in] temperature   The temperature level (0-255)
 * @param[out] pma_a        PWM A set for color and brightness
 * @param[out] pma_b        PWM B set for color and brightness
 */
void kpanel_get_pwm(uint8_t brightness, uint8_t temperature, uint8_t *pwm_a, uint8_t *pwm_b);

/**@brief Setings the panel brightness and temperature
 * @param[in] brightness    The brightness level (0-255)
 * @param[in] temperature   The temperature level (0-255)
 */
void kpanel_set(uint8_t brightness, uint8_t temperature);

/**@brief Sets the panel by mode and a multiplier
 * @param[in] mode      The mode to set (1 = brightness, 2 = temperature)
 * @param[in] increase  Whether or not to increase or decrease the settings
 * @param[in] factor    The multipler factor
 */
void kpanel_set_by_factor(uint8_t mode, bool increase, uint8_t factor);

/**@brief Initializes the quad-encoder
 * @param[in] psela     Encoder PIN A
 * @param[in] pselb     Encoder PIN B
 */
void kpanel_qdec_init(uint32_t psela, uint32_t pselb);

/**@brief Event handler for the encoder switch pin
 * @param[in] pin       Encoder switch PIN
 * @param[in] action    The action that occured on the pin
 */
void kpanel_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

/**@brief Initializes a specific encoder switch pin
 * @param[in] pin       Encoder switch PIN
 */
void kpanel_goiote_init_pin(uint8_t pin);

/**@brief Returns the kpanel settings
 */
kpanel_t kpanel_get_settings();

/**@brief Entrypoint
 */
void kpanel_init(void);

#ifdef __cplusplus
}
#endif

#endif // KPANEL_PANEL_H__