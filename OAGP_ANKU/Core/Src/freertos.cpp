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
#include "Kontrol.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern IMU imu;
extern MAG mag;
extern GPS gps;
extern KontrolLibrary kontrol;
Paket GpsPaket(0x12, 0x34, 0x01, 0x0D); //veri boyutu 13
Paket ImuPaket(0x12, 0x34, 0x02, 0x0D);//veri boyutu 13
Paket VersiyonPaket(0x12, 0x34, 0x03, 0x04); //veri boyutu 4
Paket YoklamaPaket(0x12, 0x34, 0x04, 0x04);//veri boyutu 4
Paket RotaPaket(0x12, 0x34, 0x05, 0x04);//veri boyutu 4
Paket SistemPaket(0x12, 0x34, 0x06, 0x09);//veri boyutu 9
Paket KomutPaket(0x12,0x34, 0x07, 0x0D); //veri boyutu 13
Paket KalmanPaket(0x12, 0x34, 0x08, 0x0D); //veri boyutu 13
Paket RPMPaket(0x12,0x34, 0x09, 0x09); //veri boyutu 9

extern Paket ArayuzPaket;
extern Paket ArabaArkaPaket;
extern Paket ArabaOnPaket;
extern Paket JetsonPaket;
extern uint8_t txState;
extern uint32_t lastJetsonTime;

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
//
//float konumX = 0.0f;
//float konumY =0.0f;
//float hizX = 0.0f;
//float hizY =0.0f;
//float ivmeX = 0.0f;
//float ivmeY =0.0f;

