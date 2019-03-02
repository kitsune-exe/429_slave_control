#include "robot.h"

extern struct SPI spiTomaster;;
volatile int cnt = 0;

struct SPI spi2;
int32_t TX[7];
int32_t RX[7];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim3)
	{
    Spi_Slave_CRC(&spiTomaster);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_10)
	{
    //Spi_Slave_CRC(spi2);
		Spi_Slave_TransmitReceive(&spiTomaster);
    //Spi_Slave_CRC(&spiTomaster);
		if(Spi_Crc_Error(&spiTomaster)){
			MX_SPI5_Init();
			cnt ++;
		}else{
			//Update_Position_Spi(&eurobot2019.controlsystem.position, &eurobot2019.hardware.spiToslave);
		}
    
    //HAL_SPI_Receive(&hspi2, (uint8_t*)RX, 28, 2);
	}
}
