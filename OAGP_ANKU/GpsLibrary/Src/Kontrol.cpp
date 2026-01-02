/*
 * Kontrol.cpp
 *
 *  Created on: Dec 29, 2025
 *      Author: T_rab
 */

#include "Kontrol.h"

KontrolLibrary::KontrolLibrary()
    : kontrolAsamasi(KontrolAsamasi::DONME_MODU),
      heading(0.0f), enlem(0.0f), boylam(0.0f),
      hedefEnlem(0.0f), hedefBoylam(0.0f),
      rpmSol(0.0f), rpmSag(0.0f), hedefMesafe(0.0f),
      hedefYonelim(0.0f)
{
    float tekerHiziMaks = rpmdenHiz(MAKS_RPM);
    maksAciHizi = (2.0f * tekerHiziMaks) / IZ_ACIKLIGI;
}

void KontrolLibrary::Hesapla()
{
    // GPS tabanlı hedef hesapları
    hedefMesafe = mesafeHesapla(enlem, boylam, hedefEnlem, hedefBoylam);
    hedefYonelim = yonelimHesapla(enlem, boylam, hedefEnlem, hedefBoylam);

    // Heading hatası
    float aciHatasi = aciyiNormalizeEt(hedefYonelim - heading);
    float mutlakAciHatasi = std::fabs(aciHatasi);
    float aciHatasiRad = aciHatasi * (M_PI / 180.0f);

    // FSM: Histerezisli geçiş
    if (kontrolAsamasi == KontrolAsamasi::DONME_MODU)
    {
        if (mutlakAciHatasi < CIKIS_DONME_ESIGI)
            kontrolAsamasi = KontrolAsamasi::ILERLEME_MODU;
    }
    else
    {
        if (mutlakAciHatasi > GIRIS_DONME_ESIGI)
            kontrolAsamasi = KontrolAsamasi::DONME_MODU;
    }

    // STAGE 1: DONME_MODU
    if (kontrolAsamasi == KontrolAsamasi::DONME_MODU)
    {
        float omegaKomut = KP_YONELIM * aciHatasiRad;
        omegaKomut = sinirla(omegaKomut, -maksAciHizi, maksAciHizi);

        float tekerHizi = omegaKomut * IZ_ACIKLIGI * 0.5f;
        float rpm = HizdanRPM(std::fabs(tekerHizi));
        rpm = sinirla(rpm, MIN_RPM, MAKS_RPM);

        float isaret = (omegaKomut > 0.0f) ? 1.0f : -1.0f;

        if (hedefMesafe < 1.5f)
		{
        	rpm=0;
		}
        rpmSol =  isaret * rpm;
        rpmSag = -isaret * rpm;

    }
    // STAGE 2: ILERLEME_MODU
    else
    {
//        float omegaKomut = KP_YONELIM * aciHatasiRad;
//        omegaKomut = sinirla(omegaKomut, -maksAciHizi, maksAciHizi);
//
//        float yonelimOlcegi = 1.0f - (mutlakAciHatasi / GIRIS_DONME_ESIGI);
//        yonelimOlcegi = sinirla(yonelimOlcegi, 0.0f, 1.0f);
//
//        float temelHiz;
//        if (hedefMesafe > 20.0f)
//            temelHiz = 1.80f;
//        else if (hedefMesafe > 1.5f)
//            temelHiz = 1.55f * (hedefMesafe / 20.0f) + 0.25f;
//        else
//        {
//            temelHiz = 0.0f;
//            omegaKomut = 0.0f;
//            yonelimOlcegi = 0.0f;
//        }
//
//        float hizKomut = temelHiz * (0.5f + 0.5f * yonelimOlcegi);
//
//        float hizSol  = hizKomut + omegaKomut * IZ_ACIKLIGI * 0.5f;
//        float hizSag  = hizKomut - omegaKomut * IZ_ACIKLIGI * 0.5f;
//
//        rpmSol = sinirla(HizdanRPM(hizSol),  -MAKS_RPM, MAKS_RPM);
//        rpmSag = sinirla(HizdanRPM(hizSag),  -MAKS_RPM, MAKS_RPM);
    	        rpmSol =0;
    	        rpmSag = 0;
    }
}

// ===================== Yardımcı Fonksiyonlar =====================
float KontrolLibrary::aciyiNormalizeEt(float aci)
{
    while (aci > 180.0f)  aci -= 360.0f;
    while (aci < -180.0f) aci += 360.0f;
    return aci;
}

float KontrolLibrary::sinirla(float deger, float minDeger, float maxDeger)
{
    if (deger > maxDeger) return maxDeger;
    if (deger < minDeger) return minDeger;
    return deger;
}

float KontrolLibrary::rpmdenHiz(float rpm)
{
    return (rpm * 2.0f * M_PI * TEKER_YARICAPI) / 60.0f;
}

float KontrolLibrary::HizdanRPM(float hiz)
{
    return (hiz * 60.0f) / (2.0f * M_PI * TEKER_YARICAPI);
}

float KontrolLibrary::mesafeHesapla(float mevcutEnlem, float mevcutBoylam, float hedefEnlem, float hedefBoylam)
{
    mevcutEnlem *= (M_PI / 180.0f);
    mevcutBoylam *= (M_PI / 180.0f);
    hedefEnlem  *= (M_PI / 180.0f);
    hedefBoylam *= (M_PI / 180.0f);

    float farkEnlem = hedefEnlem - mevcutEnlem;
    float farkBoylam = hedefBoylam - mevcutBoylam;

    float a = std::sin(farkEnlem*0.5f) * std::sin(farkEnlem*0.5f) +
              std::cos(mevcutEnlem) * std::cos(hedefEnlem) *
              std::sin(farkBoylam*0.5f) * std::sin(farkBoylam*0.5f);

    float c = 2.0f * std::atan2(std::sqrt(a), std::sqrt(1.0f - a));
    return DUNYA_YARICAPI_METRE * c;
}

float KontrolLibrary::yonelimHesapla(float mevcutEnlem, float mevcutBoylam, float hedefEnlem, float hedefBoylam)
{
    mevcutEnlem *= (M_PI / 180.0f);
    mevcutBoylam *= (M_PI / 180.0f);
    hedefEnlem  *= (M_PI / 180.0f);
    hedefBoylam *= (M_PI / 180.0f);

    float farkBoylam = hedefBoylam - mevcutBoylam;

    float x = std::sin(farkBoylam) * std::cos(hedefEnlem);
    float y = std::cos(mevcutEnlem) * std::sin(hedefEnlem) -
              std::sin(mevcutEnlem) * std::cos(hedefEnlem) * std::cos(farkBoylam);

    float yonelim = std::atan2(x, y) * (180.0f / M_PI);
    return std::fmod(yonelim + 360.0f, 360.0f);
}



