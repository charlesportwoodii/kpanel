#include "kpanel_panel.h"
#include "nrf_drv_gpiote.h"
#include "nrfx_gpiote.h"
#include "nrfx_qdec.h"
#include "nrf_drv_qdec.h"
#include "panel.h"
#include <stdio.h>
#include <math.h>

void kpanel_get_pwm(uint8_t brightness, uint8_t temperature, uint8_t *pwm_a, uint8_t *pwm_b)
{
    uint8_t a = ceil((double)temperature * 100 / 256);
    uint8_t b = 100 - (int)a;

    double brightnessFactor = ((double)brightness * 100 / 256) / 100;

    (*pwm_a) = ceil((double)a * brightnessFactor);
    (*pwm_b) = ceil((double)b * brightnessFactor);
}

void kpanel_set(uint8_t brightness, uint8_t temperature)
{
    brightness = MAX(0, MIN(brightness, 255));
    temperature = MAX(0, MIN(temperature, 255));

    kpanel_settings.temperature = temperature;
    kpanel_settings.brightness = brightness;

    uint8_t pwm_a = 0;
    uint8_t pwm_b = 0;
    kpanel_get_pwm(brightness, temperature, &pwm_a, &pwm_b);

    NRF_LOG_DEBUG("Settings changed: brightness: %d, color_temp: %d, pwm_a: %d, pwm_b: %d",
        kpanel_settings.brightness,
        kpanel_settings.temperature,
        pwm_a,
        pwm_b
    );
}

void kpanel_set_by_factor(uint8_t mode, bool increase, uint8_t factor)
{
    uint8_t brightness = kpanel_settings.brightness;
    uint8_t temperature = kpanel_settings.temperature;

    if (mode == 1) {
        kpanel_set(increase ? MIN((brightness + factor), 255) : MAX((brightness - factor), 0), temperature);
    } else {
        kpanel_set(brightness, increase ? MIN((temperature + factor), 255) : MAX((temperature - factor), 0));
    }
}

static void kpanel_qdec_handler(nrf_drv_qdec_event_t event)
{
    if (event.type == NRF_QDEC_EVENT_REPORTRDY) {
        kpanel_set_by_factor(kpanel_settings.mode, (event.data.report.acc >= 1), 2);
    }
}

void kpanel_qdec_init(uint32_t psela, uint32_t pselb)
{
    kpanel_settings.mode_enabled = true;
    nrf_drv_qdec_config_t config = NRFX_QDEC_DEFAULT_CONFIG;
    config.psela = psela;
    config.pselb = pselb;
    config.pselled = NRF_QDEC_LED_NOT_CONNECTED;
    nrf_drv_qdec_init(&config, kpanel_qdec_handler);
    nrf_drv_qdec_enable();
}

void kpanel_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if (kpanel_settings.mode != 0) {
        nrf_drv_qdec_uninit();
    }

    switch ((uint32_t)pin) {
        case ENCODER_1TOGGLE:
            kpanel_settings.mode = 1;
            kpanel_qdec_init(ENCODER_1A, ENCODER_1B);
            break;
        case ENCODER_2TOGGLE:
            kpanel_settings.mode = 2;
            kpanel_qdec_init(ENCODER_2A, ENCODER_2B);
            break;
        default:
            break;
    }

    NRF_LOG_DEBUG("Mode changed to %s", (kpanel_settings.mode == 1 ? "brightness": "color_temp"));
}

void kpanel_goiote_init_pin(uint8_t pin)
{
    ret_code_t err_code;
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_HITOLO(false);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(pin, &in_config, kpanel_pin_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(pin, true);
}

kpanel_t kpanel_get_settings()
{
    return kpanel_settings;
}

void kpanel_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    kpanel_settings.mode = 0;
    kpanel_settings.mode_enabled = false;
    kpanel_settings.temperature = 0;
    kpanel_settings.brightness = 0;

    kpanel_goiote_init_pin(ENCODER_1TOGGLE);
    kpanel_goiote_init_pin(ENCODER_2TOGGLE);
}