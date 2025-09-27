/*
 * Madgwick.h
 *
 *  Created on: Sep 27, 2025
 *      Author: T_rab
 */

#ifndef INC_MADGWICK_H_
#define INC_MADGWICK_H_


#include <cmath>

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
