/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Motor.h"
/* USER CODE END Includes */
	   uint32_t dutyCycle = 0;
	   uint32_t komutasyon = 0;
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Motor nesneleri
Motor motor1(&htim1,&htim2,TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3,
             M1_AL_GPIO_Port, M1_AL_Pin,
             M1_BL_GPIO_Port, M1_BL_Pin,
             M1_CL_GPIO_Port, M1_CL_Pin,
             M1_HALL_A_GPIO_Port, M1_HALL_A_Pin,
             M1_HALL_B_GPIO_Port, M1_HALL_B_Pin,
             M1_HALL_C_GPIO_Port, M1_HALL_C_Pin);

Motor motor2(&htim8,&htim2,TIM_CHANNEL_1, TIM_CHANNEL_3, TIM_CHANNEL_2,

			 M2_AL_GPIO_Port, M2_AL_Pin,
             M2_BL_GPIO_Port, M2_BL_Pin,
			 M2_CL_GPIO_Port, M2_CL_Pin,

             M2_HALL_A_GPIO_Port, M2_HALL_A_Pin,
             M2_HALL_B_GPIO_Port, M2_HALL_B_Pin,
             M2_HALL_C_GPIO_Port, M2_HALL_C_Pin);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
void motorKomutasyon();
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t M1_Hall_A, M1_Hall_B, M1_Hall_C;

uint8_t M2_Hall_A, M2_Hall_B, M2_Hall_C;

uint8_t M1HallState, M2HallState;
uint8_t komutasyon_u8 = 0;
uint8_t surusAktifFlag=0;
uint8_t komutasyonSuresi=200;
uint16_t dolulukOrani=200;
float dt_sec=0;
uint16_t m1Changes=0;
uint16_t m2Changes=0;
float m1_rad_s=0;
float m2_rad_s=0;
float m1_rpm=0;
float m2_rpm=0;
uint16_t counter=0;
uint16_t sayi=75;
float m1_speed_ms=0;
float m2_speed_ms=0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
  HAL_TIM_Base_Start_IT(&htim2);

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
  __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
  __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 0);
  __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 0);

  HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M2_AL_GPIO_Port, M2_AL_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M2_BL_GPIO_Port, M2_BL_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M2_CL_GPIO_Port, M2_CL_Pin, GPIO_PIN_SET);

  motor1.init();
  motor2.init();

  motor1.aktif = false;
  motor2.aktif = false;
  motor1.setDirection(ILERI);
  motor2.setDirection(ILERI);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	 // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
//	  HAL_Delay(200);
	  motor1.komutasyon();
	  motor2.komutasyon();



  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == M1_HALL_A_Pin || GPIO_Pin == M1_HALL_B_Pin || GPIO_Pin == M1_HALL_C_Pin)
	{
		motor1.updateHall();
		motor1.hallCounter++;
	}

	if (GPIO_Pin == M2_HALL_A_Pin || GPIO_Pin == M2_HALL_B_Pin || GPIO_Pin == M2_HALL_C_Pin)
	{
		motor2.updateHall();
		motor2.hallCounter++;
	}
	for(int i=0;i<sayi;i++)
	{

	}
}

#define HALLS_PER_REV 90
#define DT_SEC 0.005f  // 5 ms
#define RADIUS 0.085f  // metre

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        if(counter % 200 == 0)
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
            m1Changes = motor1.hallCounter;
            m2Changes = motor2.hallCounter;

            // Mekanik devir/s
            float m1_rev_s = m1Changes / (float)HALLS_PER_REV / 1;
            float m2_rev_s = m2Changes / (float)HALLS_PER_REV / 1;

            // Radyan/s
            m1_rad_s = m1_rev_s * 2.0f * 3.14159265f;
            m2_rad_s = m2_rev_s * 2.0f * 3.14159265f;

            // RPM
            m1_rpm = m1_rev_s * 60.0f;
            m2_rpm = m2_rev_s * 60.0f;

            // Doğrusal hız (m/s)
            m1_speed_ms = m1_rad_s * RADIUS;
            m2_speed_ms = m2_rad_s * RADIUS;

            // Sayaçları resetle
            motor1.hallCounter = 0;
            motor2.hallCounter = 0;
            counter = 0;
        }
        counter++;
    }
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
