#include <stdio.h>
#include "driver/i2c.h"
#include "LED_Ctr.h"


#define I2C_EXAMPLE_MASTER_SCL_IO          26               /*!< gpio number for I2C master clock */
#define I2C_EXAMPLE_MASTER_SDA_IO          25               /*!< gpio number for I2C master data  */
#define I2C_EXAMPLE_MASTER_NUM             I2C_NUM_1        /*!< I2C port number for master dev */
#define I2C_EXAMPLE_MASTER_TX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_RX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_EXAMPLE_MASTER_FREQ_HZ         100000           /*!< I2C master clock frequency */

#define SLAVE_DEV_ADDR      	           0x5B             /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL                           0x1              /*!< I2C nack value */





#define RGB_LED_MaxNUM 5

#define AW9623B_ID 0x23
#define AW9623B_ID_REG 0x10

#define GLOBAL_CONIFG_REG 0x11
#define P0_LED_Mode_REG 0X12
#define P1_LED_Mode_REG 0X13

#define P0_0_DIM_REG 0X24
#define P0_1_DIM_REG 0X25
#define P0_2_DIM_REG 0X26
#define P0_3_DIM_REG 0X27
#define P0_4_DIM_REG 0X28
#define P0_5_DIM_REG 0X29
#define P0_6_DIM_REG 0X2A
#define P0_7_DIM_REG 0X2B

#define P1_0_DIM_REG 0X20
#define P1_1_DIM_REG 0X21
#define P1_2_DIM_REG 0X22
#define P1_3_DIM_REG 0X23
#define P1_4_DIM_REG 0X2C
#define P1_5_DIM_REG 0X2D
#define P1_6_DIM_REG 0X2E
#define P1_7_DIM_REG 0X2F



static const uint8_t Led_RGB_Tab[RGB_LED_MaxNUM][3]={
												{P1_0_DIM_REG,P1_1_DIM_REG,P1_2_DIM_REG},
												{P1_3_DIM_REG,P0_0_DIM_REG,P0_1_DIM_REG},
												{P0_2_DIM_REG,P0_3_DIM_REG,P0_4_DIM_REG},
												{P0_5_DIM_REG,P0_6_DIM_REG,P0_7_DIM_REG},
												{P1_4_DIM_REG,P1_5_DIM_REG,P1_6_DIM_REG}
										    };

/**
 * @brief i2c master initialization
 */
static void i2c_demo_init()
{
    int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_EXAMPLE_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_EXAMPLE_MASTER_RX_BUF_DISABLE,
                       I2C_EXAMPLE_MASTER_TX_BUF_DISABLE, 0);
}






static esp_err_t i2c_demo_write(i2c_port_t i2c_num,uint8_t RegAddr, uint8_t* data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ( SLAVE_DEV_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
	i2c_master_write_byte(cmd, RegAddr, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}



static esp_err_t i2c_demo_read(i2c_port_t i2c_num,uint8_t RegAddr, uint8_t* data_rd, size_t size)
{
    if (size == 0) {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
	i2c_master_write_byte(cmd, ( SLAVE_DEV_ADDR << 1 ) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, RegAddr, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, ( SLAVE_DEV_ADDR << 1 ) | READ_BIT, ACK_CHECK_EN);
    if (size > 1) {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

 static esp_err_t AW9623B_i2c_write(uint8_t RegAddr,uint8_t Data)
 {
	 return i2c_demo_write(I2C_NUM_1,RegAddr,&Data,1);
	
 }

  static esp_err_t AW9623B_i2c_read(uint8_t RegAddr,uint8_t* pData)
 {

	 return i2c_demo_read(I2C_NUM_1,RegAddr,pData,1);
 }





  

 void LED_Ctr_Init(void)
 {
 	uint8_t TempID=0;
	
	i2c_demo_init();

	AW9623B_i2c_read(AW9623B_ID_REG,&TempID);
	
	if(AW9623B_ID==TempID)
	{
		AW9623B_i2c_write(P0_LED_Mode_REG,0X00);
		AW9623B_i2c_write(P1_LED_Mode_REG,0X00);


	}
		
 }
 	

void LED_Brightness_Set(uint8_t Led_Index,uint8_t Brightness)
{

		Led_Index=Led_RGB_Tab[Led_Index/3][Led_Index%3];
			
		AW9623B_i2c_write(Led_Index,Brightness);


}



void LED_Test_Dispaly(void)
{
    uint8_t TempIndex;


	for(TempIndex=0;TempIndex<RGB_LED_MaxNUM*3;TempIndex++)
		{
			LED_Brightness_Set(TempIndex,LED_ON);
		    vTaskDelay(100/ portTICK_PERIOD_MS);		
		}


}

