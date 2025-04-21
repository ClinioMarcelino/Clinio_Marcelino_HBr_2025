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

struct Bola{
    uint8_t x,y;
};

uint8_t g_board_pins_local[128][64] = {0};

void init_random() {
    adc_init();
    adc_select_input(0); // suponha que o ADC0 esteja livre

    uint16_t noise = adc_read(); // Leitura de ruído do ADC
    srand(noise); // Usa o ruído como semente
}

void init_i2c(){
   
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

  
    ssd1306_init();
}

void draw_pins(uint8_t *ssd, struct render_area *frame_area, int8_t n_linhas){
    uint8_t linha = 0;
    while(linha<n_linhas){
    int8_t num_pinos = linha + 1;
    int8_t y = (linha * PINS_SPACE) + Y_INICIAL_BOARD;

    int8_t x_inicio = (ssd1306_width / 2) - 2*(num_pinos-1);

    for(int i = 0; i < num_pinos; i++) {
        int x = x_inicio + i * PINS_SPACE;
        ssd1306_set_pixel(ssd, x, y,1); 
    }
    linha++;
    }
}

// returns 0 for Left and 1 for Right
int left_or_right(){
    int a = rand();
    int b = rand();

    // Uma operação simples para combinar a e b, mas garantindo uma boa distribuição
    int combined = (a * 31 + b * 17) % 100; // Modifique os coeficientes conforme necessário

    return combined % 2; // Retorna 0 ou 1
}

void update_ball_position(struct Bola *bola){
    if(((bola->y) - Y_INICIAL_BOARD) % 4 <= 1 && (bola->y >=4) && (bola->y <= (N_LINHAS*PINS_SPACE))){
        if(left_or_right()){
            bola->x += MOVEMENT;
        }
        else
            bola->x -= MOVEMENT;
    }
    bola->y += MOVEMENT;

    if (bola->y > 50){
        bola->y = 0;
        bola->x = ssd1306_width / 2;
    }
}

void position_ball(uint8_t *ssd, struct Bola *bola){
    ssd1306_set_pixel(ssd, bola->x, bola->y,1);
}

int main(){
    stdio_init_all();
    
    init_i2c();
    init_random();

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

    struct Bola ball = {
        x:ssd1306_width/2,
        y:0
    };

   
    while (true) {
        memset(ssd, 0 , ssd1306_buffer_length);
        draw_pins(ssd,&frame_area,10);
        
        position_ball(ssd, &ball);
          
        render_on_display(ssd,&frame_area);

        update_ball_position(&ball);
        sleep_ms(250);
    }

}
