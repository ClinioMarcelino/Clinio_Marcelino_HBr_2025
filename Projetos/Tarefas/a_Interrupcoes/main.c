#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "pico/time.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"


#define I2C_SDA 14
#define I2C_SCL 15
#define SSD1306_BUFFER_LENGTH (ssd1306_width * ssd1306_n_pages)

#define BTN_B 6
#define BTN_A 5


volatile bool counting_active = false;
volatile bool reset_triggered = false;
volatile int b_clicks = 0;
volatile bool debounce_a = false;
volatile bool debounce_b = false;

void gpio_callback(uint gpio, uint32_t events);
void display_status(int count, int clicks);
void init_i2c();

int main() {
    stdio_init_all();
    gpio_init(BTN_A);
    gpio_init(BTN_B);

    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_set_dir(BTN_B, GPIO_IN);

    gpio_pull_up(BTN_A);
    gpio_pull_up(BTN_B);

    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BTN_B, GPIO_IRQ_EDGE_FALL, true);

    init_i2c();

    int counter = 0;

    while (1) {
        if (reset_triggered) {
            reset_triggered = false;
            counting_active = true;
            b_clicks = 0;
            counter = 9;

            for (; counter >= 0; counter--) {
                display_status(counter, b_clicks);
                sleep_ms(1000);
            }

            counting_active = false;
            display_status(0, b_clicks);
        }
        sleep_ms(10); 
    }
}

void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BTN_A) {
        if (!debounce_a) {
            debounce_a = true;
            reset_triggered = true;
            // Debounce simples
            sleep_ms(200);
            debounce_a = false;
        }
    } else if (gpio == BTN_B) {
        if (counting_active && !debounce_b) {
            debounce_b = true;
            b_clicks++;
            sleep_ms(100);
            debounce_b = false;
        }
    }
}

void display_status(int count, int clicks) {
    char buffer[32];
    ssd1306_clear();
    sprintf(buffer, "Contador: %d", count);
    ssd1306_draw_string(0, 0, buffer, 1);
    sprintf(buffer, "B Clicks: %d", clicks);
    ssd1306_draw_string(0, 16, buffer, 1);
    
}

void init_i2c(){
   
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

  
    ssd1306_init();
}