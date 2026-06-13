/*
 * Paket.h
 *
 *  Created on: Sep 27, 2025
 *      Author: T_rab
 */


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
    void GpsPaketOlustur(float latitude, float longitude, float mesafe);
    void KalmanPaketOlustur(float latitude, float longitude, float yonelim);
    void ImuPaketOlustur(float pitch, float roll, float yaw);
    void RPMPaketOlustur(float sagrpm_f ,float solrpm_f);
    void VersiyonPaketOlustur(uint8_t b, uint8_t o, uint8_t s);
    void YoklamaPaketOlustur(uint8_t jetsonBilgi);
    void RotaPaketOlustur();
    void SistemPaketOlustur(float birinciveri,float ikinciveri);
    void KomutPaketOlustur(float yon, float rpmSag, float rpmSol);
    void PaketBaslikYaz();

    // -------------------- PAKET KOPYALAMA --------------------
    void txPaketCagir(uint8_t *kopyaDizi);

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
    bool ArabaDurBayrak        = false;
    bool ileriGitBayrak        = false;
    bool geriGitBayrak         = false;
    bool sagaGitBayrak         = false;
    bool solaGitBayrak         = false;
    bool ileriDurBayrak        = false;
    bool geriDurBayrak         = false;
    bool kalibrasyonMAGBayrak  = false;
    bool kalibrasyonIMUBayrak  = false;


    //--------------------TEKER VERİLERİ----------------------
    float saghiz_f=0.0f;
    float solhiz_f=0.0f;
    // -------------------- ENGEL VERİLERİ --------------------
    float sagrpm_f=0.0f;
	float solrpm_f=0.0f;
	uint8_t durum_u8=0;
    // -------------------- ARAYÜZ VERİLERİ --------------------
   float ArayuzEnlem_f  = 0.0f;
   float ArayuzBoylam_f = 0.0f;

private:
    // -------------------- UART --------------------
    UART_HandleTypeDef* huart;
    uint8_t Data;    // DMA ile alınan tek byte

    // -------------------- RING BUFFER --------------------
    uint8_t ArayuzBuffer_u8[120];
    uint16_t readIndex_u16  = 0;
    uint16_t writeIndex_u16 = 0;

    // -------------------- PAKET DEĞİŞKENLERİ --------------------
    uint8_t tempBuffer[25];   // Maksimum beklenen data boyutu
	uint8_t tempIndex = 0;
    uint8_t baslik1_u8;
    uint8_t baslik2_u8;
    uint8_t paketTipi_u8;
    uint8_t dataBoyutu_u8;
    uint8_t txPaketBoyutu;
    int16_t  dataLength_s16 = 0;
    uint32_t startIndex_u32 = 0;
    uint32_t intBits_u32    = 0;
    float    floatsonuc_f   = 0;

    // -------------------- PAKET DİZİLERİ --------------------
    uint8_t txBuffer[17];


    // -------------------- ÖZEL FONKSİYONLAR --------------------
    uint8_t CRC8Hesaplama(uint8_t *data, uint8_t baslangic, uint8_t bitis);
    void floatToBytes(float *Deger_f, uint8_t* bytes);
    float bytesToFloat(uint8_t* buffer_u8, int32_t startIndex_s32);
};
