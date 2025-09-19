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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
	HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(M2_AL_GPIO_Port, M2_AL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(M2_BL_GPIO_Port, M2_BL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(M2_CL_GPIO_Port, M2_CL_Pin, GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
//	  motorKomutasyon();
//	  HAL_Delay(200);
	  /* Sağ hall oku */
//	  	  M1_Hall_A = HAL_GPIO_ReadPin(M1_HALL_A_GPIO_Port, M1_HALL_A_Pin);
//	  	  M1_Hall_B = HAL_GPIO_ReadPin(M1_HALL_B_GPIO_Port, M1_HALL_B_Pin);
//	      M1_Hall_C = HAL_GPIO_ReadPin(M1_HALL_C_GPIO_Port, M1_HALL_C_Pin);
//
//	      M1HallState = (M1_Hall_A << 2) | (M1_Hall_B << 1) | M1_Hall_C;

	      /* Sol hall oku */
//	      M2_Hall_A = HAL_GPIO_ReadPin(M2_HALL_A_GPIO_Port, M2_HALL_A_Pin);
//	      M2_Hall_B = HAL_GPIO_ReadPin(M2_HALL_B_GPIO_Port, M2_HALL_B_Pin);
//	      M2_Hall_C = HAL_GPIO_ReadPin(M2_HALL_C_GPIO_Port, M2_HALL_C_Pin);
//
//	      M2HallState = (M2_Hall_A << 2) | (M2_Hall_B << 1) | M2_Hall_C;


	      //HAL_Delay(komutasyonSuresi);
	      if(surusAktifFlag)
	      {
//	    	  komutasyon_u8++;
//	    	  if(komutasyon_u8>6)
//	    	  {
//	    		  komutasyon_u8=1;
//	    	  }
	    	  motorKomutasyon();
	      }
	      else
	      {
	    		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	    		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
	    		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
	    		HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_SET);
	    		HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_SET);
	    		HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_SET);
	    		HAL_GPIO_WritePin(M2_AL_GPIO_Port, M2_AL_Pin, GPIO_PIN_SET);
	    		HAL_GPIO_WritePin(M2_BL_GPIO_Port, M2_BL_Pin, GPIO_PIN_SET);
	    		HAL_GPIO_WritePin(M2_CL_GPIO_Port, M2_CL_Pin, GPIO_PIN_SET);
	      }

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
int HallStateLookUp[7] = {0,5, 4, 6, 2, 3, 1 };
void motorKomutasyon()
{
	switch (M1HallState)
	{
	case 5: //4
	{

		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, dolulukOrani); // U fazı PWM
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
		HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_SET);
		break;
	}
	case 1: //5
	{

		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, dolulukOrani);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
		HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_RESET);
		break;
	}
	case 3: //1
	{

		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dolulukOrani);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
		HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_RESET);
		break;
	}
	case 2: //3
	{

		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, dolulukOrani);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
		HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_SET);
		break;
	}
	case 6: //2
	{

		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, dolulukOrani);
		HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_SET);
		break;
	}
	case 4: //6
	{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, dolulukOrani);
		HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_SET);
		break;
	}
	default:
	{
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
		HAL_GPIO_WritePin(M1_AL_GPIO_Port, M1_AL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_BL_GPIO_Port, M1_BL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M1_CL_GPIO_Port, M1_CL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M2_AL_GPIO_Port, M2_AL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M2_BL_GPIO_Port, M2_BL_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(M2_CL_GPIO_Port, M2_CL_Pin, GPIO_PIN_SET);
		break;
	}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	M1_Hall_A = HAL_GPIO_ReadPin(M1_HALL_A_GPIO_Port, M1_HALL_A_Pin);
	M1_Hall_B = HAL_GPIO_ReadPin(M1_HALL_B_GPIO_Port, M1_HALL_B_Pin);
	M1_Hall_C = HAL_GPIO_ReadPin(M1_HALL_C_GPIO_Port, M1_HALL_C_Pin);
	M1HallState = (M1_Hall_A << 2) | (M1_Hall_B << 1) | M1_Hall_C;

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
