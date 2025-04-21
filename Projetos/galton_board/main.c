#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "inc/ssd1306.h"
#include <string.h>
#include <time.h>

#define I2C_SDA 14
#define I2C_SCL 15
#define SSD1306_BUFFER_LENGTH (ssd1306_width * ssd1306_n_pages)

#define PINS_SPACE 4
#define N_LINHAS 10
#define Y_INICIAL_BOARD 5
#define MOVEMENT 2
#define X_BASE_BOARD ((ssd1306_width / 2) - ((N_LINHAS / 2) * PINS_SPACE))

#define MAX_BALLS 100

struct Bola {
    uint8_t x, y;
    bool active;
};

void init_random() {
    adc_init();
    adc_select_input(0);
    uint16_t noise = adc_read();
    srand(noise);
}

void init_i2c() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();
}

void draw_pins(uint8_t *ssd, struct render_area *frame_area, int8_t n_linhas) {
    for (int linha = 0; linha < n_linhas; linha++) {
        int num_pinos = linha + 1;
        int y = (linha * PINS_SPACE) + Y_INICIAL_BOARD;
        int x_inicio = (ssd1306_width / 2) - 2 * (num_pinos - 1);

        for (int i = 0; i < num_pinos; i++) {
            int x = x_inicio + i * PINS_SPACE;
            ssd1306_set_pixel(ssd, x, y, 1);
        }
    }
}

bool left_or_right() {
    return rand() % 2;
}

void update_ball_position(struct Bola *bola, uint8_t *histograma) {
    if (((bola->y - Y_INICIAL_BOARD) % 4 <= 1) && (bola->y >= 4) && (bola->y <= (N_LINHAS * PINS_SPACE))) {
        if (left_or_right()) {
            bola->x += MOVEMENT;
        } else {
            bola->x -= MOVEMENT;
        }
    }

    bola->y += MOVEMENT;

    if (bola->y > 50) {
        int index = (bola->x - X_BASE_BOARD) / PINS_SPACE;
        if (index >= 0 && index <= N_LINHAS) {
            histograma[index]++;
        }

        // bola->x = ssd1306_width / 2;
        // bola->y = 0;
        bola->active = false;
    }
}


void position_ball(uint8_t *ssd, struct Bola *bola) {
    ssd1306_set_pixel(ssd, bola->x, bola->y, 1);
}

void update_histogram(uint8_t *ssd, uint8_t *histograma) {
    for (int i = 0; i <= N_LINHAS; i++) {
        if (histograma[i] > 0) {
            int x = X_BASE_BOARD + i * PINS_SPACE;
            int y_base = ssd1306_height - 1;

            for (int y = y_base; y > y_base - histograma[i]; y--) {
                if (y >= 0) {
                    ssd1306_set_pixel(ssd, x, y, 1);
                }
            }
        }
    }
}

void spawn_ball(struct Bola *bolas) {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (!bolas[i].active) {
            bolas[i].x = ssd1306_width / 2;
            bolas[i].y = 0;
            bolas[i].active = true;
            break;
        }
    }
}

int main() {
    stdio_init_all();

    init_i2c();
    init_random();

    struct render_area frame_area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

    struct Bola bolas[MAX_BALLS];
    for (int i = 0; i < MAX_BALLS; i++) {
        bolas[i].active = false;
    }

    uint8_t histograma[N_LINHAS + 1] = {0};
    int count_ticks = 0;

    while (true) {
        memset(ssd, 0, ssd1306_buffer_length);

        draw_pins(ssd, &frame_area, N_LINHAS);

        for (int i = 0; i < MAX_BALLS; i++) {
            if (bolas[i].active) {
                position_ball(ssd, &bolas[i]);
                update_ball_position(&bolas[i], histograma);
            }
        }

        update_histogram(ssd, histograma);
        render_on_display(ssd, &frame_area);

        if (count_ticks++ > 5) {
            spawn_ball(bolas);
            count_ticks = 0;
        }

        sleep_ms(10);
    }

    return 0;
}
