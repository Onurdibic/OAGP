/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M2_AL_Pin GPIO_PIN_7
#define M2_AL_GPIO_Port GPIOA
#define M2_CL_Pin GPIO_PIN_0
#define M2_CL_GPIO_Port GPIOB
#define M2_BL_Pin GPIO_PIN_1
#define M2_BL_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_2
#define LED_GPIO_Port GPIOB
#define M1_AL_Pin GPIO_PIN_13
#define M1_AL_GPIO_Port GPIOB
#define M1_BL_Pin GPIO_PIN_14
#define M1_BL_GPIO_Port GPIOB
#define M1_CL_Pin GPIO_PIN_15
#define M1_CL_GPIO_Port GPIOB
#define M2_AH_Pin GPIO_PIN_6
#define M2_AH_GPIO_Port GPIOC
#define M2_CH_Pin GPIO_PIN_7
#define M2_CH_GPIO_Port GPIOC
#define M2_BH_Pin GPIO_PIN_8
#define M2_BH_GPIO_Port GPIOC
#define M1_AH_Pin GPIO_PIN_8
#define M1_AH_GPIO_Port GPIOA
#define M1_BH_Pin GPIO_PIN_9
#define M1_BH_GPIO_Port GPIOA
#define M1_CH_Pin GPIO_PIN_10
#define M1_CH_GPIO_Port GPIOA
#define M1_HALL_C_Pin GPIO_PIN_10
#define M1_HALL_C_GPIO_Port GPIOC
#define M1_HALL_C_EXTI_IRQn EXTI15_10_IRQn
#define M1_HALL_B_Pin GPIO_PIN_11
#define M1_HALL_B_GPIO_Port GPIOC
#define M1_HALL_B_EXTI_IRQn EXTI15_10_IRQn
#define M1_HALL_A_Pin GPIO_PIN_12
#define M1_HALL_A_GPIO_Port GPIOC
#define M1_HALL_A_EXTI_IRQn EXTI15_10_IRQn
#define M2_HALL_B_Pin GPIO_PIN_5
#define M2_HALL_B_GPIO_Port GPIOB
#define M2_HALL_B_EXTI_IRQn EXTI9_5_IRQn
#define M2_HALL_C_Pin GPIO_PIN_6
#define M2_HALL_C_GPIO_Port GPIOB
#define M2_HALL_C_EXTI_IRQn EXTI9_5_IRQn
#define M2_HALL_A_Pin GPIO_PIN_7
#define M2_HALL_A_GPIO_Port GPIOB
#define M2_HALL_A_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
