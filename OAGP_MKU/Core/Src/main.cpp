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
#include <cstdlib>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Motor.h"
#include "Paket.h"
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

Paket ANKUPaket(&huart3);
Paket TekerPaket(0x12, 0x34, 0x06, 0x09);
Paket RPMPaket(0x12, 0x34, 0x08, 0x09);
MotorDirection baseDir;
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

typedef struct {
    int rpm;
    int maxPwm;
} RpmLimit;

const RpmLimit rpmLimitTable[] = {
    {0,   100},
    {10,  110},
    {20,  120},
    {30,  130},
    {40,  140},
    {50,  150},
    {60,  160},
    {70,  160},
    {80,  160},
    {90,  160},
    {100, 160},
    {110, 170},
    {120, 170},
    {130, 170},
    {140, 180},
    {150, 190},
    {160, 200},
    {170, 210},
    {180, 220},
    {190, 230},
    {200, 240},
    {210, 250},
    {220, 260},
    {230, 270},
    {240, 280},
    {250, 290},
    {260, 300},
    {270, 310},
    {280, 320},
    {290, 330},
    {300, 340},
    {310, 350},
    {320, 360},
    {330, 370},
    {340, 380},
    {350, 390},
    {360, 400},
    {370, 410},
    {380, 420},
    {390, 430},
    {400, 440},
    {410, 450},
    {420, 460}
};

const int rpmLimitTableSize = sizeof(rpmLimitTable) / sizeof(rpmLimitTable[0]);

// Gelen komut değişkenleri
float yon_f = 0.0f;
float sagRpm_f = 0.0f;
float solRpm_f = 0.0f;
int counterrr=0;
// PI hata değişkenleri (motor 1 ve 2)
float hata1 = 0.0f;
float hata2 = 0.0f;

float prev_motor1pwm = 0.0f;
float prev_motor2pwm = 0.0f;

float maxIncrease_motor1 = 0.0f;
float maxIncrease_motor2 = 0.0f;

float pwmLimit1 = 255.0f;
float pwmLimit2 = 255.0f;
// Integral akümülatörleri (PI kontrol)
float integral1 = 0.0f;
float integral2 = 0.0f;

// PWM çıkışları
int pwm1 = 0;
int pwm2 = 0;
int motor1pwm = 0;
int motor2pwm = 0;
float sag_hiz =0;
float sol_hiz=0;
uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t rxData;
uint8_t tekerBuffer[13];
uint8_t rpmBuffer[13];

volatile uint16_t rxIndex = 0;

volatile uint8_t flag_10ms  = 0;
volatile uint8_t flag_20ms = 0;
volatile uint8_t flag_100ms = 0;
volatile uint8_t flag_500ms = 0;
volatile uint8_t flag_1000ms = 0;

volatile uint16_t tickCounter = 0;
int MotorPIKontrol(float rpm_hedef, float rpm_olculen, float *integral);

int getMaxPwmFromRpm(float rpm)
{
    int last = rpmLimitTable[0].maxPwm;

    for (int i = 0; i < rpmLimitTableSize; i++)
    {
        if (rpm >= rpmLimitTable[i].rpm)
            last = rpmLimitTable[i].maxPwm;
        else
            break;
    }

    return last;
}


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
  MX_DMA_Init();
  MX_USART3_UART_Init();

  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  ANKUPaket.PaketKesmeYapilandir();

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);

  __HAL_TIM_MOE_ENABLE(&htim1);
  __HAL_TIM_MOE_ENABLE(&htim8);

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
  motor1.aktif = true;
  motor2.aktif = true;
  motor1.setDirection(ILERI);
  motor2.setDirection(ILERI);
  motor1.updateHall();
  motor2.updateHall();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  motor1.komutasyon(motor1pwm);
	  motor2.komutasyon(motor2pwm);
	  if(flag_10ms)
	  {
		flag_10ms = 0;
		// 10 ms işlemleri
		ANKUPaket.PaketCoz();

	  }

