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
  xOffset_f = yOffset_f = 0.0f;
  xScaleFactor_f = yScaleFactor_f = 1.0f;
}

void MAG::Yapilandir()
{
  // Configuration Register A
  uint8_t dataCRA[2] = {HMC5883_REGISTER_MAG_CRA_REG_M, 0x78}; // 8 ortalama, 15 Hz, normal ölçüm
  HAL_I2C_Master_Transmit(hi2c, HMC5883L_ADDRESS, dataCRA, 2, 100);

  // Configuration Register B
  uint8_t dataCRB[2] = {HMC5883_REGISTER_MAG_CRB_REG_M, HMC5883_MAGGAIN_1_3}; // Kazanç = +/- 1.3
  HAL_I2C_Master_Transmit(hi2c, HMC5883L_ADDRESS, dataCRB, 2, 100);

  // Mode Register
  uint8_t dataMR[2] = {HMC5883_REGISTER_MAG_MR_REG_M, 0x00}; // Sürekli ölçüm modu
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
  int16_t xEksen_s16, yEksen_s16, zEksen_s16;
  int16_t xMin = 3200, yMin = 3200;
  int16_t xMax = -3200, yMax = -3200;

  // 2000 örnek alarak min/max bul
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

  // Offset (merkez) hesapla
  xOffset_f = (xMax + xMin) / 2.0f;
  yOffset_f = (yMax + yMin) / 2.0f;

  // Ölçek (yarıçap) hesapla
  float xScale = (xMax - xMin) / 2.0f;
  float yScale = (yMax - yMin) / 2.0f;

  // Ortalama ölçek değeri
  float avgScale = (xScale + yScale) / 2.0f;

  // Her eksen için ölçek faktörü
  xScaleFactor_f = avgScale / xScale;
  yScaleFactor_f = avgScale / yScale;
}

void MAG::TumEkseniKalibreEt()
{
    int16_t xEksen_s16, yEksen_s16, zEksen_s16;
    int16_t xMin = 32000, yMin = 32000, zMin = 32000;
    int16_t xMax = -32000, yMax = -32000, zMax = -32000;

    // 2000 örnek alarak min/max bul
    for (int i = 0; i < 1500; i++)
    {
        uint32_t startTick = HAL_GetTick();
        MagDataOku(&xEksen_s16, &yEksen_s16, &zEksen_s16);

        if (xEksen_s16 < xMin) xMin = xEksen_s16;
        if (xEksen_s16 > xMax) xMax = xEksen_s16;

        if (yEksen_s16 < yMin) yMin = yEksen_s16;
        if (yEksen_s16 > yMax) yMax = yEksen_s16;

        if (zEksen_s16 < zMin) zMin = zEksen_s16;
        if (zEksen_s16 > zMax) zMax = zEksen_s16;

        while ((HAL_GetTick() - startTick) < 10) {}  // 5 ms bekleme
    }

    // Offset (hard-iron düzeltmesi)
    xOffset_f = (xMax + xMin) / 2.0f;
    yOffset_f = (yMax + yMin) / 2.0f;
    zOffset_f = (zMax + zMin) / 2.0f;

    // Ölçek (soft-iron düzeltmesi)
    float xScale = (xMax - xMin) / 2.0f;
    float yScale = (yMax - yMin) / 2.0f;
    float zScale = (zMax - zMin) / 2.0f;

    // Ortalama ölçek
    float avgScale = (xScale + yScale + zScale) / 3.0f;

    // Her eksen için ölçek faktörleri
    xScaleFactor_f = avgScale / xScale;
    yScaleFactor_f = avgScale / yScale;
    zScaleFactor_f = avgScale / zScale;

}


float* MAG::HeadingOlustur(float pitch, float roll)
{

  float phi, theta;

  // Manyetometre verilerini oku
  MagDataOku(&x_s16, &y_s16, &z_s16);

  // Kalibrasyon offset ve ölçek düzeltmeleri uygula
  kalibreliX_f = (x_s16 - xOffset_f) * xScaleFactor_f;
  kalibreliY_f = (y_s16 - yOffset_f) * yScaleFactor_f;
  kalibreliZ_f = z_s16;
//  kalibreliZ_f = (z_s16 - zOffset_f) * zScaleFactor_f;
  // Pitch ve Roll açılarını radyana çevir
  phi = pitch * (PI / 180.0f);   // pitch = X ekseni etrafında dönme
  theta = roll * (PI / 180.0f);  // roll = Y ekseni etrafında dönme

  // Tilt kompanzasyon formülleri
  Xh = kalibreliX_f * cos(phi) + kalibreliZ_f * sin(phi);
  Yh = kalibreliX_f * sin(theta) * sin(phi) + kalibreliY_f * cos(theta) + kalibreliZ_f * sin(theta) * cos(phi);

  // Başlık (heading) açısı hesapla
  heading_f = atan2(Yh, Xh);

  // Manyetik sapma düzeltmesi (İstanbul ~ +6°)
  heading_f += 6.03f * (PI / 180.0f);

  if (heading_f < 0)
      heading_f += 2 * PI;
  if (heading_f > 2 * PI)
      heading_f -= 2 * PI;

  headingAcisi_f = heading_f * (180.0f / PI);  // derece cinsinden


  return &headingAcisi_f;
}
