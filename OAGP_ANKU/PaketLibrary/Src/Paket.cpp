/*
 * Paket.cpp
 *
 *  Created on: Sep 27, 2025
 *      Author: T_rab
 */
#include "Paket.h"
#include <string.h>

enum Durumlar
{
	Baslik1Coz,
	Baslik2Coz,
	PaketTuruSec,
	DataBoyutuAl,
	DataOku
};
enum GelenPaketler
{
	ROTA=0x01,
	VERSIYON=0x02,
	YOKLAMA=0x03,
	DUR=0x04,
	YON=0x05,
	TEKER=0x06,
	KALIBRASYON=0x07,
	JETSON=0x08,

};

//enum GidenPaketler
//{
//	GPS=0x01,
//	IMU=0x02,
//	VERSIYON=0x03,
//	YOKLAMA=0x04,
//	ROTA =0x05,
//	SISTEM=0x06,
//	KOMUT=0x07,
//  KALMAN=0x08,
//  RPM=0x09
//};

Paket::Paket(UART_HandleTypeDef* huart)
{
	this->huart=huart;
}

Paket::Paket(uint8_t baslik1_u8, uint8_t baslik2_u8, uint8_t paketTipi_u8, uint8_t dataBoyutu_u8)
{
	this->baslik1_u8=baslik1_u8;
	this->baslik2_u8=baslik2_u8;
	this->paketTipi_u8=paketTipi_u8;
	this->dataBoyutu_u8=dataBoyutu_u8;
}

void Paket::PaketKesmeYapilandir()
{
	HAL_UART_Receive_DMA(huart, &Data,1);
}

void Paket::PaketBaslikYaz()
{
    txBuffer[0] = baslik1_u8;
    txBuffer[1] = baslik2_u8;
    txBuffer[2] = paketTipi_u8;
    txBuffer[3] = dataBoyutu_u8;
}
void Paket::GpsPaketOlustur(float latitude, float longitude, float mesafe)
{
    PaketBaslikYaz();
    floatToBytes(&latitude,  txBuffer + 4);
    floatToBytes(&longitude, txBuffer + 8);
    floatToBytes(&mesafe,    txBuffer + 12);
    txBuffer[16] = CRC8Hesaplama(txBuffer, 4, 16);
    txPaketBoyutu = 17;
}

void Paket::KalmanPaketOlustur(float latitude, float longitude, float yonelim)
{
    PaketBaslikYaz();
    floatToBytes(&latitude,  txBuffer + 4);
    floatToBytes(&longitude, txBuffer + 8);
    floatToBytes(&yonelim,   txBuffer + 12);
    txBuffer[16] = CRC8Hesaplama(txBuffer, 4, 16);
    txPaketBoyutu = 17;
}

void Paket::ImuPaketOlustur(float pitch, float roll, float heading)
{
    PaketBaslikYaz();
    floatToBytes(&pitch,   txBuffer + 4);
    floatToBytes(&roll,    txBuffer + 8);
    floatToBytes(&heading, txBuffer + 12);
    txBuffer[16] = CRC8Hesaplama(txBuffer, 4, 16);
    txPaketBoyutu = 17;
}

void Paket::RPMPaketOlustur(float sagrpm_f, float solrpm_f)
{
    PaketBaslikYaz();
    floatToBytes(&sagrpm_f, txBuffer + 4);
    floatToBytes(&solrpm_f, txBuffer + 8);
    txBuffer[12] = CRC8Hesaplama(txBuffer, 4, 12);
    txPaketBoyutu = 13;
}

void Paket::VersiyonPaketOlustur(uint8_t b, uint8_t o, uint8_t s)
{
    PaketBaslikYaz();
    txBuffer[4] = b;
    txBuffer[5] = o;
    txBuffer[6] = s;
    txBuffer[7] = CRC8Hesaplama(txBuffer, 4, 7);
    txPaketBoyutu = 8;
}

