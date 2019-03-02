#ifndef __HARDWARE_H
#define __HARDWARE_H

/* 	In hardware.h & hardware.c define:
		Components of the robot
		Geometry constants of the robot
		Pin & Timer connection of circuit
		Basic operation of components         */
		
#include "gpio.h"
#include "tim.h"
#include "crc.h"
#include "spi.h"

//---------------------------------------------------------------------------
struct SPI
{
	SPI_HandleTypeDef *Spi;
	GPIO_TypeDef 			*Nss_port;
	uint16_t 					Nss_pin;
	int32_t	 					Tx[7];
	int32_t	 					Rx[7];
	uint16_t					length;
  volatile int			Error;
	volatile int			TransmitCount;
	volatile double		SuccessRate;
};
void Spi_Init(struct SPI *spi);
void Spi_Write(struct SPI *spi, uint8_t n, uint32_t value);
void Spi_TransmitReceive(struct SPI *spi);
void Spi_Slave_TransmitReceive(struct SPI *spi);
void Spi_Slave_CRC(struct SPI *spi);

//---------------------------------------------------------------------------
struct HARDWARE
{
	struct SPI spiTomaster;
};
void Hardware_Init(struct HARDWARE *hardware);
void spi_Mapping(struct SPI *spi);
int Spi_Crc_Error(struct SPI *spi);




#endif
