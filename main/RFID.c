
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"

#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5

esp_err_t ret;
spi_device_handle_t spi;


static void SPI_Config(void)
{
    spi_bus_config_t buscfg={
        .miso_io_num=PIN_NUM_MISO,
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=256
    };
    spi_device_interface_config_t devcfg={
        .clock_speed_hz=100*1000,           
        .mode=2,                                //SPI mode 0
        .spics_io_num=PIN_NUM_CS,               //CS pin
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        .pre_cb=NULL,  //Specify pre-transfer callback to handle D/C line
    };
    //Initialize the SPI bus
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);
    //Attach the LCD to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);




}



 void SPI_SendData( uint8_t *data, int len)
{
    spi_transaction_t t;

    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=len*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=data;               //Data

    esp_err_t ret=spi_device_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.

}



static void SPI_ReciveData(uint8_t *data, int len)
{
	spi_transaction_t t;
	
	memset(&t, 0, sizeof(t));
	t.length=8*len;
	t.tx_buffer=data;
	
	esp_err_t ret = spi_device_transmit(spi, &t);
	assert( ret == ESP_OK );
}









void RFID_Init(void)
{
	SPI_Config();



}








