#include <stdio.h>
#include "pico/stdlib.h"
#include "lib/matriz.h"
#include "hardware/i2c.h"
#include "bh1750_light_sensor.h"

// Para o sensor de luz BH1750. Endereço 0x23
#define I2C_PORT i2c0               // i2c0 pinos 0 e 1, i2c1 pinos 2 e 3
#define I2C_SDA 0                   // 0 ou 2
#define I2C_SCL 1                   // 1 ou 3

// //função para exibir dados no display
// void display_dados (){

//     //mostrar dados de tempo e intensidade da luz (leitura)

//     //strings para mostrar dados - 7 ao todo
//     char str_pressao[5]; //para a pressão
//     int pressaok = pressao;
//     sprintf(str_pressao,"%d",pressaok);
//     char str_temp[5];
//     sprintf(str_temp,"%.2f",temperatura_final);
//     char str_umi[5];
//     sprintf(str_umi,"%.2f",umidade);
//     char str_min_umi[5];
//     sprintf(str_min_umi,"%.0f",min_umidade);
//     char str_max_umi[5];
//     sprintf(str_max_umi,"%.0f",max_umidade);
//     char str_min_temp[5];
//     sprintf(str_min_temp,"%.0f",min_temperatura);
//     char str_max_temp[5];
//     sprintf(str_max_temp,"%.0f",max_temperatura);

//     //  Atualiza o conteúdo do display com as informações necessárias
//     ssd1306_fill(&ssd, !cor);                          // Limpa o display
//     ssd1306_rect(&ssd, 3, 1, 122, 61, cor, !cor);      // Desenha um retângulo ok
//     ssd1306_line(&ssd, 3, 13, 123, 13, cor);           // Desenha uma linha ok
//     ssd1306_line(&ssd, 3, 23, 123, 23, cor);           // Desenha uma linha
//     ssd1306_line(&ssd, 3, 33, 123, 33, cor);           // Desenha uma linha
//     ssd1306_line(&ssd, 3, 43, 123, 43, cor);           // Desenha uma linha
//     ssd1306_line(&ssd, 3, 53, 123, 53, cor);           // Desenha uma linha
//     ssd1306_draw_string(&ssd, "WI-FI:", 22, 5); 
//     if(wifi_connection)                         //Mostra status de conexão
//         ssd1306_draw_string(&ssd, "ON", 78, 5); 
//     else
//         ssd1306_draw_string(&ssd, "branco", 78, 5); // Desenha uma string
//     //dados de umidade
//     ssd1306_draw_string(&ssd, "Umi.:", 10, 15);  
//     ssd1306_draw_string(&ssd, str_umi, 60, 15);  
//     ssd1306_draw_string(&ssd, "%", 100, 15);  
//     ssd1306_draw_string(&ssd, "Mx:", 4, 25);  
//     ssd1306_draw_string(&ssd, str_max_umi, 27, 25);
//     ssd1306_draw_string(&ssd, "Mn:", 64, 25); 
//     ssd1306_draw_string(&ssd, str_min_umi, 89, 25);
//     //dados de temperatura
//     ssd1306_draw_string(&ssd, "Temp.:", 10, 35); 
//     ssd1306_draw_string(&ssd, str_temp, 60, 35);  
//     ssd1306_draw_string(&ssd, "C", 100, 35);  
//     ssd1306_draw_string(&ssd, "Mx:", 4, 45);   
//     ssd1306_draw_string(&ssd, str_max_temp, 27, 45);
//     ssd1306_draw_string(&ssd, "Mn:", 64, 45);  
//     ssd1306_draw_string(&ssd, str_min_temp, 89, 45);
//     //dados de pressão
//     ssd1306_draw_string(&ssd, "Pres.:", 10, 55); 
//     ssd1306_draw_string(&ssd, str_pressao, 60, 55);  
//     ssd1306_draw_string(&ssd, "kPa", 90, 55);  
    
//     ssd1306_line(&ssd, 62, 23, 62, 33, cor);           // Desenha uma linha vertical umidade
//     ssd1306_line(&ssd, 62, 43, 62, 53, cor);           // Desenha uma linha vertical temperatura
//     ssd1306_send_data(&ssd);                           // Atualiza o display
//     sleep_ms(700);

// };

// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
#define botaoB 6
void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}

void matriz(uint intensidade){
    COR_RGB branco = {0.5*intensidade, 0.5*intensidade, 0.5*intensidade};
    //matriz que será modificada no loop
    Matriz_leds painel_luminoso = {{branco,branco,branco,branco,branco},{branco,branco,branco,branco,branco},{branco,branco,branco,branco,branco},{branco,branco,branco,branco,branco},{branco,branco,branco,branco,branco}};
    acionar_leds(painel_luminoso);
};

//implementar RTOS depois

PIO pio = pio0; 
uint sm = 0;    
// uint sinal_amarelo=0;

// void vMatrizLedsTask(){

//     //Configurações para matriz de leds
//     uint offset = pio_add_program(pio, &pio_matrix_program);
//     pio_matrix_program_init(pio, sm, offset, MatrizLeds, 800000, IS_RGBW);

//     uint intensidade = 0;

//     while(true){

//         //ler de uma fila o dados de intensidade

//         branco = {0.5*intensidade, 0.5*intensidade,0.5*intensidade};
//         //matriz que será modificada no loop
//         Matriz_leds painel_luminoso = {{branco,branco,branco,branco,branco},{branco,branco,branco,branco,branco},{branco,branco,branco,branco,branco},{branco,branco,branco,branco,branco},{branco,branco,branco,branco,branco}};
//         acionar_leds(matriz);

//         //pausa na task
//     };
// };


int main()
{
    stdio_init_all();

    // Para ser utilizado o modo BOOTSEL com botão B
    gpio_init(botaoB);
    gpio_set_dir(botaoB, GPIO_IN);
    gpio_pull_up(botaoB);
    gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
   // Fim do trecho para modo BOOTSEL com botão B


    // Inicializa o I2C0
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
   // Inicializa o sensor de luz BH1750
    bh1750_power_on(I2C_PORT);

    char str_lux[10];  // Buffer para armazenar a string
    

    //Configurações para matriz de leds
    uint offset = pio_add_program(pio, &pio_matrix_program);
    pio_matrix_program_init(pio, sm, offset, MatrizLeds, 800000, IS_RGBW);

    while (true) {
        matriz(100);
        sleep_ms(1000);
        // Leitura do sensor de Luz BH1750
        uint16_t lux = bh1750_read_measurement(I2C_PORT);
        printf("Lux = %d\n", lux);
        sleep_ms(1000);
    }
}
