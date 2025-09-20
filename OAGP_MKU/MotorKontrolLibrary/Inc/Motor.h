/*
 * Motor.h
 *
 *  Created on: Sep 20, 2025
 *      Author: T_rab
 */
#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f1xx_hal.h"  // MCU modelinize göre güncelleyin

// Motor yönleri için enum
enum MotorDirection {
    ILERI = 0,
    GERI  = 1
};

class Motor {
private:
    // Timer & PWM kanalları
    TIM_HandleTypeDef *htim;           // PWM timer
    TIM_HandleTypeDef *htim_timebase;  // Hall zamanı ölçmek için timer (örn: TIM2)
    uint32_t tim_channel_a;
    uint32_t tim_channel_b;
    uint32_t tim_channel_c;

    // Low side MOSFET pinleri
    GPIO_TypeDef *AL_GPIO_Port;
    uint16_t AL_Pin;
    GPIO_TypeDef *BL_GPIO_Port;
    uint16_t BL_Pin;
    GPIO_TypeDef *CL_GPIO_Port;
    uint16_t CL_Pin;

    // Hall sensör pinleri
    GPIO_TypeDef *HallA_Port;
    uint16_t HallA_Pin;
    GPIO_TypeDef *HallB_Port;
    uint16_t HallB_Pin;
    GPIO_TypeDef *HallC_Port;
    uint16_t HallC_Pin;

    // Hall sensöründen alınan state
    uint8_t HallState;

    // Motor yönü
    MotorDirection direction;

    // Hall state sayaçları (isteğe bağlı)
    uint32_t mHallStateDefaultSayac {};
    uint32_t mHallState1Sayac {};
    uint32_t mHallState2Sayac {};
    uint32_t mHallState3Sayac {};
    uint32_t mHallState4Sayac {};
    uint32_t mHallState5Sayac {};
    uint32_t mHallState6Sayac {};

    uint8_t mHallStateArray[4096];
    uint8_t mHallStateArrayIleri[4096];
    uint8_t mHallStateIleriDefaultSayac;
    uint32_t mSayac = 0;

    uint32_t sayacArttir()
    {
        if (mSayac < 4096) mSayac++;
        return mSayac;
    }

public:
    // Constructor
    Motor(TIM_HandleTypeDef *htim_, TIM_HandleTypeDef *htim_timebase_,
          uint32_t ch_a, uint32_t ch_b, uint32_t ch_c,
          GPIO_TypeDef *AL_Port, uint16_t AL_Pin_,
          GPIO_TypeDef *BL_Port, uint16_t BL_Pin_,
          GPIO_TypeDef *CL_Port, uint16_t CL_Pin_,
          GPIO_TypeDef *HallA_Port_, uint16_t HallA_Pin_,
          GPIO_TypeDef *HallB_Port_, uint16_t HallB_Pin_,
          GPIO_TypeDef *HallC_Port_, uint16_t HallC_Pin_);

    // Motoru başlat
    void init();

    // Hall sensörlerini oku ve deltaTime hesapla
    void updateHall();

    // Komutasyon işlemi
    void komutasyon();

    // Motor yönünü ayarla
    void setDirection(MotorDirection dir);

    // Motor yönünü al
    MotorDirection getDirection() const { return direction; }

    // Motor durumları
    bool aktif;         // Motor aktif mi
    uint16_t dutyCycle; // PWM duty cycle
    uint16_t hallCounter = 0;
};

#endif // MOTOR_H
