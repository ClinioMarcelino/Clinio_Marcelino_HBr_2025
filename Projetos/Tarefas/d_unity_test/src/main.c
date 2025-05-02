#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "temperature.h"

#define INTERNAL_ADC_TEMP_SENSOR_CHANNEL 4

void init_adc(void) {
    adc_init();
    adc_select_input(INTERNAL_ADC_TEMP_SENSOR_CHANNEL);
}

int main()
{
    stdio_init_all();
    init_adc();
    
    while (true) {
        uint16_t adc_val = adc_read();
        float temp_celsius = temperature_in_Celcius(adc_val);
        printf("Temperature: %.2f C\n", temp_celsius);
        sleep_ms(1000);
    }
}
