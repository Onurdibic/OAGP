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
#include "usart.h"
#include "dma.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Motor.h"
/* USER CODE END Includes */
#define RX_BUFFER_SIZE 128   // buffer boyutu, ihtiyacına göre artırabilirsin
#define HALLS_PER_REV 90
#define DT_SEC 0.005f  // 5 ms
#define RADIUS 0.085f  // metre
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
uint16_t motor1pwm=0;
uint16_t motor2pwm=0;
float hedefRPM_motor1 = 300.0f;
float hedefRPM_motor2 = 0.0f;
float hata=0;
float Kp = 0.5f;
float Ki = 0.6f;
float integral = 0.0f;
float I_LIMIT = 2000.0f;

uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t rxData;
uint8_t txByte = 0x55;
volatile uint16_t rxIndex = 0;

volatile uint8_t flag_5ms  = 0;
volatile uint8_t flag_20ms = 0;
volatile uint8_t flag_100ms = 0;
volatile uint8_t flag_500ms = 0;
volatile uint8_t flag_1000ms = 0;

volatile uint16_t tickCounter = 0;

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
  MX_USART3_UART_Init();
  MX_DMA_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_UART_Receive_DMA(&huart3, &rxData, 1);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);

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
	  motor1.komutasyon(motor1pwm);
	  motor2.komutasyon(motor2pwm);

	  if(flag_5ms)
	  {
		  flag_5ms = 0;
		  // 5 ms işlemleri
	  }

	  if(flag_20ms)
	  {
		  flag_20ms = 0;
		  // 20 ms işlemleri
		  // Motor1 hız kontrolü
		  if(motor1.aktif)
		  {
			  float mevcutRPM = motor1.m_rpm;
			  hata = hedefRPM_motor1 - mevcutRPM;

			  integral += hata * 0.02f;  // 20 ms = 0.02 s
			  if(integral > I_LIMIT)  integral = I_LIMIT;
			  if(integral < -I_LIMIT) integral = -I_LIMIT;

			  // PWM hesapla
			  float kontrol = (Kp * hata) + (Ki * integral);

			  motor1pwm = (int)kontrol;

			  // PWM limitleri
			  if(motor1pwm < 0)   motor1pwm = 0;
			  if(motor1pwm > 650) motor1pwm = 650;
		  }

	  }

	  if(flag_100ms)
	  {
		  flag_100ms = 0;
		  // 100 ms işlemleri


	  }

	  if(flag_500ms)
	  {
		  flag_500ms = 0;
		  // 500 ms işlemleri
		  uint8_t txBuf[1] = {0x55};
		  HAL_UART_Transmit_DMA(&huart3, txBuf, 1);

	  }

	  if(flag_1000ms)
	  {
		  flag_1000ms = 0;
		  // 1 saniyelik işlemler
		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);

		  motor1.hizHesapla(1);
		  motor2.hizHesapla(1);
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == M1_HALL_A_Pin || GPIO_Pin == M1_HALL_B_Pin || GPIO_Pin == M1_HALL_C_Pin)
	{
		motor1.updateHall();
//		motor1.hallCounter++;
	}

	if (GPIO_Pin == M2_HALL_A_Pin || GPIO_Pin == M2_HALL_B_Pin || GPIO_Pin == M2_HALL_C_Pin)
	{
		motor2.updateHall();
//		motor2.hallCounter++;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
       tickCounter++;  // 5 ms sayacı

	   if(tickCounter % 4 == 0)    // 20 ms (4 * 5 ms)
		   flag_20ms = 1;

	   if(tickCounter % 20 == 0)   // 100 ms (20 * 5 ms)
		   flag_100ms = 1;

	   if(tickCounter % 100 == 0)  // 500 ms (100 * 5 ms)
		   flag_500ms = 1;

	   if(tickCounter % 200 == 0)  // 1000 ms (200 * 5 ms = 1 sn)
	   {
		   flag_1000ms = 1;
		   tickCounter = 0; // Sayaç sıfırla (overflow olmasın)
	   }

    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        // gelen byte’ı buffer’a koy
        rxBuffer[rxIndex++] = rxData;
        if(rxIndex >= RX_BUFFER_SIZE) rxIndex = 0;

        // tekrar DMA başlat (1 byte)
        HAL_UART_Receive_DMA(&huart3, &rxData, 1);
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
