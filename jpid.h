//
// Created by Jin Kim on 2021-12-05.
//

#ifndef JPID_JPID_H
#define JPID_JPID_H

#include <float.h>
#include <stdbool.h>

typedef struct
{
    float kP, kI, kD, kA;
    float errorAccumulation;
    float prevError;
    float errorAccumulationMin, errorAccumulationMax;
    float output;
    float outputMin, outputMax;
} JPID;

void JPID_init(JPID *jpid);

void JPID_setGainPID(JPID* jpid, const float kP, const float kI, const float kD);

void JPID_setGainPIDA(JPID* jpid, const float kP, const float kI, const float kD,const float kA);

void JPID_setAntiWindup(JPID* jpid, const float kA);

void JPID_setOutputMinMax(JPID* jpid, const float min, const float max);

void JPID_setErrorAccumulationMinMax(JPID* jpid, const float min, const float max);

float JPID_clip(const float x, const float min, const float max);

float JPID_update(JPID* jpid, const float target, const float measurement);

#endif //JPID_JPID_H
