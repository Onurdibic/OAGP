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
#include "usart.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Gps.h"
#include "Mpu.h"
#include "Mag.h"
#include "Paket.h"
#include <stdio.h>
#include "Matris.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern IMU imu;
extern MAG mag;
extern GPS gps;
Paket GpsPaket(0x12, 0x34, 0x01, 0x0D); //veri boyutu 13
Paket KalmanPaket(0x12, 0x34, 0x08, 0x09); //veri boyutu 13
Paket ImuPaket(0x12, 0x34, 0x02, 0x0D);//veri boyutu 13
Paket VersiyonPaket(0x12, 0x34, 0x03, 0x04); //veri boyutu 4
Paket YoklamaPaket(0x12, 0x34, 0x04, 0x04);//veri boyutu 4
Paket RotaPaket(0x12, 0x34, 0x05, 0x04);//veri boyutu 4
Paket SistemPaket(0x12, 0x34, 0x06, 0x09);//veri boyutu 9
Paket KomutPaket(0x12,0x34, 0x07, 0x0D); //veri boyutu 13

extern Paket ArayuzPaket;
extern Paket ArabaArkaPaket;
extern Paket ArabaOnPaket;

extern uint8_t txState;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define EARTH_RADIUS 6371000.0f  // Dünya yarıçapı (metre)
float enlem_f=0,boylam_f=0;
float enlemCikti_f=0,boylamCikti_f=0;

float arkaSagTekerHiz=0.0f;
float arkaSolTekerHiz=0.0f;
float onSagTekerHiz=0.0f;
float onSolTekerHiz=0.0f;

float konumX = 0.0f;
float konumY =0.0f;
float hizX = 0.0f;
float hizY =0.0f;
float ivmeX = 0.0f;
float ivmeY =0.0f;

float pitch;
float roll;
float yaw;
float heading;
float irtifa;
float imucipsicaklik;
float barosicaklik;
float x_dead=0;
float y_dead=0;
float enlemKalmanCikti_f=0;
float boylamKalmanCikti_f=0;
float enlem_ref=0;
float boylam_ref=0;
int a=0;
int yoklamaCounter=0;

