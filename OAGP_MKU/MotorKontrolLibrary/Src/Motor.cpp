/*
 * Motor.cpp
 *
 *  Created on: Sep 20, 2025
 *      Author: T_rab
 */


#include "Motor.h"

Motor::Motor(TIM_HandleTypeDef *htim_,TIM_HandleTypeDef *htim_timebase_,
             uint32_t ch_a, uint32_t ch_b, uint32_t ch_c,
             GPIO_TypeDef *AL_Port, uint16_t AL_Pin_,
             GPIO_TypeDef *BL_Port, uint16_t BL_Pin_,
             GPIO_TypeDef *CL_Port, uint16_t CL_Pin_,
             GPIO_TypeDef *HallA_Port_, uint16_t HallA_Pin_,
             GPIO_TypeDef *HallB_Port_, uint16_t HallB_Pin_,
             GPIO_TypeDef *HallC_Port_, uint16_t HallC_Pin_)
{
    htim = htim_;
    htim_timebase = htim_timebase_;
    tim_channel_a = ch_a;
    tim_channel_b = ch_b;
    tim_channel_c = ch_c;

    AL_GPIO_Port = AL_Port; AL_Pin = AL_Pin_;
    BL_GPIO_Port = BL_Port; BL_Pin = BL_Pin_;
    CL_GPIO_Port = CL_Port; CL_Pin = CL_Pin_;

    HallA_Port = HallA_Port_; HallA_Pin = HallA_Pin_;
    HallB_Port = HallB_Port_; HallB_Pin = HallB_Pin_;
    HallC_Port = HallC_Port_; HallC_Pin = HallC_Pin_;

    dutyCycle = 200;
    aktif = false;
    HallState = 0;
}

void Motor::init()
{
    __HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
    __HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
    __HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);

    HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
}

void Motor::updateHall()
{
    uint8_t ha = HAL_GPIO_ReadPin(HallA_Port, HallA_Pin);
    uint8_t hb = HAL_GPIO_ReadPin(HallB_Port, HallB_Pin);
    uint8_t hc = HAL_GPIO_ReadPin(HallC_Port, HallC_Pin);
    HallState = (ha << 2) | (hb << 1) | hc;
}

void Motor::setDirection(MotorDirection dir) { direction = dir; }

void Motor::komutasyon()
{
    if (!aktif)
    {
        __HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
        __HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
        __HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
        HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
        return;
    }

    switch(direction)
    {
    	case ILERI:
    		switch (HallState)
			{
    			case 5: // 4. state için
					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, dutyCycle);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
					//mHallStateArrayIleri[i++] = 5;
					break;

    			case 4: // 6. state için
					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, dutyCycle);

					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
					//mHallStateArrayIleri[i++] = 4;

					break;

    			case 6: // 2. state için
					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, dutyCycle);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
					//mHallStateArrayIleri[i++] = 6;
					break;
    			case 2: // 3. state için
					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, dutyCycle);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
					//mHallStateArrayIleri[i++] = 2;
					break;

    			case 3: // 1. state için
					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, dutyCycle);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_RESET);
					//mHallStateArrayIleri[i++] = 3;
					break;

				case 1: // 5. state için
					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, dutyCycle);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_RESET);
					//mHallStateArrayIleri[i++] = 1;
					break;

				default:
					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
					//mHallStateIleriDefaultSayac++;
					break;
			}
    		break;

    	case GERI:
    		switch (HallState)
			{
				case 1: // 3. state için

//					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, dutyCycle);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
//					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_RESET);
//					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
//					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);

					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, dutyCycle);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
					mHallState1Sayac++;
					if (mHallStateArray[mSayac - 1] != 1)
						mHallStateArray[sayacArttir()] = 1;
					break;

				case 3: // 2. state için

//					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, dutyCycle);
//					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_RESET);
//					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
//					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);

					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, dutyCycle);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);

					mHallState3Sayac ++;
					if (mHallStateArray[mSayac - 1] != 3)
					mHallStateArray[sayacArttir()] = 3;
					break;

				case 2: // 6. state için

//					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, dutyCycle);
//					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
//					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_RESET);
//					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);

					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, dutyCycle);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);

					mHallState2Sayac ++;
					if (mHallStateArray[mSayac - 1] != 2)
					mHallStateArray[sayacArttir()] = 2;
					break;

				case 6: // 4. state için

//					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, dutyCycle);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
//					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
//					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_RESET);
//					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);

					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, dutyCycle);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_RESET);
					mHallState6Sayac ++;
					if (mHallStateArray[mSayac - 1] != 6)
					mHallStateArray[sayacArttir()] = 6;
					break;

				case 4: // 5. state için

//					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, dutyCycle);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
//					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
//					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
//					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_RESET);

					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, dutyCycle);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_RESET);
					mHallState4Sayac ++;
					if (mHallStateArray[mSayac - 1] != 4)
					mHallStateArray[sayacArttir()] = 4;
					break;

				case 5: // 1
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, dutyCycle);
//					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
//					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
//					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
//					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_RESET);

					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, dutyCycle);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
					mHallState5Sayac ++;
					if (mHallStateArray[mSayac - 1] != 5)
					mHallStateArray[sayacArttir()] = 5;
					break;


				default:
					__HAL_TIM_SET_COMPARE(htim, tim_channel_a, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_b, 0);
					__HAL_TIM_SET_COMPARE(htim, tim_channel_c, 0);
					HAL_GPIO_WritePin(AL_GPIO_Port, AL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BL_GPIO_Port, BL_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(CL_GPIO_Port, CL_Pin, GPIO_PIN_SET);
					mHallStateDefaultSayac ++;
					break;
			}

    		break;

    }

}
