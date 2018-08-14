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
 ** \��  ��  ͨ��SPI�ӿ���1311дһ���ֽڵ�����
 **
 ** \��  ��  ������
 ** \����ֵ  none
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
 ** \��  ��  ͨ��SPI�ӿ���1311�ļĴ���дһ���ֽ�����
 **
 ** \��  ��  uint8_t regAdd: �Ĵ�����ַ�� uint8_t data: Ҫд�������
 ** \����ֵ  none
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
 ** \��  ��  ͨ��SPI�ӿڶ�ȡ1311�ļĴ���
 **
 ** \��  ��  uint8_t regAdd: �Ĵ�����ַ
 ** \����ֵ  uint8_t �Ĵ�������
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
 ** \��  ��  ͨ��SPI�ӿ���1311��FIFOдָ����Ŀ������
 **
 ** \��  ��  uint8_t* ��������ͷ��ַ�� uint8_t count: Ҫд�����������
 ** \����ֵ  none
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
 ** \��  ��  ͨ��SPI�ӿ���1311��FIFO��ȡָ����Ŀ������
 **
 ** \��  ��  uint8_t* data �����ȡ���ݵĻ������׵�ַ�� uint8_t count ��ȡ���ֽ���
 ** \����ֵ  none
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

