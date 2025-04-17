#include <stdio.h>
#include <string.h>
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

#define TEMPO_LIMITE_MS 10000000 //10s 
#define DEBOUNCE_TIME 100000 //100ms 


volatile bool contagem_pontos = false;
volatile bool trigger_jogo = false;
volatile uint8_t b_clicks = 0;
volatile bool atualiza_display = false;

absolute_time_t last_press_a = 0;
absolute_time_t last_press_b = 0;
absolute_time_t ultimo_acao = 0;

char str[32];

void gpio_callback(uint gpio, uint32_t events);
void display_status(uint8_t *ssd, struct render_area *frame_area, int count, int clicks);
void display_welcome_screen(uint8_t *ssd, struct render_area *frame_area);
void init_i2c();
void init_btn(uint pin);

int main() {
    stdio_init_all();
    init_btn(BTN_A);
    init_btn(BTN_B);

    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BTN_B, GPIO_IRQ_EDGE_FALL, true);

    init_i2c();


    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    uint8_t ssd[SSD1306_BUFFER_LENGTH];
    memset(ssd, 0, SSD1306_BUFFER_LENGTH);

    display_welcome_screen(ssd, &frame_area);
    ultimo_acao = get_absolute_time();

    while (1) {
        absolute_time_t now = get_absolute_time();

        // Se 10s sem ação, volta para tela de boas-vindas
        if (!contagem_pontos && absolute_time_diff_us(ultimo_acao, now) > TEMPO_LIMITE_MS) {
            display_welcome_screen(ssd, &frame_area);
            ultimo_acao = now;
        }

        if (trigger_jogo) {
            trigger_jogo = false;
            contagem_pontos = true;
            b_clicks = 0;
            atualiza_display = true;
            ultimo_acao = get_absolute_time();

            int64_t inicio = to_ms_since_boot(get_absolute_time());
            int64_t agora;
            int8_t tempo_restante = 9;

            while (tempo_restante >= 0) {
                agora = to_ms_since_boot(get_absolute_time());
                int novo_tempo = 9 - ((agora - inicio) / 1000);

                if (novo_tempo != tempo_restante) {
                    tempo_restante = novo_tempo;
                    display_status(ssd, &frame_area, tempo_restante, b_clicks);
                }

                if (atualiza_display) {
                    display_status(ssd, &frame_area, tempo_restante, b_clicks);
                    atualiza_display = false;
                }

                sleep_ms(10);
            }

            contagem_pontos = false;
            display_status(ssd, &frame_area, 0, b_clicks);
        }

        sleep_ms(10);
    }
}

void gpio_callback(uint gpio, uint32_t events) {
    absolute_time_t now = get_absolute_time();

    if (gpio == BTN_A) {
        if (absolute_time_diff_us(last_press_a, now) > DEBOUNCE_TIME) {  // 200ms debounce
            trigger_jogo = true;
            ultimo_acao = now;
            last_press_a = now;
        }
    } else if (gpio == BTN_B) {
        if (contagem_pontos && absolute_time_diff_us(last_press_b, now) > 100000) {  // 100ms debounce
            b_clicks++;
            atualiza_display = true;
            ultimo_acao = now;
            last_press_b = now;
        }
    }
}

void display_status(uint8_t *ssd, struct render_area *frame_area, int count, int clicks) {
    memset(ssd, 0, SSD1306_BUFFER_LENGTH);

    snprintf(str, sizeof(str), "Contador: %d", count);
    ssd1306_draw_string(ssd, 5, 10, str);

    snprintf(str, sizeof(str), "Clicks:   %d", clicks);
    ssd1306_draw_string(ssd, 5, 30, str);

    render_on_display(ssd, frame_area);
}

void display_welcome_screen(uint8_t *ssd, struct render_area *frame_area) {
    memset(ssd, 0, SSD1306_BUFFER_LENGTH);

    ssd1306_draw_string(ssd, 20, 10, "Bem vindo!");
    ssd1306_draw_string(ssd, 5, 40, "A para iniciar");
    // (ssd,5"para iniciar");

    render_on_display(ssd, frame_area);
}

void init_i2c() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();
}

void init_btn(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}
