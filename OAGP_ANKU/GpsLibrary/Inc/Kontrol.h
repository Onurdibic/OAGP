/*
 * Kontrol.h
 *
 *  Created on: Dec 29, 2025
 *      Author: T_rab
 */

#ifndef KONTROLLIBRARY_H
#define KONTROLLIBRARY_H

#include <cmath>
#include <cstdint>

class KontrolLibrary
{
public:
    KontrolLibrary();

    void SetHeading(float yeniHeading) { heading = yeniHeading; }
    void SetKonum(float enlem_, float boylam_) { enlem = enlem_; boylam = boylam_; }
    void SetHedef(float hedefEnlem_, float hedefBoylam_) { hedefEnlem = hedefEnlem_; hedefBoylam = hedefBoylam_; }

    void Hesapla(); // GPS tabanlı yön ve RPM hesaplarını yapar

    // Hesaplanan değerler
    float rpmSol;
    float rpmSag;
    float hedefMesafe;
    float hedefYonelim;

    // Sabitler
    static constexpr float TEKER_YARICAPI = 0.085f;
    static constexpr float IZ_ACIKLIGI = 0.45f;
    static constexpr float MAKS_RPM = 210.0f;
    static constexpr float MIN_RPM = 35.0f;
    static constexpr float KP_YONELIM = 3.0f;
    static constexpr float GIRIS_DONME_ESIGI = 35.0f;
    static constexpr float CIKIS_DONME_ESIGI = 25.0f;
    static constexpr float DUNYA_YARICAPI_METRE = 6371000.0f;

    // FSM
    enum class KontrolAsamasi { DONME_MODU, ILERLEME_MODU };
    KontrolAsamasi kontrolAsamasi;

    // Durum
    float heading;
    float enlem;
    float boylam;
    float hedefEnlem;
    float hedefBoylam;
    float maksAciHizi;

    // Yardımcı fonksiyonlar
    static float aciyiNormalizeEt(float aci);
    static float sinirla(float deger, float minDeger, float maxDeger);
    static float rpmdenHiz(float rpm);
    static float HizdanRPM(float hiz);
    static float mesafeHesapla(float mevcutEnlem, float mevcutBoylam, float hedefEnlem, float hedefBoylam);
    static float yonelimHesapla(float mevcutEnlem, float mevcutBoylam, float hedefEnlem, float hedefBoylam);
};

#endif // KONTROLLIBRARY_H

