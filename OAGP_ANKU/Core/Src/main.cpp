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
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "freertos.h"

#ifdef __cplusplus
}
#endif


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Mpu.h"
#include "Mag.h"
#include "Gps.h"
#include "Paket.h"
#include "Kontrol.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
IMU imu(&hi2c1);
MAG mag(&hi2c1);
GPS gps(&huart2);
KontrolLibrary kontrol;
Paket ArayuzPaket(&huart3);
Paket ArabaArkaPaket(&huart4);
Paket ArabaOnPaket(&huart5);
extern Paket GpsPaket;
extern uint8_t GpsVeriPaket[17];
extern Paket KalmanPaket;
extern uint8_t KalmanVeriPaket[13];
extern Paket RPMPaket;
extern uint8_t RPMVeriPaket[13];
extern Paket YoklamaPaket;
extern uint8_t YoklamaVeriPaket[8];
extern Paket VersiyonPaket;
extern uint8_t VersiyonVeriPaket[8];
extern Paket ImuPaket;
extern uint8_t ImuVeriPaket[8];
extern int a;
extern float pitch_f,roll_f, heading_f, imucipsicaklik;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
extern "C" void MX_FREERTOS_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* UART RX buffer */
#define RX_BUFFER_SIZE 128
uint8_t rxBuffer[RX_BUFFER_SIZE];   // CPU buffer
volatile uint16_t rxIndex = 0;
uint8_t txState=0;
uint8_t rxData;

extern float enlemCikti_f;
extern float boylamCikti_f;
extern float enlemKalmanCikti_f;
extern float boylamKalmanCikti_f;
extern float HizdanRPM(float v);
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

  mag.Yapilandir();
  imu.Yapilandir();

//  GPIOD->ODR ^= GPIO_PIN_12;
//  HAL_Delay(1000);
//  imu.kalibreEt();
//  GPIOD->ODR ^= GPIO_PIN_12;
//  HAL_Delay(1000);
//  mag.XveYKalibreEt();
//  GPIOD->ODR ^= GPIO_PIN_12;
  gps.Yapilandir();

  // --- Matris Test SONU ---
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART5)
    {
        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);
        __HAL_UART_CLEAR_NEFLAG(huart);

        HAL_UART_AbortReceive(huart);
        ArabaOnPaket.PaketKesmeYapilandir();
    }
    if (huart->Instance == UART4)
	{
		__HAL_UART_CLEAR_OREFLAG(huart);
		__HAL_UART_CLEAR_FEFLAG(huart);
		__HAL_UART_CLEAR_NEFLAG(huart);

		HAL_UART_AbortReceive(huart);
		ArabaArkaPaket.PaketKesmeYapilandir();
	}
    if (huart->Instance == USART3)
	{
		__HAL_UART_CLEAR_OREFLAG(huart);
		__HAL_UART_CLEAR_FEFLAG(huart);
		__HAL_UART_CLEAR_NEFLAG(huart);

		HAL_UART_AbortReceive(huart);
		ArayuzPaket.PaketKesmeYapilandir();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
	{
		gps.DataOku();
	}
    if(huart->Instance == USART3)
    {
    	ArayuzPaket.DataAlveBayrakKaldir();
    }
    if (huart->Instance == UART4)
	{
    	ArabaArkaPaket.DataAlveBayrakKaldir();
	}
    if (huart->Instance == UART5)
	{
    	ArabaOnPaket.DataAlveBayrakKaldir();
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3)
    {
    	if(ArayuzPaket.VersiyonPaketBayrak==true)
		{
			a=1;
			VersiyonPaket.VersiyonPaketOlustur(0, 0, 7);
			VersiyonPaket.versiyonPaketCagir(VersiyonVeriPaket);
			HAL_UART_Transmit_DMA(&huart3, VersiyonVeriPaket, sizeof(VersiyonVeriPaket));
			ArayuzPaket.VersiyonPaketBayrak=false;
		}
//		if(ArayuzPaket.YoklamaPaketFlag==true)
//		{
//			a=2;
//			YoklamaPaket.YoklamaPaketOlustur();
//			YoklamaPaket.yoklamaPaketCagir(YoklamaVeriPaket);
//			HAL_UART_Transmit_DMA(&huart3, YoklamaVeriPaket, sizeof(YoklamaVeriPaket));
//			ArayuzPaket.YoklamaPaketFlag=false;
//			//yoklamaCounter++;
//		}
		if(ArayuzPaket.YoklamaPaketFlag)
		{
			if(txState == 1)
			{
				ImuPaket.ImuPaketOlustur(pitch_f,roll_f, heading_f, imucipsicaklik);
				ImuPaket.imuPaketCagir(ImuVeriPaket);
				HAL_UART_Transmit_DMA(&huart3, ImuVeriPaket, 17);
				txState = 2;
			}
			else if(txState == 2)
			{
				GpsPaket.GpsPaketOlustur(enlemCikti_f,boylamCikti_f,0,0);
				GpsPaket.gpsPaketCagir(GpsVeriPaket);
				HAL_UART_Transmit_DMA(&huart3, GpsVeriPaket, 17);

				txState = 3;
			}
			else if(txState == 3)
			{
				KalmanPaket.KalmanPaketOlustur(enlemKalmanCikti_f,boylamKalmanCikti_f);
				KalmanPaket.kalmanPaketCagir(KalmanVeriPaket);
				HAL_UART_Transmit_DMA(&huart3, KalmanVeriPaket, 13);
				txState = 4;
			}
			else if(txState == 4)
			{
				float sag_rpm = kontrol.HizdanRPM(ArabaOnPaket.saghiz_f);
				float sol_rpm = kontrol.HizdanRPM(ArabaOnPaket.solhiz_f);

				RPMPaket.RPMPaketOlustur(sag_rpm, sol_rpm);
				RPMPaket.rpmPaketCagir(RPMVeriPaket);
				HAL_UART_Transmit_DMA(&huart3, RPMVeriPaket, 13);
				ArabaArkaPaket.saghiz_f=0;
				ArabaOnPaket.saghiz_f=0;
				ArabaArkaPaket.solhiz_f=0;
				ArabaOnPaket.solhiz_f=0;
				txState = 0;
			}
		}


    }
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
