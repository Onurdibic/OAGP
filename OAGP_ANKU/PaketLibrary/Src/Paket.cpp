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
	ENGEL=0x08
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

void Paket::GpsPaketOlustur(float latitude,float longitude,float mesafe)
{
    gpspaket[0] = baslik1_u8;
    gpspaket[1] = baslik2_u8;
    gpspaket[2] = paketTipi_u8;
    gpspaket[3] = dataBoyutu_u8;

    floatToBytes(&latitude, latBytes_u8);
    floatToBytes(&longitude, lonBytes_u8);
    floatToBytes(&mesafe, mesafeBytes_u8);

    memcpy(gpspaket + 4, latBytes_u8, 4);
    memcpy(gpspaket + 8, lonBytes_u8, 4);
    memcpy(gpspaket + 12, mesafeBytes_u8, 4);

    gpspaket[16]=CRC8Hesaplama(gpspaket,4, 16);
}

void Paket::KalmanPaketOlustur(float latitude,float longitude, float yonelim)
{
    kalmanpaket[0] = baslik1_u8;
    kalmanpaket[1] = baslik2_u8;
    kalmanpaket[2] = paketTipi_u8;
    kalmanpaket[3] = dataBoyutu_u8;

    floatToBytes(&latitude, latBytes_u8);
    floatToBytes(&longitude, lonBytes_u8);
    floatToBytes(&yonelim, yonelimBytes_u8);

    memcpy(kalmanpaket + 4, latBytes_u8, 4);
    memcpy(kalmanpaket + 8, lonBytes_u8, 4);
    memcpy(kalmanpaket + 12, yonelimBytes_u8, 4);

    kalmanpaket[16]=CRC8Hesaplama(kalmanpaket,4, 16);
}


void Paket::ImuPaketOlustur(float pitch,float roll,float heading)
{
    imupaket[0] = baslik1_u8;
    imupaket[1] = baslik2_u8;
    imupaket[2] = paketTipi_u8;
    imupaket[3] = dataBoyutu_u8;

    floatToBytes(&pitch, pitchBytes_u8);
    floatToBytes(&roll, rollBytes_u8);
    floatToBytes(&heading, headingBytes_u8);

    memcpy(imupaket + 4, pitchBytes_u8, 4);
    memcpy(imupaket + 8, rollBytes_u8, 4);
    memcpy(imupaket + 12, headingBytes_u8, 4);

    imupaket[16] = CRC8Hesaplama(imupaket, 4,16);
}

void Paket::RPMPaketOlustur(float sagrpm_f ,float solrpm_f)
{
	rpmpaket[0] = baslik1_u8;
	rpmpaket[1] = baslik2_u8;
	rpmpaket[2] = paketTipi_u8;
	rpmpaket[3] = dataBoyutu_u8;

    floatToBytes(&sagrpm_f, sagrpmBytes_u8);
    floatToBytes(&solrpm_f, solrpmBytes_u8);

    memcpy(rpmpaket + 4, sagrpmBytes_u8, 4);
    memcpy(rpmpaket + 8, solrpmBytes_u8, 4);

    rpmpaket[12]=CRC8Hesaplama(rpmpaket,4, 12);
}


void Paket::VersiyonPaketOlustur(uint8_t b,uint8_t o,uint8_t s)
{
	versiyonpaket[0] = baslik1_u8;
	versiyonpaket[1] = baslik2_u8;
	versiyonpaket[2] = paketTipi_u8;
	versiyonpaket[3] = dataBoyutu_u8;
	versiyonpaket[4] = b;
	versiyonpaket[5] = o;
	versiyonpaket[6] = s;
	versiyonpaket[7] = CRC8Hesaplama(versiyonpaket, 4,7);
}

void Paket::YoklamaPaketOlustur(uint8_t jetsonBilgi)
{
	yoklamapaket[0] = baslik1_u8;
	yoklamapaket[1] = baslik2_u8;
	yoklamapaket[2] = paketTipi_u8;
	yoklamapaket[3] = dataBoyutu_u8;
	yoklamapaket[4] = jetsonBilgi;
	yoklamapaket[5] = 0x02;
	yoklamapaket[6] = 0x03;
	yoklamapaket[7] = CRC8Hesaplama(yoklamapaket, 4,7);
}

void Paket::RotaPaketOlustur()
{
	rotapaket[0] = baslik1_u8;
	rotapaket[1] = baslik2_u8;
	rotapaket[2] = paketTipi_u8;
	rotapaket[3] = dataBoyutu_u8;
	rotapaket[4] = 0x11;
	rotapaket[5] = 0x22;
	rotapaket[6] = 0x33;
	rotapaket[7] = CRC8Hesaplama(rotapaket, 4,7);
}