void Paket::YoklamaPaketOlustur(uint8_t jetsonBilgi)
{
    PaketBaslikYaz();
    txBuffer[4] = jetsonBilgi;
    txBuffer[5] = 0x02;
    txBuffer[6] = 0x03;
    txBuffer[7] = CRC8Hesaplama(txBuffer, 4, 7);
    txPaketBoyutu = 8;
}

void Paket::RotaPaketOlustur()
{
    PaketBaslikYaz();
    txBuffer[4] = 0x11;
    txBuffer[5] = 0x22;
    txBuffer[6] = 0x33;
    txBuffer[7] = CRC8Hesaplama(txBuffer, 4, 7);
    txPaketBoyutu = 8;
}

void Paket::SistemPaketOlustur(float birinciveri, float ikinciveri)
{
    PaketBaslikYaz();
    floatToBytes(&birinciveri, txBuffer + 4);
    floatToBytes(&ikinciveri,  txBuffer + 8);
    txBuffer[12] = CRC8Hesaplama(txBuffer, 4, 12);
    txPaketBoyutu = 13;
}

void Paket::KomutPaketOlustur(float yon, float rpmSag, float rpmSol)
{
    PaketBaslikYaz();
    floatToBytes(&yon,    txBuffer + 4);
    floatToBytes(&rpmSag, txBuffer + 8);
    floatToBytes(&rpmSol, txBuffer + 12);
    txBuffer[16] = CRC8Hesaplama(txBuffer, 4, 16);
    txPaketBoyutu = 17;
}

void Paket::txPaketCagir(uint8_t *kopyaDizi)
{
    memcpy(kopyaDizi, txBuffer, txPaketBoyutu);
}

void Paket::DataAlveBayrakKaldir()
{
    ArayuzBuffer_u8[writeIndex_u16] = Data;
    writeIndex_u16 = (writeIndex_u16 + 1) % sizeof(ArayuzBuffer_u8);

    HAL_UART_Receive_DMA(huart, &Data, 1);
}

