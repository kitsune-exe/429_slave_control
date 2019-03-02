#ifndef __POSITION_H
#define __POSITION_H

#include "hardware.h"

#define PI 3.14159265359

struct POINT
{
	volatile double X;
	volatile double Y;
	volatile double Theta;
};

struct POSITION
{
	struct 								POINT CommandPoint;
	struct 								POINT Points[2];
	volatile unsigned int EncoderReadingsL[2];
	volatile unsigned int EncoderReadingsR[2];
	volatile double 			SpeedL;
	volatile double 			SpeedR;
	volatile double 			Speed;											// 0.1mm/s	
	volatile double 			Omega;											// degree/s
	int 									UpdateFrequency;
	double								EncoderDistance;    				//distance from an encoder to another
};

void Set_Command_Point(struct POSITION *pos, double x, double y, double theta);

void Update_Encoder_Readings(struct POSITION *pos, struct ENCODER *encoderL, struct ENCODER *encoderR);

void Update_Speed_Omega(struct POSITION *pos, struct ENCODER *encoderL, struct ENCODER *encoderR);

void Update_Position(struct POSITION *pos);

void Set_Position_Transmit(struct POSITION *pos, struct SPI *spi);

void position_handler(struct POSITION *pos, struct ENCODER *encoderL, struct ENCODER *encoderR, struct SPI *spi);

void Position_Init(struct POSITION *pos);







#endif