void Paket::SistemPaketOlustur(float birinciveri,float ikinciveri)
{
	sistempaket[0] = baslik1_u8;
	sistempaket[1] = baslik2_u8;
	sistempaket[2] = paketTipi_u8;
	sistempaket[3] = dataBoyutu_u8;
	floatToBytes(&birinciveri, dereceBytes_u8);
	floatToBytes(&ikinciveri, bataryaBytes_u8);
	memcpy(sistempaket + 4, dereceBytes_u8, 4);
	memcpy(sistempaket + 8, bataryaBytes_u8, 4);

	sistempaket[12] = CRC8Hesaplama(sistempaket, 4,12);
}

void Paket::KomutPaketOlustur(float yon, float rpmSag, float rpmSol)
{
	komutpaket[0] = baslik1_u8;
	komutpaket[1] = baslik2_u8;
	komutpaket[2] = paketTipi_u8;
	komutpaket[3] = dataBoyutu_u8;
	floatToBytes(&yon, yonBytes_u8);
	floatToBytes(&rpmSag, rpmSagBytes_u8);
	floatToBytes(&rpmSol, rpmSolBytes_u8);
	memcpy(komutpaket + 4, yonBytes_u8, 4);
	memcpy(komutpaket + 8, rpmSagBytes_u8, 4);
	memcpy(komutpaket + 12, rpmSolBytes_u8, 4);

	komutpaket[16] = CRC8Hesaplama(komutpaket, 4,16);
}

void Paket::gpsPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, gpspaket, sizeof(gpspaket));}
void Paket::imuPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, imupaket, sizeof(imupaket));}
void Paket::versiyonPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, versiyonpaket, sizeof(versiyonpaket));}
void Paket::yoklamaPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, yoklamapaket, sizeof(yoklamapaket));}
void Paket::rotaPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, rotapaket, sizeof(rotapaket));}
void Paket::sistemPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, sistempaket, sizeof(sistempaket));}
void Paket::komutPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, komutpaket, sizeof(komutpaket));}
void Paket::kalmanPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, kalmanpaket, sizeof(kalmanpaket));}
void Paket::rpmPaketCagir(uint8_t *kopyaDizi){memcpy(kopyaDizi, rpmpaket, sizeof(rpmpaket));}

void Paket::DataAlveBayrakKaldir()
{
    ArayuzBuffer_u8[writeIndex_u16] = Data;
    writeIndex_u16 = (writeIndex_u16 + 1) % sizeof(ArayuzBuffer_u8);

    HAL_UART_Receive_DMA(huart, &Data, 1);
}