void Paket::PaketCoz()
{
    Durumlar Durum = Baslik1Coz;
    GelenPaketler Paket = VERSIYON;

    while (readIndex_u16 != writeIndex_u16)
    {
        uint8_t byte = ArayuzBuffer_u8[readIndex_u16];
        readIndex_u16 = (readIndex_u16 + 1) % sizeof(ArayuzBuffer_u8);

        switch (Durum)
        {
            // -------------------- HEADER 1 --------------------
            case Baslik1Coz:
                if (byte == 0x12)
                    Durum = Baslik2Coz;
                break;

            // -------------------- HEADER 2 --------------------
            case Baslik2Coz:
                if (byte == 0x34)
                    Durum = PaketTuruSec;
                else
                    Durum = Baslik1Coz;
                break;

            // -------------------- PAKET TÜRÜ --------------------
            case PaketTuruSec:
                Paket = (GelenPaketler)byte;
                Durum = DataBoyutuAl;
                break;

            // -------------------- DATA BOYUTU --------------------
            case DataBoyutuAl:
                dataLength_s16 = byte;
                tempIndex = 0;
                Durum = DataOku;
                break;

            // -------------------- DATA OKUMA --------------------
            case DataOku:
                tempBuffer[tempIndex++] = byte;

                if (tempIndex >= dataLength_s16)
                {
                    bool crcDogru = false;

                    switch (Paket)
                    {
                    	// -------------------- TEKER --------------------
						case TEKER:
							if (dataLength_s16 == 9 &&tempBuffer[8] == CRC8Hesaplama(tempBuffer, 0, 8))
							{
								saghiz_f = bytesToFloat(tempBuffer, 0);
								solhiz_f = bytesToFloat(tempBuffer, 4);
							}
							break;
						// -------------------- JETSON --------------------
						case JETSON:
							if (dataLength_s16 == 10 &&tempBuffer[9] == CRC8Hesaplama(tempBuffer, 0, 9))
							{
								sagrpm_f = bytesToFloat(tempBuffer, 0);
								solrpm_f = bytesToFloat(tempBuffer, 4);
								durum_u8 = tempBuffer[8];

							}
							break;

                        // -------------------- ROTA --------------------
                        case ROTA:
                            if (dataLength_s16 == 9 && tempBuffer[8] == CRC8Hesaplama(tempBuffer, 0, 8))
                            {
                                if (!GidilecekNoktaBayrak)
                                {
                                    ArayuzEnlem_f = bytesToFloat(tempBuffer, 0);
                                    ArayuzBoylam_f = bytesToFloat(tempBuffer, 4);
                                }
                                GidilecekNoktaBayrak = true;
                                RotaGeldiBayrak = true;
                            }
                            break;
						 // -------------------- DUR --------------------
					     case DUR:
						    if (dataLength_s16 == 4 &&tempBuffer[3] == CRC8Hesaplama(tempBuffer, 0, 3))
						    {
							    ileriDurBayrak=true;
							    GidilecekNoktaBayrak = false;
						    }
						    break;

                        // -------------------- VERSİYON --------------------
                        case VERSIYON:
                            if (dataLength_s16 == 4 && tempBuffer[3] == CRC8Hesaplama(tempBuffer, 0, 3))
                            {
                                VersiyonPaketBayrak = true;
                            }
                            break;

                        // -------------------- YOKLAMA --------------------
                        case YOKLAMA:
                            if (dataLength_s16 == 4 && tempBuffer[3] == CRC8Hesaplama(tempBuffer, 0, 3))
                            {
                                YoklamaFlag = true;
                                YoklamaPaketFlag = true;
                            }
                            break;
                        // -------------------- YÖN --------------------
                        case YON:
                            if (dataLength_s16 == 4 &&tempBuffer[3] == CRC8Hesaplama(tempBuffer, 0, 3))
                            {
                                switch (tempBuffer[0])
                                {
                                    case 0x01: ileriGitBayrak = true; break;
                                    case 0x02: geriGitBayrak = true; break;
                                    case 0x03: sagaGitBayrak = true; break;
                                    case 0x04: solaGitBayrak = true; break;
                                    case 0x05: ileriDurBayrak = true; break;
                                    case 0x06: geriDurBayrak = true; break;
                                }
                            }
                            break;
                         // -------------------- KALIBRASYON --------------------
						 case KALIBRASYON:
							if (dataLength_s16 == 2 && tempBuffer[1] == CRC8Hesaplama(tempBuffer, 0, 1))
							{
								if (tempBuffer[0] == 0x01)
								{
									kalibrasyonMAGBayrak = true;
								}
								else if (tempBuffer[0] == 0x02)
								{
									kalibrasyonIMUBayrak = true;
								}
							}
							break;

                        default:
                            // Tanımsız paket tipi
                            break;
                    }

                    Durum = Baslik1Coz;
                }
                break;
        }
    }
}

float *Paket::ArayuzLatAl(){return &ArayuzEnlem_f;}
float *Paket::ArayuzLonAl(){return &ArayuzBoylam_f;}

uint8_t Paket::CRC8Hesaplama(uint8_t *data, uint8_t baslangic ,uint8_t bitis)
{
    uint8_t crc = 0x00;

    for (uint8_t i = baslangic; i < bitis; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0X07;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}

float Paket::bytesToFloat(uint8_t* buffer_u8, int32_t startIndex_s32)
{
	uint32_t intBits_u32 =(buffer_u8[(startIndex_s32 + 3)% sizeof(ArayuzBuffer_u8)] << 24) |
    					(buffer_u8[(startIndex_s32 + 2) % sizeof(ArayuzBuffer_u8)] << 16) |
						(buffer_u8[(startIndex_s32 + 1) % sizeof(ArayuzBuffer_u8)] << 8)  |
						(buffer_u8[(startIndex_s32 + 0) % sizeof(ArayuzBuffer_u8)] << 0)  ;

    memcpy(&floatsonuc_f, &intBits_u32, sizeof(floatsonuc_f));
    return floatsonuc_f;
}

void Paket::floatToBytes(float *Deger_f, uint8_t* bytes)
{
    uint8_t* p = (uint8_t*)Deger_f;
    for (int i = 0; i < 4; i++)
    {
        bytes[i] = p[i];
    }
}
