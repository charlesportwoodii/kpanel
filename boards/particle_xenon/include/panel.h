/**
 * Copyright (c) 2019-Present Charles R. Portwood II <charlesportwoodii@erianna.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PANEL_H
#define PANEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_gpio.h"

#define ENCODER_1TOGGLE     NRF_GPIO_PIN_MAP(0,3)   // A0
#define ENCODER_1A          NRF_GPIO_PIN_MAP(0,4)   // A1
#define ENCODER_1B          NRF_GPIO_PIN_MAP(0,28)  // A2

#define ENCODER_2TOGGLE     NRF_GPIO_PIN_MAP(0,29)  // A3
#define ENCODER_2A          NRF_GPIO_PIN_MAP(0,30)  // A4
#define ENCODER_2B          NRF_GPIO_PIN_MAP(0,31)  // A5

#define KPANEL_PWM0         NRF_GPIO_PIN_MAP(1,03)  // D8
#define KPANEL_PWM1         NRF_GPIO_PIN_MAP(1,12)  // D7

#ifdef __cplusplus
}
#endif

#endif // PANEL_H
