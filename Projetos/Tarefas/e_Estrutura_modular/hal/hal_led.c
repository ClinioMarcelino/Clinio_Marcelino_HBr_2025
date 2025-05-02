#include "hal_led.h"
#include "led_embutido.h"

void hal_led_init(void) {
    led_embutido_init();
}

void hal_led_toggle(void) {
    led_embutido_toggle();
}
