
#include <stdint.h>
#include "driver/i2c.h"


void i2c_demo_init(void);

esp_err_t i2c_demo_read(uint8_t slave_addr ,i2c_port_t i2c_num, uint8_t* data_rd, size_t size);


esp_err_t i2c_demo_write(uint8_t slave_addr ,i2c_port_t i2c_num, uint8_t* data_wr, size_t size);
















