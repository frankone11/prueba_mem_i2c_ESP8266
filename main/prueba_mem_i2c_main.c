/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#include "at24.h"

#define PIN_P	GPIO_NUM_2

const uint8_t device = (uint8_t) (0x50U << 1); //device address 0x50 (7bit) or 0xA0 (8 bit)


void app_main()
{
	uint8_t i, j;
	uint8_t datos[64] = {
		0x55,0xAA, 0x00, 0xFF, 0x0F, 0xF0, 127, 0,
		0x55,0xAA, 0x00, 0xFF, 0x0F, 0xF0, 127, 1,
		0x55,0xAA, 0x00, 0xFF, 0x0F, 0xF0, 127, 2,
		0x55,0xAA, 0x00, 0xFF, 0x0F, 0xF0, 127, 3,
		0x55,0xAA, 0x00, 0xFF, 0x0F, 0xF0, 127, 4,
		0x55,0xAA, 0x00, 0xFF, 0x0F, 0xF0, 127, 5,
		0x55,0xAA, 0x00, 0xFF, 0x0F, 0xF0, 127, 6,
		0x55,0xAA, 0x00, 0xFF, 0x0F, 0xF0, 127, 7		
		};
	uint8_t buff[8] = {0};

	gpio_set_direction(PIN_P, GPIO_MODE_OUTPUT);
	printf("*** MEM I2C con ESP8266 ***\n\n");

	printf("\n*** Programa de muestra en memoria ***\n\n");

	AT24_Init();

	printf("Se van a guardar 64 bits en la memoria.\n");

	AT24_WriteBytes(device, 0, datos, 32);
	vTaskDelay(100/portTICK_RATE_MS);
	AT24_WriteBytes(device, 32, datos+32, 32);
	vTaskDelay(100/portTICK_RATE_MS);

	/*for(i= 0; i < 8; i++)
	{
		AT24_WriteByte(device, i, i);
		vTaskDelay(100/portTICK_RATE_MS);
	}*/

	while(1)
	{
		printf("\nSe van a leer los datos en la memoria.\n");
		
		for(i=0; i < 8; i++)
		{
			memset(buff, 0, sizeof(buff));
			AT24_ReadBytes(device, i*8, buff, sizeof(buff));
			printf("Dato %d: ", i);
			for(j = 0; j < 8; j++)
				printf("0x%.2X (%d) ", buff[j], buff[j]);
			printf("\n");
		}
		
		gpio_set_level(PIN_P, gpio_get_level(PIN_P)?0:1);
		vTaskDelay(1000/portTICK_RATE_MS);
	}
}