//	  if (flag_20ms)
//	  {
//	      flag_20ms = 0;
//
//	      // --- Yön Al ---
//	      yon_f = ANKUPaket.gelenYonAl();
//	      sagRpm_f = ANKUPaket.gelenSagRpmAl();
//	      solRpm_f = ANKUPaket.gelenSolRpmAl();
////	      sagRpm_f = 100;
////		  solRpm_f = 100;
//	      if (yon_f == 1) { motor1.setDirection(ILERI); motor2.setDirection(ILERI); }
//	      else if (yon_f == 2) { motor1.setDirection(GERI); motor2.setDirection(GERI); }
//
//	      // --- Hız Filtreleme ---
//	      motor1.hizHesaplaFiltered(0.02f);
//	      motor2.hizHesaplaFiltered(0.02f);
//
//	      // --- RPM bazlı maksimum PWM limit uygula ---
//		  int sagMaxPwm = getMaxPwmFromRpm(motor1.m_rpm); // Anlık RPM okuma
//		  int solMaxPwm = getMaxPwmFromRpm(motor2.m_rpm);
//
//	      // --- PWM hesapla ---
//	      motor1pwm = motor1.updatePWM(sagRpm_f, 0.02f,sagMaxPwm);
//	      motor2pwm = motor2.updatePWM(solRpm_f, 0.02f,solMaxPwm);
//
//	      if (motor1pwm > sagMaxPwm) motor1pwm = sagMaxPwm;
//	      if (motor2pwm > solMaxPwm) motor2pwm = solMaxPwm;
//
//	      // --- PWM Artış Hesabı (Sadece artış, azalma yok) ---
////	      float inc1 = 0.0f;
////	      float inc2 = 0.0f;
////
////	      if (motor1pwm > prev_motor1pwm)
////	          inc1 = motor1pwm - prev_motor1pwm;
////
////	      if (motor2pwm > prev_motor2pwm)
////	          inc2 = motor2pwm - prev_motor2pwm;
////
////	      if (inc1 > maxIncrease_motor1) maxIncrease_motor1 = inc1;
////	      if (inc2 > maxIncrease_motor2) maxIncrease_motor2 = inc2;
////
////	      prev_motor1pwm = motor1pwm;
////	      prev_motor2pwm = motor2pwm;
//
//	      // --- Komutasyon uygula ---
//	      motor1.komutasyon(motor1pwm);
//	      motor2.komutasyon(motor2pwm);
//
//	      // --- Hız çıkışları ---
//	      sag_hiz = motor1.m_speed_ms;
//	      sol_hiz = motor2.m_speed_ms;
//
//	      if (motor1.getDirection() == GERI)
//	      {
//	          sag_hiz = -sag_hiz;
//	          sol_hiz = -sol_hiz;
//	      }
//
//	      // --- Paket oluştur & gönder ---
//	      TekerPaket.TekerPaketOlustur(sag_hiz, sol_hiz);
//	      TekerPaket.tekerPaketCagir(tekerBuffer);
//	      HAL_UART_Transmit_DMA(&huart3, tekerBuffer, sizeof(tekerBuffer));
//	  }

	  if (flag_20ms)
	  {
	      flag_20ms = 0;

	      yon_f     = ANKUPaket.gelenYonAl();
	      sagRpm_f = ANKUPaket.gelenSagRpmAl();
	      solRpm_f = ANKUPaket.gelenSolRpmAl();

	      // --- Paket yönü ---
	      MotorDirection baseDir;
	      if (yon_f == 1)
	          baseDir = ILERI;
	      else if (yon_f == 2)
	          baseDir = GERI;
	      else
	          baseDir = ILERI;

	      // --- Hedef RPM işaretinden yön ---
	      MotorDirection hedefDir1 =
	          (sagRpm_f >= 0) ? baseDir : (baseDir == ILERI ? GERI : ILERI);

	      MotorDirection hedefDir2 =
	          (solRpm_f >= 0) ? baseDir : (baseDir == ILERI ? GERI : ILERI);

	      motor1.setDirectionSoft(hedefDir1);
	      motor2.setDirectionSoft(hedefDir2);

	      // --- Hız filtreleme ---
	      motor1.hizHesaplaFiltered(0.02f);
	      motor2.hizHesaplaFiltered(0.02f);

	      // --- Mutlak hedef RPM ---
	      int sagRpmAbs = (sagRpm_f >= 0) ? sagRpm_f : -sagRpm_f;
	      int solRpmAbs = (solRpm_f >= 0) ? solRpm_f : -solRpm_f;

	      // --- Max PWM ---
	      int sagMaxPwm = getMaxPwmFromRpm(motor1.m_rpm);
	      int solMaxPwm = getMaxPwmFromRpm(motor2.m_rpm);

	      // --- PWM hesapla (artık her zaman +) ---
	      motor1pwm = motor1.updatePWM(sagRpmAbs, 0.02f, sagMaxPwm);
	      motor2pwm = motor2.updatePWM(solRpmAbs, 0.02f, solMaxPwm);

	      if (motor1pwm > sagMaxPwm) motor1pwm = sagMaxPwm;
	      if (motor2pwm > solMaxPwm) motor2pwm = solMaxPwm;

	      // --- Komütasyon ---
	      motor1.komutasyon(motor1pwm);
	      motor2.komutasyon(motor2pwm);

	      // --- Hız çıkışları ---
	      sag_hiz = motor1.m_speed_ms;
	      sol_hiz = motor2.m_speed_ms;

	      if (motor1.getDirection() == GERI)
	          sag_hiz = -sag_hiz;

	      if (motor2.getDirection() == GERI)
	          sol_hiz = -sol_hiz;


	      TekerPaket.TekerPaketOlustur(sag_hiz, sol_hiz);
	      TekerPaket.tekerPaketCagir(tekerBuffer);
	      HAL_UART_Transmit_DMA(&huart3, tekerBuffer, sizeof(tekerBuffer));
	  }

	  if(flag_100ms)
	  {
		  flag_100ms = 0;
		  // 100 ms işlemleri
//		  		  motor1.hizHesaplaFiltered(0.1f);
//		  		  motor2.hizHesaplaFiltered(0.1f);
	  // --- Paket oluştur & gönder ---
//		  RPMPaket.RPMPaketOlustur(
//		      motor1.m_rpm,
//		      motor2.m_rpm
//		  );
//
//		  RPMPaket.rpmPaketCagir(rpmBuffer);
//		  HAL_UART_Transmit_DMA(&huart3, rpmBuffer, sizeof(rpmBuffer));

	  }

	  if(flag_500ms)
	  {
		  flag_500ms = 0;
		  // 500 ms işlemleri
		  // --- PI Kontrol Önce Anlık RPM Değeri ---
		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	  }

	  if(flag_1000ms)
	  {
		  flag_1000ms = 0;
		  // 1 saniyelik işlemler
//		  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);

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
		//motor1.komutasyon(motor1pwm);
	}

	if (GPIO_Pin == M2_HALL_A_Pin || GPIO_Pin == M2_HALL_B_Pin || GPIO_Pin == M2_HALL_C_Pin)
	{
		motor2.updateHall();
		//motor2.komutasyon(motor2pwm);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
       tickCounter++;  // 5 ms sayacı
       if(tickCounter % 2 == 0)    // 10 ms (2 * 5 ms)
      		   flag_10ms = 1;

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
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
    	ANKUPaket.DataAlveBayrakKaldir();
    	counterrr++;

    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        __HAL_UART_CLEAR_OREFLAG(huart);
        __HAL_UART_CLEAR_FEFLAG(huart);
        __HAL_UART_CLEAR_NEFLAG(huart);

        HAL_UART_AbortReceive(huart);
        ANKUPaket.PaketKesmeYapilandir();
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
