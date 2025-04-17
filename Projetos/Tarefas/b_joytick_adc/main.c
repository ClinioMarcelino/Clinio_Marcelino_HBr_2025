#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"

#define I2C_SDA 14
#define I2C_SCL 15
#define SSD1306_BUFFER_LENGTH (ssd1306_width * ssd1306_n_pages)

#define ADC_PICO 26
#define JSK_X_CHANNEL 1
#define JSK_Y_CHANNEL 0
#define JSK_X_PIN (ADC_PICO + JSK_X_CHANNEL)
#define JSK_Y_PIN (ADC_PICO + JSK_X_CHANNEL)

void init_i2c();

int main()
{
    stdio_init_all();

    init_adc();
    init_i2c();

    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    char str[32];

    while (true) {
        adc_select_input(JSK_Y_CHANNEL);
        snprintf(str, sizeof(str), "Joystick Y  %d", adc_read());
        ssd1306_draw_string(ssd, 5, 20, str);

        adc_select_input(JSK_X_CHANNEL);
        snprintf(str, sizeof(str), "Joystick X  %d", adc_read());
        ssd1306_draw_string(ssd, 5, 40, str);


        render_on_display(ssd, &frame_area);
        sleep_ms(10);
    }
}

void init_i2c(){
   
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

  
    ssd1306_init();
}

void init_adc(){
    adc_init();       

    adc_gpio_init(JSK_X_PIN);
    adc_gpio_init(JSK_Y_PIN);

}


