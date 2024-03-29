#ifndef __AT24_DEF__
#define __AT24_DEF__

void AT24_Init();
esp_err_t AT24_WriteByte(uint8_t device, uint16_t address, uint8_t data);
esp_err_t AT24_WriteBytes(uint8_t device, uint16_t address, uint8_t *data, uint8_t len);
esp_err_t AT24_ReadBytes(uint8_t device, uint16_t address, uint8_t *data, uint8_t len);

#endif //__AT24_DEF__