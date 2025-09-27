/*
 * Gps.h
 *
 *  Created on: Jun 29, 2024
 *      Author: onurd
 */

#ifndef MYOTONOMDRIVERS_INC_GPS_H_
#define MYOTONOMDRIVERS_INC_GPS_H_

#include "main.h"
#include "lwgps/lwgps.h"

class GPS {

public:
    GPS(UART_HandleTypeDef* huart);
    void Yapilandir();
    void UartRxCpltCallback();
    float* LatitudeAl();
    float* LongitudeAl();
    void GpsDataCek();

private:
    UART_HandleTypeDef* huart_;
    lwgps_t gps_;
    uint8_t rxBuffer_u8[128];
    uint8_t rxIndex_u8;
    uint8_t rxData_u8;
    float latitude_f32;
    float longitude_f32;
};

#endif /* MYOTONOMDRIVERS_INC_GPS_H_ */
