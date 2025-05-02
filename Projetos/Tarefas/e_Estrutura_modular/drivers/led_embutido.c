#include "led_embutido.h"

void led_embutido_init(void) {
    cyw43_arch_init();
}

void led_embutido_set(bool state) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state);
}

void led_embutido_toggle(void) {
    static bool state = false;
    state = !state;
    led_embutido_set(state);
}
