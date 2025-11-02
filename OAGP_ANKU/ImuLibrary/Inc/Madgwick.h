/*
 * Madgwick.h
 *
 *  Created on: Sep 27, 2025
 *      Author: T_rab
 */

#ifndef INC_MADGWICK_H_
#define INC_MADGWICK_H_


#include <cmath>

#define PI 3.1415f

class MadgwickAHRS
{
private:
    float beta;          // algoritma sabiti (gï¿½rï¿½ltï¿½ seviyesine baï¿½lï¿½)


public:
    MadgwickAHRS(float beta = 0.2f) : beta(beta), q0(1.0f), q1(0.0f), q2(0.0f), q3(0.0f) {}
    float q0, q1, q2, q3; // quaternion

    void updateIMU(float gx, float gy, float gz, float ax, float ay, float az, float dt);
    void getEuler(float &pitch, float &roll, float &yaw);
};


#endif /* INC_MADGWICK_H_ */


//#ifndef MADGWICK_AHRS_H
//#define MADGWICK_AHRS_H
//
//#include <cmath>
//
//class MadgwickAHRS {
//public:
//    MadgwickAHRS(float sampleFreq = 200.0f, float beta = 0.1f);
//
//    // Güncelleme fonksiyonları
//    void update(float gx, float gy, float gz,
//                float ax, float ay, float az,
//                float mx, float my, float mz);
//
//    void updateIMU(float gx, float gy, float gz,
//                   float ax, float ay, float az);
//
//    // Euler açılarını elde et (derece cinsinden)
//    void getEulerAngles(float &roll, float &pitch, float &yaw) const;
//
//    // Getter - quaternion değerlerini döndürür
//    void getQuaternion(float &q0, float &q1, float &q2, float &q3) const;
//
//    // Parametre ayarları
//    void setBeta(float b) { beta = b; }
//    void setSampleFreq(float freq) { sampleFreq = freq; }
//
//private:
//    float beta;        // 2 * proportional gain
//    float sampleFreq;  // örnekleme frekansı (Hz)
//    float q0, q1, q2, q3; // quaternion
//
//    float invSqrt(float x);
//};
//
//#endif // MADGWICK_AHRS_H
