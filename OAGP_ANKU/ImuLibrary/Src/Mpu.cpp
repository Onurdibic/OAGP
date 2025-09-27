/*
 * Mpu.cpp
 *
 *  Created on: Sep 27, 2025
 *      Author: T_rab
 */
#include "Mpu.h"

#define PWR_MGMT_1_REG 0X6B
#define SMPLRT_DIV_REG 0X19
#define GYRO_CNFG_REG 0X1B
#define ACC_CNFG_REG 0X1C
#define MPU6500_ADDRESS 0x68<< 1
#define WHO_AM_I 0x75
#define RESET_BIT 0x80
#define EARTH_RADIUS 6378137.0 // DÃ¼nya yarÄ±Ã§apÄ± (metre)

IMU::IMU(I2C_HandleTypeDef *hi2c)
    : hi2c(hi2c),
//	  lpfax(10.0,20.0),
//	  lpfay(10.0,20.0),
//	  kalmanax(0.05, 0.5, 10),
//	  kalmanay(0.05, 0.5, 10),
//	  kalmangx(0.05, 0.5, 10),
//	  kalmangy(0.05, 0.5, 10),
	  hizX(0.0f), hizY(0.0f),
	  enlem_f(0.0f), boylam_f(0.0f){}


void IMU::Yapilandir()
{
	uint8_t kontrol_u8;

	HAL_I2C_Mem_Read(hi2c, MPU6500_ADDRESS , WHO_AM_I, 1, &kontrol_u8, 1, 1000);
	if (kontrol_u8 == 0x70)
	{
		data_u8 = 0x00;
		HAL_I2C_Mem_Write(hi2c, MPU6500_ADDRESS , PWR_MGMT_1_REG, 1, &data_u8, 1, 1000);
		// SMPLRT_DIV register
		data_u8 = 0x07;
		HAL_I2C_Mem_Write(hi2c, MPU6500_ADDRESS ,SMPLRT_DIV_REG, 1, &data_u8, 1, 1000);
		//  ACCEL_CONFIG Register Â±2g(00),Â±4g(01),Â±8g(10),Â±16g(11) 4:3
		data_u8 = 0x00;
		HAL_I2C_Mem_Write(hi2c, MPU6500_ADDRESS, ACC_CNFG_REG, 1, &data_u8, 1, 1000);
		//  GYRO_CONFIG Register +250dps(00),+500dps(01),+1000dps(10),+2000dps(11) 4:3
		data_u8 = 0x00;
		HAL_I2C_Mem_Write(hi2c, MPU6500_ADDRESS , GYRO_CNFG_REG, 1, &data_u8, 1, 1000);
	}
}

void IMU::accOku()
{
	uint8_t accBuffer[6];

	accBuffer[0] = 0x3B;
	HAL_I2C_Master_Transmit(hi2c, MPU6500_ADDRESS, accBuffer, 1, 10);
	HAL_I2C_Master_Receive(hi2c, MPU6500_ADDRESS, accBuffer, 6, 10);
	accEksen[0] = (accBuffer[0] << 8 | accBuffer[1]);
	accEksen[1] = (accBuffer[2] << 8 | accBuffer[3]);
	accEksen[2] = (accBuffer[4] << 8 | accBuffer[5]);
}

void IMU::sicaklikOku()
{
	uint8_t sicaklikBuffer[2];

	sicaklikBuffer[0] = 0x41;
	HAL_I2C_Master_Transmit(hi2c, MPU6500_ADDRESS, sicaklikBuffer, 1, 10);
	HAL_I2C_Master_Receive(hi2c, MPU6500_ADDRESS, sicaklikBuffer, 2, 10);
	hamSicaklik_u16 = (sicaklikBuffer[0] << 8 | sicaklikBuffer[1]);
	Sicaklik_f=((float)((float)hamSicaklik_u16 / 333.87)) + 21;
}

void IMU::gyroOku()
{
	uint8_t gyroBuffer[6];

	gyroBuffer[0] = 0x43;
	HAL_I2C_Master_Transmit(hi2c, MPU6500_ADDRESS, gyroBuffer, 1, 10);
	HAL_I2C_Master_Receive(hi2c, MPU6500_ADDRESS, gyroBuffer, 6, 10);
	gyroEksen[0] = (gyroBuffer[0] << 8 | gyroBuffer[1]);
	gyroEksen[1] = (gyroBuffer[2] << 8 | gyroBuffer[3]);
	gyroEksen[2] = (gyroBuffer[4] << 8 | gyroBuffer[5]);

}

