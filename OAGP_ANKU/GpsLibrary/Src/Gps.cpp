/*
 * Gps.cpp
 *
 *  Created on: Jun 29, 2024
 *      Author: onurd
 */
#include "Gps.h"

GPS::GPS(UART_HandleTypeDef* huart)
{
	this->huart_=huart;
	this->rxIndex_u8=0;
	this->rxData_u8=0;
	this->latitude_f32=0;
	this->longitude_f32=0;
}

void GPS::Yapilandir()
{
    lwgps_init(&gps_);
    HAL_UART_Receive_IT(huart_, &rxData_u8, 1);
}

void GPS::UartRxCpltCallback()
{
    if (rxData_u8 != '\n' && rxIndex_u8 < sizeof(rxBuffer_u8))
    {
        rxBuffer_u8[rxIndex_u8++] = rxData_u8;
    }
    else
    {
        lwgps_process(&gps_, rxBuffer_u8, rxIndex_u8 + 1);
        GpsDataCek();
        rxIndex_u8 = 0;
        rxData_u8 = 0;
    }
    HAL_UART_Receive_IT(huart_, &rxData_u8, 1);
}

void GPS::GpsDataCek()
{
	latitude_f32 = gps_.latitude;
	longitude_f32 = gps_.longitude;
}

float* GPS::LatitudeAl(){return &latitude_f32;}

float* GPS::LongitudeAl(){return &longitude_f32;}

