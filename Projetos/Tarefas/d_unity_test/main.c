#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define INTERNAL_ADC_TEMP_SENSOR_CHANNEL 4

void init_adc(){
    adc_init();
    adc_select_input(INTERNAL_ADC_TEMP_SENSOR_CHANNEL);
}

float temperature_in_Celcius(float adc_read){
    return 27 - ((((adc_read * 3.3) / (1 << 12) - 1) - 0.706) / 0.001721);
}
int main()
{
    stdio_init_all();

    while (true) {
        temperature_in_Celcius(adc_read());
    }
}
