
#include <stdint.h>
#include "driver/i2c.h"
#include "freertos/task.h"

#include "at24.h"

#define I2C_OLED_MASTER_SCL_IO           GPIO_NUM_5       /*!< gpio number for I2C master clock */
#define I2C_OLED_MASTER_SDA_IO           GPIO_NUM_4       /*!< gpio number for I2C master data  */
#define I2C_OLED_MASTER_NUM              I2C_NUM_0        /*!< I2C port number for master dev */
#define I2C_OLED_MASTER_TX_BUF_DISABLE   0                /*!< I2C master do not need buffer */
#define I2C_OLED_MASTER_RX_BUF_DISABLE   0                /*!< I2C master do not need buffer */
#define ACK_CHECK_EN 1


void AT24_Init()
{
	i2c_config_t conf;
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = I2C_OLED_MASTER_SDA_IO;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_io_num = I2C_OLED_MASTER_SCL_IO;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.clk_stretch_tick = 300; // 300 ticks, Clock stretch is about 210us, you can make changes according to the actual situation.
	ESP_ERROR_CHECK(i2c_driver_install(I2C_OLED_MASTER_NUM, conf.mode));
	ESP_ERROR_CHECK(i2c_param_config(I2C_OLED_MASTER_NUM, &conf));
}

esp_err_t AT24_WriteBytes(uint8_t device, uint16_t address, uint8_t *data, uint8_t len)
{
	esp_err_t ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, device & 0xFEU, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, (uint8_t)((address & 0x0F00U) >> 8), ACK_CHECK_EN); //12 bytes address
	i2c_master_write_byte(cmd, (uint8_t)(address & 0x00FFU), ACK_CHECK_EN);
	i2c_master_write(cmd, data, len, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(I2C_OLED_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	vTaskDelay(10/portTICK_RATE_MS);
	return ret;
}

esp_err_t AT24_WriteByte(uint8_t device, uint16_t address, uint8_t data)
{
	esp_err_t ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, device & 0xFEU, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, (uint8_t)((address & 0x0F00U) >> 8), ACK_CHECK_EN); //12 bytes address
	i2c_master_write_byte(cmd, (uint8_t)(address & 0x00FFU), ACK_CHECK_EN);
	i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(I2C_OLED_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	vTaskDelay(10/portTICK_RATE_MS);
	return ret;
}

esp_err_t AT24_ReadBytes(uint8_t device, uint16_t address, uint8_t *data, uint8_t len)
{
	esp_err_t ret;
	i2c_cmd_handle_t cmd;

	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, device & 0xFEU, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, (uint8_t)((address & 0xFF00U) >> 8), ACK_CHECK_EN); //16 bytes address
	i2c_master_write_byte(cmd, (uint8_t)(address & 0x00FFU), ACK_CHECK_EN);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(I2C_OLED_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	if(ret != ESP_OK)
		return ret;

	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, device | 0x01U, ACK_CHECK_EN);  //Lectura en 0x01U
	i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
	i2c_master_stop(cmd);
	ret = i2c_master_cmd_begin(I2C_OLED_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	return ret;
}