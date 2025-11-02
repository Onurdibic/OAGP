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
#include <string.h>

// -----------------------------------------------------------
//  PAKET SINIFI
//  STM32 UART-DMA + RING BUFFER TABANLI PAKET İLETİŞİMİ
// -----------------------------------------------------------
class Paket
{
public:
    // -------------------- KURUCULAR --------------------
    Paket(UART_HandleTypeDef* huart);
    Paket(uint8_t baslik1_u8, uint8_t baslik2_u8,
          uint8_t paketTipi_u8, uint8_t dataBoyutu_u8);

    // -------------------- PAKET OLUŞTURMA --------------------
    void GpsPaketOlustur(float latitude, float longitude, float altitude, float derece);
    void ImuPaketOlustur(float pitch, float roll, float yaw, float sicaklik);
    void VersiyonPaketOlustur(uint8_t b, uint8_t o, uint8_t s);
    void YoklamaPaketOlustur();
    void RotaPaketOlustur();
    void SistemPaketOlustur(float derece, float batarya);
    void KomutPaketOlustur(float yon, float rpmSag, float rpmSol);

    // -------------------- PAKET KOPYALAMA --------------------
    void gpsPaketCagir(uint8_t *kopyaDizi);
    void imuPaketCagir(uint8_t *kopyaDizi);
    void sistemPaketCagir(uint8_t *kopyaDizi);
    void versiyonPaketCagir(uint8_t *kopyaDizi);
    void yoklamaPaketCagir(uint8_t *kopyaDizi);
    void rotaPaketCagir(uint8_t *kopyaDizi);
    void komutPaketCagir(uint8_t *kopyaDizi);

    // -------------------- PAKET ALIMI & ÇÖZÜMLEME --------------------
    void PaketKesmeYapilandir();
    void DataAlveBayrakKaldir();   // UART DMA callback’inde çağrılır
    void PaketCoz();               // Ring buffer içeriğini çözer

    // -------------------- ARAYÜZ VERİLERİ --------------------
    float* ArayuzLatAl();
    float* ArayuzLonAl();

    // -------------------- DURUM BAYRAKLARI --------------------
    bool PaketCozBayrak        = false;
    bool VersiyonPaketBayrak   = false;
    bool YoklamaFlag           = false;
    bool GidilecekNoktaBayrak  = false;
    bool YoklamaPaketFlag      = false;
    bool RotaGeldiBayrak       = false;
    bool arabaDurBayrak        = false;
    bool ileriGitBayrak        = false;
    bool geriGitBayrak         = false;
    bool sagaGitBayrak         = false;
    bool solaGitBayrak         = false;
    bool ileriDurBayrak        = false;
    bool geriDurBayrak         = false;


    //--------------------TEKER VERİLERİ----------------------
    float saghiz_f=0.0f;
    float solhiz_f=0.0f;

private:
    // -------------------- UART --------------------
    UART_HandleTypeDef* huart;
    uint8_t Data;    // DMA ile alınan tek byte

    // -------------------- RING BUFFER --------------------
    uint8_t ArayuzBuffer_u8[120];
    uint16_t readIndex_u16  = 0;
    uint16_t writeIndex_u16 = 0;

    // -------------------- PAKET DEĞİŞKENLERİ --------------------
    uint8_t tempBuffer[32];   // Maksimum beklenen data boyutu
	uint8_t tempIndex = 0;
    uint8_t baslik1_u8;
    uint8_t baslik2_u8;
    uint8_t paketTipi_u8;
    uint8_t dataBoyutu_u8;

    int16_t  dataLength_s16 = 0;
    uint32_t startIndex_u32 = 0;
    uint32_t intBits_u32    = 0;
    float    floatsonuc_f   = 0;

    // -------------------- ARAYÜZ VERİLERİ --------------------
    float ArayuzEnlem_f  = 0.0f;
    float ArayuzBoylam_f = 0.0f;

    // -------------------- PAKET DİZİLERİ --------------------
    uint8_t gpspaket[17];
    uint8_t imupaket[21];
    uint8_t versiyonpaket[8];
    uint8_t yoklamapaket[8];
    uint8_t rotapaket[8];
    uint8_t sistempaket[13];
    uint8_t komutpaket[17];

    // -------------------- BYTE DÖNÜŞÜM YARDIMCILARI --------------------
    uint8_t pitchBytes_u8[4];
    uint8_t rollBytes_u8[4];
    uint8_t headingBytes_u8[4];
    uint8_t sicaklikBytes_u8[4];
    uint8_t bataryaBytes_u8[4];
    uint8_t latBytes_u8[4];
    uint8_t lonBytes_u8[4];
    uint8_t altBytes_u8[4];
    uint8_t dereceBytes_u8[4];
    uint8_t yonBytes_u8[4];
    uint8_t rpmSagBytes_u8[4];
    uint8_t rpmSolBytes_u8[4];

    // -------------------- ÖZEL FONKSİYONLAR --------------------
    uint8_t CRC8Hesaplama(uint8_t *data, uint8_t baslangic, uint8_t bitis);
    void floatToBytes(float *Deger_f, uint8_t* bytes);
    float bytesToFloat(uint8_t* buffer_u8, int32_t startIndex_s32);
};

#endif /* INC_PAKET_H_ */
