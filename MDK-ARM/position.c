#include "position.h"
#include "math.h"

void Set_Command_Point(struct POSITION *pos, double x, double y, double theta)
{
	pos->CommandPoint.X = x;
	pos->CommandPoint.Y = y;
	pos->CommandPoint.Theta = theta;
}

void Update_Encoder_Readings(struct POSITION *pos, struct ENCODER *encoderL, struct ENCODER *encoderR)
{
	pos->EncoderReadingsL[1] = pos->EncoderReadingsL[0]; 
	pos->EncoderReadingsR[1] = pos->EncoderReadingsR[0]; 
	pos->EncoderReadingsL[0] = Encoder_Read(encoderL);
	pos->EncoderReadingsR[0] = Encoder_Read(encoderR);
}

void Update_Speed_Omega(struct POSITION *pos, struct ENCODER *encoderL, struct ENCODER *encoderR)
{
	int delta = (int)(pos->EncoderReadingsL[0] - pos->EncoderReadingsL[1]);
	pos->SpeedL = delta*2*PI*(encoderL->Radius)*(pos->UpdateFrequency)/(encoderL->Resolution)/(encoderL->ReductionRatio);
	delta = (int)(pos->EncoderReadingsR[0] - pos->EncoderReadingsR[1]);
	pos->SpeedR = delta*2*PI*(encoderR->Radius)*(pos->UpdateFrequency)/(encoderR->Resolution)/(encoderR->ReductionRatio);
	
	pos->Speed = ((pos->SpeedR) + (pos->SpeedL)) / 2;
	pos->Omega = ((pos->SpeedR) - (pos->SpeedL)) / (pos->EncoderDistance) *180/PI;
}

void Update_Position(struct POSITION *pos)
{
	pos->Points[1].X = pos->Points[0].X;
	pos->Points[1].Y = pos->Points[0].Y;
	pos->Points[1].Theta = pos->Points[0].Theta;
	
	pos->Points[0].Theta += pos->Omega / (pos->UpdateFrequency);
	if(pos->Points[0].Theta >= 360)		pos->Points[0].Theta -= 360;
	else if(pos->Points[0].Theta < 0)	pos->Points[0].Theta += 360;
	
	volatile double vx, vy;
	vx = (pos->Speed) * cos(0.5*(pos->Points[0].Theta + pos->Points[1].Theta) *PI/180.0);
	vy = (pos->Speed) * sin(0.5*(pos->Points[0].Theta + pos->Points[1].Theta) *PI/180.0);
	pos->Points[0].X += vx / (pos->UpdateFrequency);
	pos->Points[0].Y += vy / (pos->UpdateFrequency);
}

void Set_Position_Transmit(struct POSITION *pos, struct SPI *spi)
{
	spi->Tx[0] = pos->Points[0].X;
	spi->Tx[1] = pos->Points[0].Y;
	spi->Tx[2] = pos->Points[0].Theta;
	Spi_Slave_CRC(spi);
}


void position_handler(struct POSITION *pos, struct ENCODER *encoderL, struct ENCODER *encoderR, struct SPI *spi)
{
	Update_Encoder_Readings(pos, encoderL, encoderR);
	Update_Speed_Omega(pos, encoderL, encoderR);
	Update_Position(pos);
	Set_Position_Transmit(pos, spi);
}


void Position_Init(struct POSITION *pos)
{
	pos->CommandPoint.X = 0;
	pos->CommandPoint.Y = 0;
	pos->CommandPoint.Theta = 0;
	pos->Points[0].X = 0;
	pos->Points[0].Y = 0;
	pos->Points[0].Theta = 0;
	pos->SpeedL = 0;
	pos->SpeedR = 0;
	pos->Speed = 0;
	pos->Omega = 0;
	pos->UpdateFrequency = 1000;
	pos->EncoderDistance = 213.2;
}



