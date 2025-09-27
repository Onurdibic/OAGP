/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "i2c.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "Mpu.h"
#include "Mag.h"
#include "Paket.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern IMU imu;
extern MAG mag;
Paket GpsPaket(0x12, 0x34, 0x01, 0x0D); //veri boyutu 13
Paket ImuPaket(0x12, 0x34, 0x02, 0x11);//veri boyutu 17
Paket VersiyonPaket(0x12, 0x34, 0x03, 0x04); //veri boyutu 4
Paket YoklamaPaket(0x12, 0x34, 0x04, 0x04);//veri boyutu 4
Paket RotaPaket(0x12, 0x34, 0x05, 0x04);//veri boyutu 4
Paket SistemPaket(0x12, 0x34, 0x06, 0x09);//veri boyutu 9

extern Paket ArayuzPaket;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
float konumX = 0;
float konumY =0;
float hizX = 0;
float hizY =0;
float ivmeX = 0;
float ivmeY =0;
float pitch;
float roll;
float yaw;
float heading;
float irtifa;
float imucipsicaklik;
float barosicaklik;

uint8_t GpsVeriPaket[17]={0};
uint8_t ImuVeriPaket[21]={0};
uint8_t SistemVeriPaket[13]={0};
uint8_t VersiyonVeriPaket[8]={0};
uint8_t YoklamaVeriPaket[8]={0};
uint8_t RotaVeriPaket[8]={0};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId myPaketTaskHandle;
osThreadId myImuTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartPaketTask(void const * argument);
void StartImuTask(void const * argument);


extern "C" void MX_USB_HOST_Init(void);
extern "C" void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
extern "C" void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myPaketTask */
  osThreadDef(myPaketTask, StartPaketTask, osPriorityIdle, 0, 1024);
  myPaketTaskHandle = osThreadCreate(osThread(myPaketTask), NULL);

  /* definition and creation of myImuTask */
  osThreadDef(myImuTask, StartImuTask, osPriorityIdle, 0, 2048);
  myImuTaskHandle = osThreadCreate(osThread(myImuTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_HOST */
  MX_USB_HOST_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartPaketTask */
/**
* @brief Function implementing the myPaketTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPaketTask */
void StartPaketTask(void const * argument)
{
  /* USER CODE BEGIN StartPaketTask */
  /* Infinite loop */
	ArayuzPaket.PaketKesmeYapilandir();
	for(;;)
	{
		if(ArayuzPaket.PaketCozBayrak)
		{
		ArayuzPaket.PaketCozBayrak = false;
		ArayuzPaket.PaketCoz();
			if(ArayuzPaket.VersiyonPaketBayrak==true)
			{
				VersiyonPaket.VersiyonPaketOlustur(0, 0, 2);
				VersiyonPaket.versiyonPaketCagir(VersiyonVeriPaket);
				HAL_UART_Transmit(&huart3, VersiyonVeriPaket, sizeof(VersiyonVeriPaket), 1000);
				ArayuzPaket.VersiyonPaketBayrak=false;
			}
			if(ArayuzPaket.YoklamaPaketFlag==true)
			{
				YoklamaPaket.YoklamaPaketOlustur();
				YoklamaPaket.yoklamaPaketCagir(YoklamaVeriPaket);
				HAL_UART_Transmit(&huart3, YoklamaVeriPaket, sizeof(YoklamaVeriPaket), 1000);
				ArayuzPaket.YoklamaPaketFlag=false;
			}
		}
		if(ArayuzPaket.GidilecekNoktaBayrak==false && ArayuzPaket.RotaGeldiBayrak==true)
		{
			RotaPaket.RotaPaketOlustur();
			RotaPaket.rotaPaketCagir(RotaVeriPaket);
			HAL_UART_Transmit(&huart3, RotaVeriPaket, sizeof(RotaVeriPaket), 1000);
			ArayuzPaket.RotaGeldiBayrak=false;
		}
		osDelay(10);
	}
  /* USER CODE END StartPaketTask */
}

/* USER CODE BEGIN Header_StartImuTask */
/**
* @brief Function implementing the myImuTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartImuTask */
void StartImuTask(void const * argument)
{
  /* USER CODE BEGIN StartImuTask */
  /* Infinite loop */
	uint8_t pozisyonVeriCounter=0;
	heading =*mag.HeadingOlustur(pitch,roll);
	for(;;)
	{
		GPIOD->ODR ^= GPIO_PIN_13;
		imu.aciBul(); //imu pitch roll yaw al

		konumX = imu.konumxX();
		konumY = imu.konumyY();
		hizX = imu.hizxX();
		hizY = imu.hizyY();
		ivmeX = imu.x();
		ivmeY = imu.y();

		// imu.EnlemBoylamGuncelle(basheading,41.2174316,36.4566603);
		//	yeniEnlem=*imu.LatAl();
		//	yeniBoylam=*imu.LongAl();

		pitch=*imu.PitchAl();
		roll=*imu.RollAl();
		yaw=*imu.YawAl();
		imucipsicaklik=*imu.SicaklikAl();
		heading =*mag.HeadingOlustur(pitch,roll);

		pozisyonVeriCounter++;
		if(pozisyonVeriCounter%50 == 0)
		{
			if(ArayuzPaket.YoklamaFlag)
			{
				ImuPaket.ImuPaketOlustur(pitch, roll,heading, imucipsicaklik);
				ImuPaket.imuPaketCagir(ImuVeriPaket);
				HAL_UART_Transmit(&huart3, ImuVeriPaket, sizeof(ImuVeriPaket), 1000);
			}
			pozisyonVeriCounter=0;
		}
		osDelay(5);
	}
  /* USER CODE END StartImuTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