void IMU::kalibreEt()
{
    for (int i = 0; i < 1000; i++)
    {
        GPIOD->ODR ^= GPIO_PIN_12;

        uint32_t startTick = HAL_GetTick();
        gyroOku();
        gyroHesap[0] += gyroEksen[0];
        gyroHesap[1] += gyroEksen[1];
        gyroHesap[2] += gyroEksen[2];

        accOku();
        accHesap[0] += accEksen[0];
        accHesap[1] += accEksen[1];

        while ((HAL_GetTick() - startTick) < 5) {}
    }

    gyroHesap[0] /= 1000;
    gyroHesap[1] /= 1000;
    gyroHesap[2] /= 1000;

    accHesap[0] /= 1000;
    accHesap[1] /= 1000;
}


void IMU::gercekDataOku()
{
    accOku();
    sicaklikOku();
    gyroOku();
    gyroEksen[0] -= gyroHesap[0];
    gyroEksen[1] -= gyroHesap[1];
    gyroEksen[2] -= gyroHesap[2];
    accEksen[0] -= accHesap[0];
    accEksen[1] -= accHesap[1];
    accEksen[2] -= 100;

    //accEksen[0]=kalmanax.veriGuncelle(accEksen[0]);
    //accEksen[1]=kalmanay.veriGuncelle(accEksen[1]);
    //gyroEksen[0]=kalmangx.veriGuncelle(gyroEksen[0]);
    //gyroEksen[1]=kalmangy.veriGuncelle(gyroEksen[1]);
    //accEksen[0]=lpfax.uygula(accEksen[0]);
    //accEksen[1]=lpfay.uygula(accEksen[1]);

}
/*
void IMU::DBC_ACI_BULMA()
{
	DBC_DATA_OKU();
	//Ham Veri Iyilestirmeleri
	gyroPitchAci_f += gyroEksen[0] * 0.000155;
	gyroRollAci_f += gyroEksen[1] * 0.000155;
	gyroYawAci_f += gyroEksen[2] * 0.000155;

	accToplamVektor_s16 = sqrt((accEksen[0]*accEksen[0])+(accEksen[1]*accEksen[1])+(accEksen[2]*accEksen[2]));

	//57.3 =1 /(3.142 /180)
	accPitchAci_f = asin((float)accEksen[1]/accToplamVektor_s16)*57.302;
	accRollAci_f = asin((float)accEksen[0]/accToplamVektor_s16)*-57.302;

	pitchAcisi_f = gyroPitchAci_f * 0.8 + accPitchAci_f * 0.2;
	rollAci_f = gyroRollAci_f * 0.8 + accRollAci_f * 0.2;

    AccToKonum(accEksen[0], accEksen[1], accEksen[2]);
}
*/
void IMU::aciBul()
{
	gercekDataOku();
	float gx = gyroEksen[0] / 131.0f * (M_PI / 180.0f); // jiroskop datasÄ± degeri dps -> rad/s (MPU6500 iÃ§in 131 LSB/dps @ Â±250 dps)
	float gy = gyroEksen[1] / 131.0f * (M_PI / 180.0f);
	float gz = gyroEksen[2] / 131.0f * (M_PI / 180.0f);

	float ax = accEksen[0] / 16384.0f;  // Ä°vmeÃ¶lÃ§er deÄŸerini g birimine dÃ¶nÃ¼ÅŸtÃ¼r (Â±2g)
	float ay = accEksen[1] / 16384.0f;
	float az = accEksen[2] / 16384.0f;

	float dt = 0.005f;
	madgwick.updateIMU(gx, gy, gz, ax, ay, az, dt);
	madgwick.getEuler(rollAci_f, pitchAcisi_f, gyroYawAci_f);

    AccToKonum(accEksen[0], accEksen[1], accEksen[2]);
}