void Paket::PaketCoz()
{
    static Durumlar Durum = Baslik1Coz;
    static GelenPaketler Paket = VERSIYON;

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
						// -------------------- ENGEL --------------------
						case ENGEL:
							if (dataLength_s16 == 10 &&tempBuffer[9] == CRC8Hesaplama(tempBuffer, 0, 9))
							{
								sagrpm_f = bytesToFloat(tempBuffer, 0);
								solrpm_f = bytesToFloat(tempBuffer, 4);
								engel_u8 = tempBuffer[8];
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
        crc ^= data[i]; // CRC deÃ„Å¸erini, dizinin bir sonraki byte ile XOR
        for (uint8_t j = 0; j < 8; j++) //Her bir byte iÃƒÂ§in dÃƒÂ¶ngÃƒÂ¼
        {
            if (crc & 0x80)//CRC deÃ„Å¸erinin en soldaki biti 1 mi
            {
                crc = (crc << 1) ^ 0X07; //En yÃƒÂ¼ksek bit birse CRC deÃ„Å¸erini bir bit sola kaydÃ„Â±r ve XOR iÃ…Å¸lemi yap
            }
            else
            {
                crc <<= 1; //En yÃƒÂ¼ksek bit sÃ„Â±fÃ„Â±rsa CRC deÃ„Å¸erini bir bit sola kaydÃ„Â±r
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



//#include "Paket.h"
//
//enum Durumlar
//{
//    Baslik1,
//    Baslik2,
//    PaketTuru,
//    DataBoyutu,
//    DataOku
//};
//
//Paket::Paket(UART_HandleTypeDef* huart)
//{
//    this->huart=huart;
//}
//
//uint8_t* Paket::GetTxBuffer()
//{
//    return txBuffer;
//}
//
//void Paket::PaketKesmeYapilandir()
//{
//    HAL_UART_Receive_DMA(huart,&Data,1);
//}
//
//void Paket::DataAlveBayrakKaldir()
//{
//    rxBuffer[writeIndex]=Data;
//    writeIndex=(writeIndex+1)%sizeof(rxBuffer);
//
//    HAL_UART_Receive_DMA(huart,&Data,1);
//}
//
//uint16_t Paket::BuildPacket(uint8_t type,uint8_t* data,uint8_t len)
//{
//    txBuffer[0]=HEADER1;
//    txBuffer[1]=HEADER2;
//    txBuffer[2]=type;
//    txBuffer[3]=len+1;
//
//    memcpy(&txBuffer[4],data,len);
//
//    txBuffer[4+len]=CRC8(txBuffer,4,4+len);
//
//    return len+5;
//}
//
//float Paket::bytesToFloat(uint8_t* b)
//{
//    FloatBytes f;
//    memcpy(f.b,b,4);
//    return f.f;
//}
//
//uint16_t Paket::GpsPaketOlustur(float lat,float lon,float mesafe)
//{
//    FloatBytes f;
//    uint8_t data[12];
//
//    f.f=lat; memcpy(&data[0],f.b,4);
//    f.f=lon; memcpy(&data[4],f.b,4);
//    f.f=mesafe; memcpy(&data[8],f.b,4);
//
//    return BuildPacket(0x01,data,12);
//}
//
//uint16_t Paket::KalmanPaketOlustur(float lat,float lon,float yon)
//{
//    FloatBytes f;
//    uint8_t data[12];
//
//    f.f=lat; memcpy(&data[0],f.b,4);
//    f.f=lon; memcpy(&data[4],f.b,4);
//    f.f=yon; memcpy(&data[8],f.b,4);
//
//    return BuildPacket(0x08,data,12);
//}
//uint16_t Paket::RotaPaketOlustur()
//{
//    // Örnek sabit veri
//    uint8_t data[3];
//    data[0] = 0x11;
//    data[1] = 0x22;
//    data[2] = 0x33;
//
//    // BuildPacket ile paket oluştur
//    return BuildPacket(0x05, data, 3);
//}
//uint16_t Paket::ImuPaketOlustur(float pitch,float roll,float yaw)
//{
//    FloatBytes f;
//    uint8_t data[12];
//
//    f.f=pitch; memcpy(&data[0],f.b,4);
//    f.f=roll; memcpy(&data[4],f.b,4);
//    f.f=yaw; memcpy(&data[8],f.b,4);
//
//    return BuildPacket(0x02,data,12);
//}
//
//uint16_t Paket::RPMPaketOlustur(float sag,float sol)
//{
//    FloatBytes f;
//    uint8_t data[8];
//
//    f.f=sag; memcpy(&data[0],f.b,4);
//    f.f=sol; memcpy(&data[4],f.b,4);
//
//    return BuildPacket(0x09,data,8);
//}
//
//uint16_t Paket::VersiyonPaketOlustur(uint8_t b,uint8_t o,uint8_t s)
//{
//    uint8_t data[3];
//    data[0]=b;
//    data[1]=o;
//    data[2]=s;
//
//    return BuildPacket(0x03,data,3);
//}
//
//uint16_t Paket::YoklamaPaketOlustur(uint8_t jetsonBilgi)
//{
//    uint8_t data[3];
//    data[0]=jetsonBilgi;
//    data[1]=0x02;
//    data[2]=0x03;
//
//    return BuildPacket(0x04,data,3);
//}
//
//uint16_t Paket::KomutPaketOlustur(float yon,float rpmSag,float rpmSol)
//{
//    FloatBytes f;
//    uint8_t data[12];
//
//    f.f=yon; memcpy(&data[0],f.b,4);
//    f.f=rpmSag; memcpy(&data[4],f.b,4);
//    f.f=rpmSol; memcpy(&data[8],f.b,4);
//
//    return BuildPacket(0x07,data,12);
//}
//
//uint16_t Paket::SistemPaketOlustur(float sicaklik,float batarya)
//{
//    FloatBytes f;
//    uint8_t data[8];
//
//    f.f=sicaklik; memcpy(&data[0],f.b,4);
//    f.f=batarya; memcpy(&data[4],f.b,4);
//
//    return BuildPacket(0x06,data,8);
//}
//
//void Paket::PaketCoz()
//{
//    static Durumlar durum=Baslik1;
//    static uint8_t paketTipi;
//
//    while(readIndex!=writeIndex)
//    {
//        uint8_t byte=rxBuffer[readIndex];
//        readIndex=(readIndex+1)%sizeof(rxBuffer);
//
//        switch(durum)
//        {
//            case Baslik1:
//                if(byte==HEADER1)
//                    durum=Baslik2;
//            break;
//
//            case Baslik2:
//                if(byte==HEADER2)
//                    durum=PaketTuru;
//                else
//                    durum=Baslik1;
//            break;
//
//            case PaketTuru:
//                paketTipi=byte;
//                durum=DataBoyutu;
//            break;
//
//            case DataBoyutu:
//                dataLength=byte;
//                tempIndex=0;
//                durum=DataOku;
//            break;
//
//            case DataOku:
//
//                tempBuffer[tempIndex++]=byte;
//
//                if(tempIndex>=dataLength+1)
//                {
//                    switch(paketTipi)
//                    {
//                    	case VERSIYON:
//						if (dataLength == 4 && tempBuffer[3] == CRC8(tempBuffer, 0, 3))
//						{
//							VersiyonPaketBayrak = true;
//						}
//						break;
//
//                        // -------------------- YOKLAMA --------------------
//                        case YOKLAMA:
//                            if (dataLength == 4 && tempBuffer[3] == CRC8(tempBuffer, 0, 3))
//                            {
//                                YoklamaFlag = true;
//                                YoklamaPaketFlag = true;
//                            }
//                            break;
//                        case TEKER:
//
//                            if(dataLength==9 &&tempBuffer[8]==CRC8(tempBuffer,0,8))
//                            {
//                                saghiz_f=bytesToFloat(&tempBuffer[0]);
//                                solhiz_f=bytesToFloat(&tempBuffer[4]);
//                            }
//
//                        break;
//
//                        case ENGEL:
//
//                            if(dataLength==10 &&tempBuffer[9]==CRC8(tempBuffer,0,9))
//                            {
//                                sagrpm_f=bytesToFloat(&tempBuffer[0]);
//                                solrpm_f=bytesToFloat(&tempBuffer[4]);
//                                engel_u8=tempBuffer[8];
//                            }
//
//                        break;
//
//                        case ROTA:
//
//                            if(dataLength==9 &&tempBuffer[8]==CRC8(tempBuffer,0,8))
//                            {
//                                if(!GidilecekNoktaBayrak)
//                                {
//                                    ArayuzEnlem_f=bytesToFloat(&tempBuffer[0]);
//                                    ArayuzBoylam_f=bytesToFloat(&tempBuffer[4]);
//                                }
//
//                                GidilecekNoktaBayrak=true;
//                                RotaGeldiBayrak=true;
//                            }
//
//                        break;
//
//                        case DUR:
//
//                            if(dataLength==4 && tempBuffer[3]==CRC8(tempBuffer,0,3))
//                            {
//                                ileriDurBayrak=true;
//                                GidilecekNoktaBayrak=false;
//                            }
//
//                        break;
//
//                        case YON:
//
//                            if(dataLength==4 &&tempBuffer[3]==CRC8(tempBuffer,0,3))
//                            {
//                                switch(tempBuffer[0])
//                                {
//                                    case 1: ileriGitBayrak=true; break;
//                                    case 2: geriGitBayrak=true; break;
//                                    case 3: sagaGitBayrak=true; break;
//                                    case 4: solaGitBayrak=true; break;
//                                }
//                            }
//
//                        break;
//
//                        case KALIBRASYON:
//
//                            if(dataLength==2 &&tempBuffer[1]==CRC8(tempBuffer,0,1))
//                            {
//                                if(tempBuffer[0]==0x01)
//                                    kalibrasyonMAGBayrak=true;
//
//                                if(tempBuffer[0]==0x02)
//                                    kalibrasyonIMUBayrak=true;
//                            }
//
//                        break;
//
//                    }
//
//                    durum=Baslik1;
//                }
//
//            break;
//        }
//    }
//}
//
//float* Paket::ArayuzLatAl()
//{
//    return &ArayuzEnlem_f;
//}
//
//float* Paket::ArayuzLonAl()
//{
//    return &ArayuzBoylam_f;
//}
//
//uint8_t Paket::CRC8(uint8_t* data,uint8_t start,uint8_t end)
//{
//    uint8_t crc=0;
//
//    for(uint8_t i=start;i<end;i++)
//    {
//        crc^=data[i];
//
//        for(uint8_t j=0;j<8;j++)
//        {
//            if(crc&0x80)
//                crc=(crc<<1)^0x07;
//            else
//                crc = crc << 1;
//        }
//    }
//
//    return crc;
//}
//
