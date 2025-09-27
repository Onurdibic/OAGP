/*
 * Mag.cpp
 *
 *  Created on: Sep 27, 2025
 *      Author: T_rab
 */

#include "Mag.h"

#define HMC5883L_ADDRESS 0x1E << 1 // I2C adresi

MAG::MAG(I2C_HandleTypeDef *hi2c)
{
  this->hi2c = hi2c;
  x_s16 = y_s16 = z_s16 = 0;
  heading_f = headingAcisi_f = 0.0f;
  xOffset_f = yOffset_f =0.0f;
}

void MAG::Yapilandir()
{
	//Configuration Register A
	uint8_t dataCRA[2] = {HMC5883_REGISTER_MAG_CRA_REG_M, 0x70}; // 8 ortalama, 15 Hz, normal Ã¶lÃ§Ã¼m
	HAL_I2C_Master_Transmit(hi2c, HMC5883L_ADDRESS, dataCRA, 2, 100);

	// Configuration Register B
	uint8_t dataCRB[2] = {HMC5883_REGISTER_MAG_CRB_REG_M, HMC5883_MAGGAIN_1_3}; // KazanÃ§ = +/- 1.3
	HAL_I2C_Master_Transmit(hi2c, HMC5883L_ADDRESS, dataCRB, 2, 100);

	// Mode Register
	uint8_t dataMR[2] = {HMC5883_REGISTER_MAG_MR_REG_M, 0x00}; // SÃ¼rekli Ã¶lÃ§Ã¼m modu
	HAL_I2C_Master_Transmit(hi2c, HMC5883L_ADDRESS, dataMR, 2, 100);
}

void MAG::MagDataOku(int16_t *x_s16, int16_t *y_s16, int16_t *z_s16)
{
  uint8_t buffer[6];
  HAL_I2C_Mem_Read(hi2c, HMC5883L_ADDRESS, HMC5883_REGISTER_MAG_OUT_X_H_M, 1, buffer, 6, 100);

  *x_s16 = (int16_t)((buffer[0] << 8) | buffer[1]);
  *z_s16 = (int16_t)((buffer[2] << 8) | buffer[3]);
  *y_s16 = (int16_t)((buffer[4] << 8) | buffer[5]);
}
void MAG::XveYKalibreEt()
{

    int16_t xEksen_s16, yEksen_s16,zEksen_s16;
    int16_t xMin = 3200, yMin = 3200;
    int16_t xMax = -3200, yMax = -3200;

    for (int i = 0; i < 2000; i++)
    {
    	uint32_t startTick = HAL_GetTick();
        MagDataOku(&xEksen_s16, &yEksen_s16, &zEksen_s16);

        if (xEksen_s16 < xMin) xMin = xEksen_s16;
        if (xEksen_s16 > xMax) xMax = xEksen_s16;
        if (yEksen_s16 < yMin) yMin = yEksen_s16;
        if (yEksen_s16 > yMax) yMax = yEksen_s16;
        while ((HAL_GetTick() - startTick) < 5) {}
    }

    xOffset_f = (xMax + xMin) / 2;
    yOffset_f = (yMax + yMin) / 2;
}
/*
void MAG::ZKalibreEt()
{
	 int16_t xEksen_s16, yEksen_s16,zEksen_s16;
    int16_t zMin = 3200;
    int16_t zMax=-3200;


    for (int i = 0; i < 1000; i++)
    {
        MagDataOku(&xEksen_s16, &yEksen_s16, &zEksen_s16);

        if (zEksen_s16 < zMin) zMin = zEksen_s16;
        if (zEksen_s16 > zMax) zMax = zEksen_s16;
        HAL_Delay(10);
    }

    zOffset_f = (zMax + zMin) / 2;
}*/
float* MAG::HeadingOlustur(float pitch, float roll)
{
    float kalibreliX_f, kalibreliY_f, kalibreliZ_f;
    float phi, theta;

    // Manyetometre verilerini oku
    MagDataOku(&x_s16, &y_s16, &z_s16);

    // Kalibrasyon offsetlerini Ã§Ä±kar
    kalibreliX_f = x_s16 - xOffset_f;
    kalibreliY_f = y_s16 - yOffset_f;
    kalibreliZ_f = z_s16;

    // Derece cinsinden olan pitch ve roll aÃ§Ä±larÄ±nÄ± radyana Ã§evir
    phi = pitch * (M_PI / 180.0f);   // pitch = X ekseni etrafÄ±nda dÃ¶nme
    theta = roll * (M_PI / 180.0f);  // roll = Y ekseni etrafÄ±nda dÃ¶nme

    // Tilt kompanzasyon formÃ¼lleri
    float Xh = kalibreliX_f * cos(phi) + kalibreliZ_f * sin(phi);
    float Yh = kalibreliX_f * sin(theta) * sin(phi) + kalibreliY_f * cos(theta) - kalibreliZ_f * sin(theta) * cos(phi);

    // BaÅŸlÄ±k (heading) aÃ§Ä±sÄ± hesapla
    heading_f = atan2(Yh, Xh);

    if (heading_f < 0)
        heading_f += 2 * M_PI;
    if (heading_f > 2 * M_PI)
        heading_f -= 2 * M_PI;

    headingAcisi_f = heading_f * (180.0f / M_PI);  // derece cinsinden

    return &headingAcisi_f;
}




