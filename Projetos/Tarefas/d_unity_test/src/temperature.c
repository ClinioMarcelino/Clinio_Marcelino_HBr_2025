#include "temperature.h"

float temperature_in_Celcius(uint16_t adc_reading) {
    return 27 - ((((adc_reading * 3.3f) / (1 << 12) - 1) - 0.706f) / 0.001721f);
}