float pitch_f;
float roll_f;
float yaw_f;
float heading_f;
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
uint8_t KalmanVeriPaket[17]={0};
uint8_t RPMVeriPaket[13]={0};
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
osThreadId myDeadRecTaskHandle;
osThreadId myKalmanTaskHandle;
osThreadId myKontrolTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartPaketTask(void const * argument);
void StartImuTask(void const * argument);
void StartDeadReckoningTask(void const * argument);
void StartKalmanKonumTask(void const * argument);
void StartKontrolTask(void const * argument);

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

	/* definition and creation of myDeadRecTask */
	osThreadDef(myDeadRecTask, StartDeadReckoningTask, osPriorityIdle, 0, 1024);
	myDeadRecTaskHandle = osThreadCreate(osThread(myDeadRecTask), NULL);

	/* definition and creation of myKalmanTask */
	osThreadDef(myKalmanTask, StartKalmanKonumTask, osPriorityHigh, 0, 1024);
	myKalmanTaskHandle = osThreadCreate(osThread(myKalmanTask), NULL);

	/* definition and creation of myKontrolTask */
	  osThreadDef(myKontrolTask, StartKontrolTask, osPriorityIdle, 0, 1024);
	  myKontrolTaskHandle = osThreadCreate(osThread(myKontrolTask), NULL);

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

    int sureCounter=0;

    for(;;)
    {
    	if(txState == 0)
    	{
    		YoklamaPaket.YoklamaPaketOlustur(JetsonPaket.durum_u8);
			YoklamaPaket.txPaketCagir(YoklamaVeriPaket);
			HAL_UART_Transmit_DMA(&huart3, YoklamaVeriPaket, sizeof(YoklamaVeriPaket));
			txState = 1;
    	}
        if(ArayuzPaket.YoklamaPaketFlag==0)
        {
        	txState = 0;
        }
        sureCounter++;
        if(ArayuzPaket.YoklamaFlag==true)
        {
        	yoklamaCounter++;
        	ArayuzPaket.YoklamaFlag=false;
        	GPIOD->ODR ^= GPIO_PIN_13;
        }
        if(sureCounter==5)
        {
        	sureCounter=0;
        	 if(yoklamaCounter==0)
			{
//				ArayuzPaket.ileriDurBayrak=true;
//				ArayuzPaket.GidilecekNoktaBayrak=false;

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

// Tek bir fonksiyon ile motorlara RPM gönderimi
void MotorBilgiGonder(float yon, float rpmSag, float rpmSol)
{
    KomutPaket.KomutPaketOlustur(yon, rpmSag, rpmSol);
    KomutPaket.txPaketCagir(KomutVeriPaket);
    HAL_UART_Transmit_DMA(&huart4, KomutVeriPaket, sizeof(KomutVeriPaket));
    HAL_UART_Transmit_DMA(&huart5, KomutVeriPaket, sizeof(KomutVeriPaket));
}

// Motorların durup durmadığını kontrol eder
bool CheckIfStopped(void)
{
    return (ArabaArkaPaket.saghiz_f || ArabaArkaPaket.solhiz_f || ArabaOnPaket.saghiz_f || ArabaOnPaket.solhiz_f);
}

void StartPaketTask(void const * argument)
{
  /* USER CODE BEGIN StartPaketTask */
  /* Infinite loop */
	uint32_t prevTime = xTaskGetTickCount();

	ArayuzPaket.PaketKesmeYapilandir();
	ArabaArkaPaket.PaketKesmeYapilandir();
	ArabaOnPaket.PaketKesmeYapilandir();
	JetsonPaket.PaketKesmeYapilandir();

	for(;;)
	{
		ArabaArkaPaket.PaketCoz();
		ArabaOnPaket.PaketCoz();
		ArayuzPaket.PaketCoz();
		JetsonPaket.PaketCoz();
		// -------------------- Jetson engel kontrolü --------------------
		if ((HAL_GetTick() - lastJetsonTime) > 500)
		{
			JetsonPaket.sagrpm_f = 0.0f;
			JetsonPaket.solrpm_f = 0.0f;
			JetsonPaket.durum_u8 = 0;
//			ArayuzPaket.ileriDurBayrak=true;
		}

		// -------------------- Arayüz komutları --------------------
		if(ArayuzPaket.ileriGitBayrak)       { MotorBilgiGonder(1, 120, 120); ArayuzPaket.ileriGitBayrak = false; }
		if(ArayuzPaket.geriGitBayrak)        { MotorBilgiGonder(1, -120, -120); ArayuzPaket.geriGitBayrak = false; }
		if(ArayuzPaket.sagaGitBayrak)        { MotorBilgiGonder(1, -40, 120);   ArayuzPaket.sagaGitBayrak = false; }
		if(ArayuzPaket.solaGitBayrak)        { MotorBilgiGonder(1, 120, -40);   ArayuzPaket.solaGitBayrak = false; }
		if(ArayuzPaket.ileriDurBayrak)       { MotorBilgiGonder(1, 0, 0); ArayuzPaket.ileriDurBayrak = CheckIfStopped(); }
		if(ArayuzPaket.geriDurBayrak)        { MotorBilgiGonder(2, 0, 0); ArayuzPaket.geriDurBayrak = CheckIfStopped(); }
		if(ArayuzPaket.kalibrasyonIMUBayrak) { imu.kalibreEt(); ArayuzPaket.kalibrasyonIMUBayrak = false; }
		if(ArayuzPaket.kalibrasyonMAGBayrak) { mag.XveYKalibreEt(); ArayuzPaket.kalibrasyonMAGBayrak = false; }

		// -------------------- Rota gönder --------------------
		if(!ArayuzPaket.GidilecekNoktaBayrak && ArayuzPaket.RotaGeldiBayrak)
		{
			RotaPaket.RotaPaketOlustur();
			RotaPaket.txPaketCagir(RotaVeriPaket);
			HAL_UART_Transmit(&huart3, RotaVeriPaket, sizeof(RotaVeriPaket), 1000);
			ArayuzPaket.RotaGeldiBayrak = false;
		}

		osDelayUntil(&prevTime, 25);
	}
  /* USER CODE END StartPaketTask */
}

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
		pitch_f = *imu.PitchAl();
		roll_f = *imu.RollAl();
		yaw_f = *imu.YawAl();
		imucipsicaklik = *imu.SicaklikAl();
		heading_f = *mag.HeadingOlustur(pitch_f,roll_f);

		//imu.AccToKonum(dt);

		osDelayUntil(&prevTime, 20);
	}
  /* USER CODE END StartImuTask */
}

/* ===================== KONTROL GÖREVİ ===================== */
void StartKontrolTask(void const * argument)
{
    uint32_t prevTime = xTaskGetTickCount();

    for (;;)
    {
        bool konumGecerli = (enlemKalmanCikti_f >= 1.0f && boylamKalmanCikti_f >= 1.0f);
        bool hedeflikDurum = ArayuzPaket.GidilecekNoktaBayrak; // Hedef var mı kontrolü
        uint8_t durum = JetsonPaket.durum_u8;

        float rpmSagFinal = 0.0f;
        float rpmSolFinal = 0.0f;

        // =================================================================
        // GENEL KONTROL: Yalnızca gidilecek bir hedef varsa algoritmaları çalıştır
        // =================================================================
        if (hedeflikDurum)
        {
            /* =================================================================
             * A) DEPTH (ENGELDEN KAÇINMA) ALGORİTMASI AKTİF (Durum: 1 veya 2)
             * =================================================================
             */

            // --- Durum 1: Engel Yok -> Kontrol STM32 GPS Algoritmasında ---
            if (konumGecerli && durum == 1)
            {
                kontrol.SetHeading(heading_f);
                kontrol.SetKonum(enlemKalmanCikti_f, boylamKalmanCikti_f);
                kontrol.SetHedef(ArayuzPaket.ArayuzEnlem_f, ArayuzPaket.ArayuzBoylam_f);

                kontrol.Hesapla();

                rpmSagFinal = kontrol.rpmSag;
                rpmSolFinal = kontrol.rpmSol;

                if (kontrol.hedefMesafe < 1.0f)
                {
                    ArayuzPaket.GidilecekNoktaBayrak = false;
                }
            }
            // --- Durum 2: Engel Var -> Kaçış RPM'leri Doğrudan Jetson'dan Alınır ---
            else if (konumGecerli && durum == 2)
            {
                rpmSagFinal = JetsonPaket.sagrpm_f;
                rpmSolFinal = JetsonPaket.solrpm_f;
            }

            /* =================================================================
             * B) SÜRÜŞ ALGORİTMASI AKTİF (Durum: 3, 4 veya 5)
             * =================================================================
             */

            // --- Durum 3 veya 4: Şerit İhlali veya Sürülebilir Alan Var ---
            else if (konumGecerli && (durum == 3 || durum == 4))
            {
                rpmSagFinal = JetsonPaket.sagrpm_f;
                rpmSolFinal = JetsonPaket.solrpm_f;
            }

            // --- Durum 5: Sürülebilir Alan Yok (STOP) ---
            else if (durum == 5)
            {
                rpmSagFinal = 0.0f;
                rpmSolFinal = 0.0f;
            }

            /* =================================================================
             * C) GÜVENLİK VE İSTİSNAİ DURUMLAR (Fail-Safe)
             * =================================================================
             */

            // --- Durum 0: Jetson İletişimi Koptu (500ms Zaman Aşımı) ---
            else if (durum == 0)
            {
                rpmSagFinal = 0.0f;
                rpmSolFinal = 0.0f;
            }

            // --- Konum Geçersizse (Diğer üstteki durumlara girmediyse güvenlik freni) ---
            else if (!konumGecerli)
            {
                rpmSagFinal = 0.0f;
                rpmSolFinal = 0.0f;
            }

            /* === Komut Gönderimi (Hedef Varken Hesaplanan Değerler) === */
            MotorBilgiGonder(1, rpmSagFinal, rpmSolFinal);
        }

        // Periyot süresini bozmamak için task gecikmesi en dışta kalmalı
        osDelayUntil(&prevTime, 30);
    }
}

/* USER CODE END Header_StartKonumTask */
void StartDeadReckoningTask(void const * argument)
{
  /* USER CODE BEGIN StartKonumTask */
  uint32_t prevTime = xTaskGetTickCount();
  float dt_f = 0.05f;

  for (;;)
  {
	  // Kalman filtresinden anlamlı konum çıktısı geldiğinde
	  if(enlemKalmanCikti_f >= 1.0f && boylamKalmanCikti_f >= 1.0f)
	  {
		  // GPS ve dead reckoning tabanlı konum güncellemesi
		  gps.YeniKonumHesapla(
			  enlemKalmanCikti_f,       // mevcut enlem
			  boylamKalmanCikti_f,      // mevcut boylam
			  heading_f,                  // mevcut yönelim
			  ArabaOnPaket.saghiz_f,  // sağ tekerlek hızı
			  ArabaOnPaket.solhiz_f,  // sol tekerlek hızı
			  dt_f,                     // zaman adımı
			  &gps.gpsdeadreset,        // dead reckoning reset flag
			  &enlemCikti_f,            // çıktı enlem
			  &boylamCikti_f            // çıktı boylam
		  );
	  }

	  // Ortalama tekerlek hızı üzerinden hareket mesafesi
	  float ortalamaHiz = (ArabaArkaPaket.saghiz_f + ArabaArkaPaket.solhiz_f) / 2.0f;
	  float katedilenMesafe = ortalamaHiz * dt_f;

	  // Heading açısını radyana çevir
	  float headingRad = heading_f * (M_PI / 180.0f);

	  // Dead reckoning ile X (doğu) ve Y (kuzey) pozisyon güncellemesi
	  if(enlemKalmanCikti_f >= 1.0f && boylamKalmanCikti_f >= 1.0f)
	  {
		  x_dead += katedilenMesafe * sinf(headingRad); // doğu yönü
		  y_dead += katedilenMesafe * cosf(headingRad); // kuzey yönü
	  }

	  // Kontrol döngüsü periyodu
	  osDelayUntil(&prevTime, 50);
  }
  /* USER CODE END StartKonumTask */
}

void StartKalmanKonumTask(void const * argument)
{
    /* USER CODE BEGIN StartKalmanTask */
    bool refAtama=true;
    float X[2] = {0.0f, 0.0f}; // x, y (sadece delta)
    float P[2][2] = {{1.0f,0},{0,1.0f}}; // covariance
    float Q[2][2] = {{0.4f,0},{0,0.4f}};     // process noise (tekerlek hatası)
    float R[2][2] = {{9.0f,0},{0,7.0f}};    // measurement noise (GPS ~5m hatası)

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
