/*
 * Mpu.h
 *
 *  Created on: Sep 27, 2025
 *      Author: T_rab
 */

#ifndef INC_MPU_H_
#define INC_MPU_H_


#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "Madgwick.h"
#include <cmath>
class IMU
{
public:
    IMU(I2C_HandleTypeDef *hi2c);
    MadgwickAHRS madgwick;

    void Yapilandir();
    void accOku();
    void gyroOku();
    void sicaklikOku();
    void kalibreEt();
    void gercekDataOku();
    void aciBul();
    void EnlemBoylamGuncelle(float heading , float latitude,float longitude);
    void AccToKonum(float accX, float accY, float accZ);
    float* PitchAl();
    float* RollAl();
    float* YawAl();
    float* SicaklikAl();
    float konumxX(){ return konumX;}
    float konumyY(){return konumY; }
    float hizxX(){ return hizX;}
    float hizyY(){return hizY; }
    float x(){return ivmeX;}
    float y(){return ivmeY;}
    float xx(){return accEksen[0];}
    float yy(){return accEksen[1];}
    float *LatAl();
    float *LongAl();
    float hizX;
    float hizY;
    float konumX;
    float konumY;
    float ivmeX ;
    float ivmeY;
    float ivmeZ ;
private:

    I2C_HandleTypeDef *hi2c;
//    Kalman kalmanax;
//    Kalman kalmanay;
//    Kalman kalmangx;
//    Kalman kalmangy;
//    AlcakGecirenFiltre lpfax;
//    AlcakGecirenFiltre lpfay;
    uint8_t data_u8;
    int16_t gyroEksen[3], accEksen[3];
    float gyroHesap[3];
    float accHesap[3] = {0.0f, 0.0f, 0.0f};
    int16_t accToplamVektor_s16;
    float accPitchAci_f, accRollAci_f;
    float gyroRollAci_f, gyroPitchAci_f, gyroYawAci_f;
    float rollAci_f, pitchAcisi_f;
    uint16_t hamSicaklik_u16;
    float Sicaklik_f;


    float enlem_f;
    float boylam_f;
};





#endif /* INC_MPU_H_ */


