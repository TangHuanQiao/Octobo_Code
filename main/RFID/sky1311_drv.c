#include "sky1311_drv.h"
#include "sky1311t_reg.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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
 
 
 void SPI_Config(void)
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
	 	 .address_bits=8,
		 .clock_speed_hz=5000*1000,			 
		 .mode=0,								 //SPI mode 0
		 .spics_io_num=PIN_NUM_CS,				 //CS pin
		 .queue_size=7, 						 //We want to be able to queue 7 transactions at a time
		 .pre_cb=NULL,	//Specify pre-transfer callback to handle D/C line
	 };
	 //Initialize the SPI bus
	 ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
	 ESP_ERROR_CHECK(ret);
	 //Attach the LCD to the SPI bus
	 ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
	 ESP_ERROR_CHECK(ret);
 
 
 
 
 }
 
 

 

 
  void DelayMS(uint32_t x)
 {
	 vTaskDelay(x/ portTICK_PERIOD_MS);
 }

 void delay(volatile uint16_t i)
 {
	 while(i--)
	 	{

	 	}

 }

void uart_puts(char *str)
{
    while(*str) printf("%c",*str++);
  
}

 void uart_newrow()
 {
	 printf("\r\n");
 }

 void uart_printBuffer(uint8_t* buff, uint16_t size)
 {
	 while(size--)
	 {
		 printf("%X",*buff++);
	 }
 }

 void uart_putHex(uint8_t data)
 {
		printf("%X",data);
 }

 void uart_putChar(uint8_t ch)
 {
	    printf("%c",ch);
 }	

 /******************************************************************************
 ** \简  述  通过SPI接口向1311写一个字节的命令
 **
 ** \参  数  命令字
 ** \返回值  none
 ******************************************************************************/
void sky1311WriteCmd(uint8_t cmd)
{

    cmd = (cmd & 0x1F) | 0x80;	        // bit7,6,5 = 100b, mean command

	spi_transaction_t t;	
	memset(&t, 0, sizeof(t));
	t.addr=cmd;	
	t.length=0;    

	esp_err_t ret = spi_device_transmit(spi, &t);
	assert( ret == ESP_OK );


}
/**
 ******************************************************************************
 ** \简  述  通过SPI接口向1311的寄存器写一个字节数据
 **
 ** \参  数  uint8_t regAdd: 寄存器地址， uint8_t data: 要写入的数据
 ** \返回值  none
 ******************************************************************************/
void sky1311WriteReg(uint8_t regAdd, uint8_t data)
{

    regAdd      =   (regAdd & 0x3F);        // bit7,6=00, config as addr/write mode

	spi_transaction_t t;	
	memset(&t, 0, sizeof(t));
	t.addr=regAdd;	
	t.length=8;    
	t.tx_buffer=&data;
	
	esp_err_t ret = spi_device_transmit(spi, &t);
	assert( ret == ESP_OK );

}
/**
 ******************************************************************************
 ** \简  述  通过SPI接口读取1311的寄存器
 **
 ** \参  数  uint8_t regAdd: 寄存器地址
 ** \返回值  uint8_t 寄存器内容
 ******************************************************************************/
uint8_t sky1311ReadReg(uint8_t regAdd)
{
    uint8_t value=0;

    regAdd      =   (regAdd & 0x3F) | 0x40;            // bit7,6=01, config as addr/read mode

	 spi_transaction_t t;	 	 
	 memset(&t, 0, sizeof(t));
	 t.addr=regAdd;
	 t.length=8;	
	 t.rxlength=8;	 
	 t.rx_buffer=&value;
	 
	 uint8_t txLow=0;
	 t.tx_buffer=&txLow;

	 esp_err_t ret = spi_device_transmit(spi, &t);
	 assert( ret == ESP_OK );
	 
    return value;
}
/**
 ******************************************************************************
 ** \简  述  通过SPI接口向1311的FIFO写指定数目的数据
 **
 ** \参  数  uint8_t* 数据内容头地址， uint8_t count: 要写入的数据数量
 ** \返回值  none
 ******************************************************************************/
void sky1311WriteFifo(uint8_t *data, uint8_t count)
{
    uint8_t add;

	if(count==0)return;


    add      =   (ADDR_FIFO & 0x3F);               // bit7,6=00, config as addr/write mode

	spi_transaction_t t;	
	memset(&t, 0, sizeof(t));
	t.addr=add;	
	t.length=8*count;    
	t.tx_buffer=data;
	
	esp_err_t ret = spi_device_transmit(spi, &t);
	assert( ret == ESP_OK );



}
/**
 ******************************************************************************
 ** \简  述  通过SPI接口向1311的FIFO读取指定数目的内容
 **
 ** \参  数  uint8_t* data 保存读取内容的缓冲区首地址， uint8_t count 读取的字节数
 ** \返回值  none
 ******************************************************************************/
void sky1311ReadFifo(uint8_t *data, uint8_t count)
{
    uint8_t add;

	if(count==0)return;

    add   =   (ADDR_FIFO & 0x3F) | 0x40;            // bit7,6=01, config as addr/read mode

	spi_transaction_t t;	
	memset(&t, 0, sizeof(t));
	t.addr=add;	
	t.length=8*count;
	t.rxlength=8*count;
	t.rx_buffer=data;
	
	esp_err_t ret = spi_device_transmit(spi, &t);
	assert( ret == ESP_OK );



}