void IMU::AccToKonum(float accX, float accY, float accZ)
{
    // 1. Madgwick quaternion bileÅŸenlerini al
    float q0 = madgwick.q0;
    float q1 = madgwick.q1;
    float q2 = madgwick.q2;
    float q3 = madgwick.q3;

    // 2. YerÃ§ekimi vektÃ¶rÃ¼nÃ¼ hesapla (referans: Madgwick white paper)
    float gX = 2.0f * (q1 * q3 - q0 * q2);
    float gY = 2.0f * (q0 * q1 + q2 * q3);
    float gZ = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    // 3. Ä°vme verilerini g â†’ m/sÂ²'ye Ã§evir (Â±2g: 16384 LSB/g)
    float accX_ms2 = (accX / 16384.0f) * 9.8066f;
    float accY_ms2 = (accY / 16384.0f) * 9.8066f;
    float accZ_ms2 = (accZ / 16384.0f) * 9.8066f;

    // 4. YerÃ§ekimi bileÅŸenlerini Ã§Ä±kararak net ivmeyi elde et
    ivmeX = accX_ms2 - gX * 9.8066f;
    ivmeY = accY_ms2 - gY * 9.8066f;
    // ivmeZ istersen: ivmeZ = accZ_ms2 - gZ * 9.8066f;

    // 5. HÄ±z ve konum hesabÄ±
    hizX += ivmeX * 0.005f;
    hizY += ivmeY * 0.005f;

    konumX += hizX * 0.005f;
    konumY += hizY * 0.005f;
}

/*
void IMU::AccToKonum(float accX, float accY, float accZ)
{
    // YerÃ§ekimi bileÅŸenleri (aÃ§Ä±ya gÃ¶re)
    float gY = sin(pitchAcisi_f * M_PI / 180.0f) * 9.8066f;
    float gX = -sin(rollAci_f * M_PI / 180.0f) * cos(pitchAcisi_f * M_PI / 180.0f) * 9.8066f;

    // Ä°vmeÃ¶lÃ§er deÄŸerlerini m/sÂ² cinsine Ã§evir
    ivmeX = (accX / 16384.0f) * 9.8066f;
    ivmeY = (accY / 16384.0f) * 9.8066f;

    // YerÃ§ekimi bileÅŸenlerini Ã§Ä±kar
    ivmeX -= gX;
    ivmeY -= gY;

    // HÄ±z ve konum hesabÄ±
    hizX += ivmeX * 0.02f;
    hizY += ivmeY * 0.02f;

    konumX += hizX * 0.02f;
    konumY += hizY * 0.02f;
}
*/
void IMU::EnlemBoylamGuncelle(float heading , float enlem_f,float boylam_f)
{
    // Heading radyan cinsinden
    float headingRad = heading * (M_PI / 180.0);
    //C++ dilinde kullanÄ±lan sin, cos gibi aÃ§Ä± deÄŸerlerini radyan cinsinden alÄ±r.
    float dy = konumY * cos(headingRad) - konumX * sin(headingRad);
    float dx = konumY * sin(headingRad) + konumX * cos(headingRad);

   	/*[cos(x)  sin(x)]*[konumX]= |dx|
      [-sin(x) cos(x)]*[konumY]= |dy|*/
    //Saat yÃ¶nÃ¼nÃ¼n tersine dÃ¶ndÃ¼rme matrisi araÃ§ ile dÃ¼nya ekseni birleÅŸmeli

    float deltaLat = (dy / EARTH_RADIUS) * (180.0 / M_PI);
    //Enlem deÄŸiÅŸikliÄŸi, DÃ¼nya yÃ¼zeyinde Y eksenindeki hareketimize baÄŸlÄ±dÄ±r. dÃ¼nya Ã§izgileri boyu eÅŸittir.
    //Boylam deÄŸiÅŸiliÄŸi, Daha yÃ¼ksek enlemlerde, DÃ¼nya'nÄ±n Ã§evresi kÃ¼Ã§Ã¼lÃ¼r, bu yÃ¼zden kosinÃ¼s fonksiyonu ile Ã¶lÃ§eklendirilir.
    float deltaLon = (dx / (EARTH_RADIUS * cos(M_PI * enlem_f / 180.0))) * (180.0 / M_PI);
    this->enlem_f = enlem_f + deltaLat;
    this->boylam_f = boylam_f + deltaLon;

}
float* IMU::PitchAl(){ return &pitchAcisi_f;}
float* IMU::RollAl(){return &rollAci_f;}
float* IMU::YawAl(){return &gyroYawAci_f;}
float* IMU::SicaklikAl(){return &Sicaklik_f;}
float* IMU::LatAl(){return &enlem_f;}
float* IMU::LongAl(){return &boylam_f;}




