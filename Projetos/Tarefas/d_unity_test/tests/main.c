#include "unity.h"

extern void test_temperature_in_Celsius_zero(void);
extern void test_temperature_in_Celsius_max_adc(void);
extern void test_temperature_in_Celsius_typical_adc(void);

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_temperature_in_Celsius_zero);
    RUN_TEST(test_temperature_in_Celsius_max_adc);
    RUN_TEST(test_temperature_in_Celsius_typical_adc);
    return UNITY_END();
}
