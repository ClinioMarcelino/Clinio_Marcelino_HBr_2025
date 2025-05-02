#ifndef LED_EMBUTIDO_H
#define LED_EMBUTIDO_H

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

void led_embutido_init(void);
void led_embutido_set(bool state);
void led_embutido_toggle(void);

#endif 
