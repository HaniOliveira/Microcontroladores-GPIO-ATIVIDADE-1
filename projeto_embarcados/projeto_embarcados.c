#include <stdio.h>
#include "pico/stdlib.h"


#define LEDS 12
#define ROWS 4
#define COLS 4


static char keys[ROWS][COLS] = {
    { '1', '2', '3', 'A' },
    { '4', '5', '6', 'B' },
    { '7', '8', '9', 'C' },
    { '*', '0', '#', 'D' }
};


static uint8_t ledPins[LEDS] = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 28, 27 };

static uint8_t rowPins[ROWS] = { 26, 22, 21, 20 };

static uint8_t colPins[COLS] = { 19, 18, 17, 16 };

void setup() {
    
    for (uint8_t l = 0; l < LEDS; l++) {
        gpio_init(ledPins[l]);
        gpio_set_dir(ledPins[l], GPIO_OUT);
        gpio_put(ledPins[l], 0);
    }

    
    for (uint8_t r = 0; r < ROWS; r++) {
        gpio_init(rowPins[r]);
        gpio_set_dir(rowPins[r], GPIO_OUT);
        gpio_put(rowPins[r], 1);
    }

    
    for (uint8_t c = 0; c < COLS; c++) {
        gpio_init(colPins[c]);
        gpio_set_dir(colPins[c], GPIO_IN);
        gpio_pull_up(colPins[c]);
    }
}

char scanKeypad() {
    for (uint8_t row = 0; row < ROWS; row++) {
        gpio_put(rowPins[row], 0); 

        for (uint8_t col = 0; col < COLS; col++) {
            if (gpio_get(colPins[col]) == 0) { 
                sleep_ms(50);
                while (gpio_get(colPins[col]) == 0); 
                gpio_put(rowPins[row], 1); 
                return keys[row][col];
            }
        }

        gpio_put(rowPins[row], 1); 
    }

    return '\0'; 
}

void loop() {
    char key = scanKeypad();

    if (key != '\0') { 
        switch (key) {
            case '1': gpio_put(ledPins[0], 1); break;
            case '2': gpio_put(ledPins[1], 1); break;
            case '3': gpio_put(ledPins[2], 1); break;
            case '4': gpio_put(ledPins[3], 1); break;
            case '5': gpio_put(ledPins[4], 1); break;
            case '6': gpio_put(ledPins[5], 1); break;
            case '7': gpio_put(ledPins[6], 1); break;
            case '8': gpio_put(ledPins[7], 1); break;
            case '9':
                for (uint8_t l = 0; l < 8; l++) gpio_put(ledPins[l], 1);
                break;
            case '0':
                for (uint8_t l = 0; l < 8; l++) gpio_put(ledPins[l], 0);
                break;
            case 'A': gpio_put(ledPins[8], 1); break;
            case 'B': gpio_put(ledPins[9], 1); break;
            case 'C': gpio_put(ledPins[10], 1); break;
            case 'D': gpio_put(ledPins[11], 1); break;
            case '*':
                for (uint8_t l = 8; l < 12; l++) gpio_put(ledPins[l], 1);
                break;
            case '#':
                for (uint8_t l = 8; l < 12; l++) gpio_put(ledPins[l], 0);
                break;
        }
    }

    sleep_ms(10); 
}

int main() {
    stdio_init_all();
    setup();
    while (1) {
        loop();
    }
}
