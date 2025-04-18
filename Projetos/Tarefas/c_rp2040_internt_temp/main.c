#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "inc/ssd1306.h"

#define I2C_SDA 14
#define I2C_SCL 15
#define SSD1306_BUFFER_LENGTH (ssd1306_width * ssd1306_n_pages)

float read_temperature_celsius() {
    adc_select_input(4); // Canal 4 é o sensor de temperatura interno
    uint16_t adc_raw = adc_read();

    // Converta o valor ADC (12 bits) para tensão
    const float conversion_factor = 3.3f / (1 << 12); // 3.3V / 4096
    float voltage = adc_raw * conversion_factor;

    // Fórmula para converter a tensão em temperatura
    float temperature_c = 27.0f - (voltage - 0.706f) / 0.001721f;

    return temperature_c;
}

int main() {
    stdio_init_all(); 
    adc_init();       

    adc_set_temp_sensor_enabled(true); 

    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);  
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0 , ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    char str[32];

    while (true) {

        snprintf(str, sizeof(str), "%.2f C", read_temperature_celsius());
        ssd1306_draw_string(ssd, 30, 300, str);

        render_on_display(ssd, &frame_area);
        sleep_ms(200);
    }

    return 0;
}
