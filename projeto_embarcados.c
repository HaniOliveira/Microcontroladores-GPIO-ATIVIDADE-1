#include <stdio.h>
#include "pico/stdlib.h"

#define LED_AZUL 11
#define LED_VERDE 12
#define LED_VERMELHO 13
#define BUZZER 21

uint8_t columns[4] = {1, 2, 3, 4};
uint8_t rows[4] = {5, 6, 7, 8};

char KEY_MAPS[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'};

void init_leds()
{
    // Array para armazenar os pinos dos LEDs
    const uint led_pins[] = {LED_AZUL, LED_VERDE, LED_VERMELHO};
    // Número de LEDs no array
    const uint num_leds = sizeof(led_pins) / sizeof(led_pins[0]);

    // Loop para inicializar cada LED
    for (uint i = 0; i < num_leds; i++)
    {
        gpio_init(led_pins[i]);
        gpio_set_dir(led_pins[i], GPIO_OUT);
    }
}
void init_keypad()
{
    for (uint8_t i = 0; i < 4; i++)
    {
        gpio_init(columns[i]);
        gpio_set_dir(columns[i], GPIO_OUT);
        gpio_put(columns[i], 1);

        gpio_init(rows[i]);
        gpio_set_dir(rows[i], GPIO_IN);
        gpio_pull_up(rows[i]);
    }
}

char get_key()
{
    for (uint8_t col = 0; col < 4; col++)
    {
        gpio_put(columns[col], 0);
        for (uint8_t row = 0; row < 4; row++)
        {
            if (gpio_get(rows[row]) == 0)
            {
                sleep_ms(50);
                if (gpio_get(rows[row]) == 0)
                {
                    gpio_put(columns[col], 1);
                    return KEY_MAPS[row * 4 + col];
                }
            }
        }
        gpio_put(columns[col], 1);
    }
    return '\0';
}

/*Função que configura uma frequência (Tom) e uma duração em milissegundos para o toque do buzzer*/
void play_tone(uint16_t frequencia, uint16_t duracao)
{

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);

    // Calcula o período do sinal em microssegundos
    uint32_t periodo = 1000000 / frequencia; // Período em microssegundos
    uint32_t metade_perodo = periodo / 2;    // Metade do período

    for (uint16_t i = 0; i < (duracao * 1000) / periodo; i++)
    {                            // Duração em milissegundos
        gpio_put(BUZZER, 1);     // Liga o buzzer
        sleep_us(metade_perodo); // Espera metade do período
        gpio_put(BUZZER, 0);     // Desliga o buzzer
        sleep_us(metade_perodo); // Espera metade do período
    }
}

void led_sequence_and_buzzer()
{
    // Acende e apaga os LEDs em sequência
    gpio_put(LED_AZUL, 1);
    sleep_ms(250);
    gpio_put(LED_AZUL, 0);

    gpio_put(LED_VERDE, 1);
    sleep_ms(250);
    gpio_put(LED_VERDE, 0);

    gpio_put(LED_VERMELHO, 1);
    sleep_ms(250);
    gpio_put(LED_VERMELHO, 0);
}

void leds_acessos_tres()
{
    // Acende os 3 leds de uma só vez e apaga após 2000ms

    gpio_put(LED_AZUL, 1);
    gpio_put(LED_VERDE, 1);
    gpio_put(LED_VERMELHO, 1);
    
    sleep_ms(2000);

    gpio_put(LED_AZUL, 0);
    gpio_put(LED_VERDE, 0);
    gpio_put(LED_VERMELHO, 0);
}

int main()
{
    stdio_init_all();
    init_leds();
    init_keypad();

    printf("Sistema inicializado.\n");

    while (1)
    {
        char key = get_key();
        if (key != '\0')
        {
            printf("Tecla pressionada: %c\n", key);

            // Switch case para controlar os LEDs
            switch (key)
            {
            case '1':
                gpio_put(LED_AZUL, 1);
                gpio_put(LED_VERDE, 0);
                gpio_put(LED_VERMELHO, 0);
                break;
            case '2':
                gpio_put(LED_AZUL, 0);
                gpio_put(LED_VERDE, 1);
                gpio_put(LED_VERMELHO, 0);
                break;
            case '3':
                gpio_put(LED_AZUL, 0);
                gpio_put(LED_VERDE, 0);
                gpio_put(LED_VERMELHO, 1);
                break;
            // Adicione mais casos para outras teclas se necessário
            default:
                gpio_put(LED_AZUL, 0);
                gpio_put(LED_VERDE, 0);
                gpio_put(LED_VERMELHO, 0); // Desliga todos por padrão
                break;
            }
            // Chama a função quando a tecla '7' é pressionada
            if (key == '7')
            {
                printf("Acionando a função: led_sequence_and_buzzer()\n\n");
                led_sequence_and_buzzer();

                // Toca o buzzer por 1000ms a uma frequência de 1000 Hz
                play_tone(1000, 1000);
            }

            // Chama a função quando a tecla '8' é pressionada
            if (key == '8')
            {
                printf("Acionando a função: leds_acessos_tres()\n\n");
                leds_acessos_tres();

            }

        }
        sleep_ms(100);
    }

    return 0;
}
