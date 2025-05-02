#include "unity.h"
#include "temperature.h"

void test_temperature_in_Celsius_zero(void) {
    float temp = temperature_in_Celcius(0);
    TEST_ASSERT_FLOAT_WITHIN(5.0, 27.0 - ((-1.0 - 0.706f)/0.001721f), temp);
}

void test_temperature_in_Celsius_max_adc(void) {
    float temp = temperature_in_Celcius(4095);
    TEST_ASSERT_TRUE(temp < 0.0f); 
}

void test_temperature_in_Celsius_typical_adc(void) {
    float temp = temperature_in_Celcius(2000);
    TEST_ASSERT_FLOAT_WITHIN(10.0, 22.0f, temp);
}
