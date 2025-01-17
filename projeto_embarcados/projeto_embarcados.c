#include <stdio.h>
#include "pico/stdlib.h"


#define LED1 11
#define LED2 12
#define LED3 13


#define BUZZER 28


uint8_t columns[4] = {4, 3, 2, 1};
uint8_t rows[4] = {8, 7, 6, 5};


char KEY_MAPS[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'};


void init_leds() {
    gpio_init(LED1);
    gpio_init(LED2);
    gpio_init(LED3);
    gpio_set_dir(LED1, GPIO_OUT);
    gpio_set_dir(LED2, GPIO_OUT);
    gpio_set_dir(LED3, GPIO_OUT);
}


void init_keypad() {
    for (uint8_t i = 0; i < 4; i++) {
        gpio_init(columns[i]);
        gpio_set_dir(columns[i], GPIO_OUT);
        gpio_put(columns[i], 1);

        gpio_init(rows[i]);
        gpio_set_dir(rows[i], GPIO_IN);
        gpio_pull_up(rows[i]);
    }
}


char get_key() {
    for (uint8_t col = 0; col < 4; col++) {
        gpio_put(columns[col], 0);
        for (uint8_t row = 0; row < 4; row++) {
            if (gpio_get(rows[row]) == 0) {
                sleep_ms(50); 
                if (gpio_get(rows[row]) == 0) {
                    gpio_put(columns[col], 1);
                    return KEY_MAPS[row * 4 + col];
                }
            }
        }
        gpio_put(columns[col], 1);
    }
    return '\0';
}


int main() {
    stdio_init_all();
    init_leds();
    init_keypad();

    printf("Sistema inicializado.\n");

    while (1) {
        char key = get_key();
        if (key != '\0') {
            printf("Tecla pressionada: %c\n", key);

            
            gpio_put(LED1, key == '1');
            gpio_put(LED2, key == '2');
            gpio_put(LED3, key == '3');
        }
        sleep_ms(100);
    }

    return 0;
}
