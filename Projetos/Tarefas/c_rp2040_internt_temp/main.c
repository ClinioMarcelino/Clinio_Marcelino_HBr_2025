#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

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
    stdio_init_all(); // Inicializa a UART para debug
    adc_init();       // Inicializa o ADC

    adc_set_temp_sensor_enabled(true); // Ativa o sensor interno

    while (true) {
        float temperature = read_temperature_celsius();
        printf("Temperatura: %.2f °C\n", temperature);
        sleep_ms(1000);
    }

    return 0;
}
