#include "hardware.h"

void Hardware_Init(struct HARDWARE *hardware)
{
	//Hardware_Mapping(hardware);
}

void spi_Mapping(struct SPI *spi)
{
	spi->Spi = 				&hspi5;
	spi->Nss_port = 		GPIOF;
	spi->Nss_pin = 		GPIO_PIN_10;
	spi->length = 			6;
}


void Spi_Write(struct SPI *spi, uint8_t n, uint32_t value)
{
	spi->Tx[n] = value;
}

void Spi_Slave_CRC(struct SPI *spi)
{
	spi->Tx[(spi->length)] = (int32_t) HAL_CRC_Calculate(&hcrc, (uint32_t *) &spi->Tx[0], 1*(spi->length));
}

void Spi_Slave_TransmitReceive(struct SPI *spi)
{
	HAL_SPI_TransmitReceive(spi->Spi, (uint8_t*)spi->Tx, (uint8_t*)spi->Rx, 4*((spi->length)+1), 2);
}

int Spi_Crc_Error(struct SPI *spi)
{
	uint32_t crc = HAL_CRC_Calculate(&hcrc,(uint32_t *) &spi->Rx[0], 1*(spi->length));
	int32_t rx = spi->Rx[(spi->length)];;
	if(crc == rx){
		spi->TransmitCount ++;
		return 0;
	} else {
		spi->TransmitCount ++;
		spi->Error ++;
		if(spi->TransmitCount > 1000)
		{
			spi->SuccessRate = 100*(spi->TransmitCount - spi->Error) / (double)(spi->TransmitCount);
			spi->Error = 0;
			spi->TransmitCount = 0;
		}
		return 1;
	}
}
