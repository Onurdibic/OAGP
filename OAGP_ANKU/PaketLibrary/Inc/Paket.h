/*
 * Paket.h
 *
 *  Created on: Sep 27, 2025
 *      Author: T_rab
 */

#ifndef INC_PAKET_H_
#define INC_PAKET_H_


#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "string.h"

class Paket
{
public:
	Paket(UART_HandleTypeDef* huart);
    Paket(uint8_t baslik1_u8, uint8_t baslik2_u8, uint8_t paketTipi_u8, uint8_t dataBoyutu_u8);

    void GpsPaketOlustur(float latitude,float longitude,float altitude,float derece);
    void ImuPaketOlustur(float pitch,float roll,float yaw,float sicaklik);
    void SistemPaketOlustur(float derece,float batarya);
    void VersiyonPaketOlustur(uint8_t b,uint8_t o,uint8_t s);
    void YoklamaPaketOlustur();
    void RotaPaketOlustur();
    void gpsPaketCagir(uint8_t *kopyaDizi);
    void imuPaketCagir(uint8_t *kopyaDizi);
    void sistemPaketCagir(uint8_t *kopyaDizi);
    void versiyonPaketCagir(uint8_t *kopyaDizi);
    void yoklamaPaketCagir(uint8_t *kopyaDizi);
    void rotaPaketCagir(uint8_t *kopyaDizi);
    void PaketKesmeYapilandir();
    void ArayuzDataAlveBayrakKaldir();
    void PaketCoz();

    float *ArayuzLatAl();
    float *ArayuzLonAl();

    bool PaketCozBayrak=false;
    bool VersiyonPaketBayrak=false;
    bool YoklamaFlag=false;
    bool GidilecekNoktaBayrak=false;
    bool YoklamaPaketFlag=false;
    bool RotaGeldiBayrak=false;
    bool arabaDurBayrak=false;

private:
    UART_HandleTypeDef* huart;

    uint8_t CRC8Hesaplama(uint8_t *data, uint8_t baslangic, uint8_t bitis);
    void floatToBytes(float *Deger_f, uint8_t* bytes);
    float bytesToFloat(uint8_t* buffer_u8, int32_t startIndex_s32);
    uint8_t gpspaket[17];
    uint8_t imupaket[21];
    uint8_t sistempaket[13];
    uint8_t versiyonpaket[8];
    uint8_t yoklamapaket[8];
    uint8_t rotapaket[8];

    uint8_t ArayuzData;
    uint8_t ArayuzBuffer_u8[120];
    uint8_t ArayuzIndex_u8=0;

    uint8_t pitchBytes_u8[4];
    uint8_t rollBytes_u8[4];
    uint8_t headingBytes_u8[4];
    uint8_t sicaklikBytes_u8[4];
    uint8_t bataryaBytes_u8[4];
    uint8_t latBytes_u8[4];
    uint8_t lonBytes_u8[4];
    uint8_t altBytes_u8[4];
    uint8_t dereceBytes_u8[4];

    uint8_t baslik1_u8;
    uint8_t baslik2_u8;
    uint8_t paketTipi_u8;
    uint8_t dataBoyutu_u8;
    int16_t dataLength_s16 = 0;
    uint32_t startIndex_u32 = 0;
    uint32_t intBits_u32 = 0;

	float floatsonuc_f=0 ;

	float ArayuzEnlem_f = 0;
	float ArayuzBoylam_f = 0;
};




#endif /* INC_PAKET_H_ */