uint8_t GpsVeriPaket[17]={0};
uint8_t ImuVeriPaket[17]={0};
uint8_t VersiyonVeriPaket[8]={0};
uint8_t YoklamaVeriPaket[8]={0};
uint8_t RotaVeriPaket[8]={0};
uint8_t SistemVeriPaket[13]={0};
uint8_t KomutVeriPaket[17]={0};
uint8_t KalmanVeriPaket[13]={0};
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
osThreadId myKonumTaskHandle;
osThreadId myKalmanTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartPaketTask(void const * argument);
void StartImuTask(void const * argument);
void StartKonumTask(void const * argument);
void StartKalmanTask(void const * argument);

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
	osThreadDef(myImuTask, StartImuTask, osPriorityIdle, 0, 1024);
	myImuTaskHandle = osThreadCreate(osThread(myImuTask), NULL);

	/* definition and creation of myKonumTask */
	osThreadDef(myKonumTask, StartKonumTask, osPriorityIdle, 0, 1024);
	myKonumTaskHandle = osThreadCreate(osThread(myKonumTask), NULL);

	/* definition and creation of myKalmanTask */
	osThreadDef(myKalmanTask, StartKalmanTask, osPriorityHigh, 0, 1024);
	myKalmanTaskHandle = osThreadCreate(osThread(myKalmanTask), NULL);

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
    /* USER CODE BEGIN StartDefaultTask */
    uint32_t prevTime = xTaskGetTickCount();
    uint16_t counter = 0;

    for(;;)
    {
    	counter++;
        // IMU paketi gönderimi
        if(ArayuzPaket.YoklamaFlag)
        {
            ImuPaket.ImuPaketOlustur(pitch, roll, heading, imucipsicaklik);
            ImuPaket.imuPaketCagir(ImuVeriPaket);

            if(txState == 0)
			{
				HAL_UART_Transmit_DMA(&huart3, ImuVeriPaket, 17);
				txState = 1;
			}
        }
        if(counter>=6)
        {
        	counter=0;
        	if(yoklamaCounter<2)
        	{
//        		ArayuzPaket.ileriDurBayrak=true;
        	}
        	yoklamaCounter=0;
        }

        osDelayUntil(&prevTime, 500);
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
	uint32_t prevTime = xTaskGetTickCount();

	ArayuzPaket.PaketKesmeYapilandir();
	ArabaArkaPaket.PaketKesmeYapilandir();
	ArabaOnPaket.PaketKesmeYapilandir();

	for(;;)
	{
		ArabaArkaPaket.PaketCoz();
		ArabaOnPaket.PaketCoz();
		ArayuzPaket.PaketCoz();


		if(ArayuzPaket.VersiyonPaketBayrak==true)
		{
			a=1;
			VersiyonPaket.VersiyonPaketOlustur(0, 0, 7);
			VersiyonPaket.versiyonPaketCagir(VersiyonVeriPaket);
			HAL_UART_Transmit(&huart3, VersiyonVeriPaket, sizeof(VersiyonVeriPaket), 1000);
			ArayuzPaket.VersiyonPaketBayrak=false;
		}
		if(ArayuzPaket.YoklamaPaketFlag==true)
		{
			a=2;
			YoklamaPaket.YoklamaPaketOlustur();
			YoklamaPaket.yoklamaPaketCagir(YoklamaVeriPaket);
			HAL_UART_Transmit(&huart3, YoklamaVeriPaket, sizeof(YoklamaVeriPaket), 1000);
			ArayuzPaket.YoklamaPaketFlag=false;
			yoklamaCounter++;
		}
		if(ArayuzPaket.ileriGitBayrak==true)
		{
			a=3;
			KomutPaket.KomutPaketOlustur(1, 200, 200);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart4, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			KomutPaket.KomutPaketOlustur(1, 200, 200);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart5, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			ArayuzPaket.ileriGitBayrak=false;
		}
		if(ArayuzPaket.geriGitBayrak==true)
		{
			a=4;
			KomutPaket.KomutPaketOlustur(2, 200, 200);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart4, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			KomutPaket.KomutPaketOlustur(2, 200, 200);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart5, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			ArayuzPaket.geriGitBayrak=false;
		}
		if(ArayuzPaket.sagaGitBayrak==true)
		{
			a=5;
			KomutPaket.KomutPaketOlustur(1, 150, 250);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart4, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			KomutPaket.KomutPaketOlustur(1, 150, 250);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart5, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			ArayuzPaket.sagaGitBayrak=false;
		}
		if(ArayuzPaket.solaGitBayrak==true)
		{
			a=6;
			KomutPaket.KomutPaketOlustur(1, 250, 150);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart4, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			KomutPaket.KomutPaketOlustur(1, 250, 150);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart5, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			ArayuzPaket.solaGitBayrak=false;
		}
		if(ArayuzPaket.ileriDurBayrak==true)
		{
			a=7;
			KomutPaket.KomutPaketOlustur(1, 0,0);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart4, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			KomutPaket.KomutPaketOlustur(1, 0,0);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart5, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			ArayuzPaket.ileriDurBayrak=false;
		}
		if(ArayuzPaket.geriDurBayrak==true)
		{
			a=8;
			KomutPaket.KomutPaketOlustur(2, 0,0);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart4, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			KomutPaket.KomutPaketOlustur(2, 0,0);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart5, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			ArayuzPaket.geriDurBayrak=false;
		}
		if(ArayuzPaket.kalibrasyonIMUBayrak==true)
		{
			a=9;
			KomutPaket.KomutPaketOlustur(1, 0,0);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart4, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			KomutPaket.KomutPaketOlustur(1, 0,0);
			KomutPaket.komutPaketCagir(KomutVeriPaket);
			HAL_UART_Transmit(&huart5, KomutVeriPaket, sizeof(KomutVeriPaket), 1000);
			imu.kalibreEt();
			ArayuzPaket.kalibrasyonIMUBayrak=false;
		}
		if(ArayuzPaket.kalibrasyonMAGBayrak==true)
		{
			a=10;

			mag.XveYKalibreEt();
			ArayuzPaket.kalibrasyonMAGBayrak=false;
		}

		if(ArayuzPaket.GidilecekNoktaBayrak==false && ArayuzPaket.RotaGeldiBayrak==true)
		{
			RotaPaket.RotaPaketOlustur();
			RotaPaket.rotaPaketCagir(RotaVeriPaket);
			HAL_UART_Transmit(&huart3, RotaVeriPaket, sizeof(RotaVeriPaket), 1000);
			ArayuzPaket.RotaGeldiBayrak=false;
		}

		osDelayUntil(&prevTime, 10);
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

	uint32_t prevTime = xTaskGetTickCount();
	uint32_t prevTimeDt = xTaskGetTickCount();

	for(;;)
	{
		uint32_t now = xTaskGetTickCount();
		float dt = (now - prevTimeDt)/1000.0f; // saniye
		prevTimeDt = now;

		imu.aciBul();
		pitch = *imu.PitchAl();
		roll = *imu.RollAl();
		yaw = *imu.YawAl();
		imucipsicaklik = *imu.SicaklikAl();
		heading = *mag.HeadingOlustur(pitch,roll);

		//imu.AccToKonum(dt);

		osDelayUntil(&prevTime, 20);
	}
  /* USER CODE END StartImuTask */
}

/* USER CODE END Header_StartKonumTask */
void StartKonumTask(void const * argument)
{
  /* USER CODE BEGIN StartKonumTask */
  uint32_t prevTime = xTaskGetTickCount();
  float dt_f = 0.02f;

  for(;;)
  {
	  enlem_f  = *gps.LatitudeAl();
	  boylam_f = *gps.LongitudeAl();

	  gps.YeniKonumHesapla(enlem_f,boylam_f,heading,
	            ArabaArkaPaket.saghiz_f,ArabaArkaPaket.solhiz_f,dt_f,
	            &gps.gpsdeadreset,&enlemCikti_f,&boylamCikti_f);

	//	float ortalama_hiz = (ArabaArkaPaket.saghiz_f + ArabaArkaPaket.solhiz_f) / 2.0f;
		float ortalama_hiz = (2.0 + 2.0) / 2.0f;
		float mesafe = ortalama_hiz * 0.02;
		float heading_rad = heading * (M_PI/180.0f);
		if(enlem_f >= 1.0f && boylam_f >= 1.0f)
		{
			x_dead += mesafe * sinf(heading_rad); // doğu
			y_dead += mesafe * cosf(heading_rad); // kuzey
		}

	  osDelayUntil(&prevTime, 20);
  }
  /* USER CODE END StartKonumTask */
}


void StartKalmanTask(void const * argument)
{
    /* USER CODE BEGIN StartKalmanTask */
    bool refAtama=true;
    float X[2] = {0.0f, 0.0f}; // x, y (sadece delta)
    float P[2][2] = {{1.0f,0},{0,1.0f}}; // covariance
    float Q[2][2] = {{1.0f,0},{0,1.0f}};     // process noise (tekerlek hatası)
    float R[2][2] = {{4.0f,0},{0,4.0f}};    // measurement noise (GPS ~2m hatası)

    uint32_t prevTime = xTaskGetTickCount();

    for(;;)
    {
        // GPS geldiğinde
        if (gps.gpskalmanreset)
        {
            gps.gpskalmanreset = false;
            enlem_f  = *gps.LatitudeAl();
			boylam_f = *gps.LongitudeAl();
            if(refAtama && enlem_f >= 1.0f && boylam_f >= 1.0f)
            {
            	refAtama=false;
            	enlem_ref  = enlem_f;
            	boylam_ref = boylam_f;
            }

            // GPS derece -> metre çevrimi (referansa göre delta)
            float x_gps = (boylam_f - boylam_ref) * cosf(enlem_ref*M_PI/180.0f) * EARTH_RADIUS * M_PI/180.0f;
            float y_gps = (enlem_f - enlem_ref) * EARTH_RADIUS * M_PI/180.0f;

            // Prediction: artık sadece o anki ölü hesaplama delta'sını kullan
            float X_pred[2] = { x_dead, y_dead };

            // Covariance update
            float P_pred[2][2] = {
                { P[0][0]+Q[0][0], P[0][1]+Q[0][1] },
                { P[1][0]+Q[1][0], P[1][1]+Q[1][1] }
            };

            // Kalman gain
            float K[2][2];
            K[0][0] = P_pred[0][0]/(P_pred[0][0]+R[0][0]);
            K[1][1] = P_pred[1][1]/(P_pred[1][1]+R[1][1]);
            K[0][1] = K[1][0] = 0;

            // Update
            X[0] = X_pred[0] + K[0][0]*(x_gps - X_pred[0]);
            X[1] = X_pred[1] + K[1][1]*(y_gps - X_pred[1]);

            // Covariance update
            P[0][0] = (1-K[0][0])*P_pred[0][0];
            P[1][1] = (1-K[1][1])*P_pred[1][1];
            P[0][1] = P[1][0] = 0;

            // Çıktıyı enlem/boylama çevir
            enlemKalmanCikti_f = enlem_ref + (X[1] / EARTH_RADIUS) * (180.0f / M_PI);
            boylamKalmanCikti_f = boylam_ref + (X[0] / (EARTH_RADIUS * cosf(enlem_ref*M_PI/180.0f))) * (180.0f / M_PI);

            // Yeni referans = Kalman çıktısı
            enlem_ref = enlemKalmanCikti_f;
            boylam_ref = boylamKalmanCikti_f;

            // Dead reckoning sıfırla
            x_dead = 0;
            y_dead = 0;
        }

        osDelayUntil(&prevTime, 1000);
    }
    /* USER CODE END StartKalmanTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
