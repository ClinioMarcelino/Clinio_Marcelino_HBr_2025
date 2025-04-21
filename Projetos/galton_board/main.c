#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include <string.h>

#define I2C_SDA 14
#define I2C_SCL 15
#define SSD1306_BUFFER_LENGTH (ssd1306_width * ssd1306_n_pages)

#define PINS_SPACE 4
#define N_LINHAS 10
#define Y_INICIAL_BOARD 5

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

uint8_t g_board_pins_local[128][64] = {0};

int main(){
    stdio_init_all();
    
    init_i2c();

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

    int y=0;
    int z=10;
   
    while (true) {
        memset(ssd, 0 , ssd1306_buffer_length);
        draw_pins(ssd,&frame_area,10);
        ssd1306_set_pixel(ssd,64,y,1);
        // ssd1306_set_pixel(ssd,64,z,1);

        
        



        
        render_on_display(ssd,&frame_area);

        if(y>50)
            y=0;
        else
            y+=2;
        

            sleep_ms(100);
    }

    // int d,e = 0;

    // while (true) {
    //     char r = rand() % 2;

    //     if(r)
    //         d++;
    //     else
    //         e++;
        
    //     memset(ssd, 0 , ssd1306_buffer_length);
    //     snprintf(str, sizeof(str), "%d .", r);
    //     ssd1306_draw_string(ssd, 5, 20, str);

    //     snprintf(str, sizeof(str), "D %d  E  %d", d,e);
    //     ssd1306_draw_string(ssd, 5,50, str);

    //     render_on_display(ssd, &frame_area);

    //     sleep_ms(100);
    // }
}
