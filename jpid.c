//
// Created by Jin Kim on 2021-12-05.
//

#include "jpid.h"
#include <stdio.h>

void JPID_init(JPID *jpid)
{
    jpid->kP = 0.0f;
    jpid->kI = 0.0f;
    jpid->kD = 0.0f;
    jpid->kA = 0.0f;
    jpid->errorAccumulation = 0.0f;
    jpid->prevError = 0.0f;
    jpid->errorAccumulationMin = -FLT_MAX;
    jpid->errorAccumulationMax = FLT_MAX;
    jpid->output = 0.0f;
    jpid->outputMin = -FLT_MAX;
    jpid->outputMax = FLT_MAX;
}

void JPID_setGain(JPID *jpid, const float kP, const float kI, const float kD)
{
    jpid->kP = kP;
    jpid->kI = kI;
    jpid->kD = kD;
}

void JPID_setAntiWindup(JPID *jpid, const float kA)
{
    jpid->kA = kA;
}

void JPID_setOutputMinMax(JPID *jpid, const float min, const float max)
{
    jpid->outputMin = min;
    jpid->outputMax = max;
}

void JPID_setErrorAccumulationMinMax(JPID *jpid, const float min, const float max)
{
    jpid->errorAccumulationMin = min;
    jpid->errorAccumulationMax = max;
}

float JPID_clip(const float x, const float min, const float max)
{
    if(x < min) return min;
    if(max < x) return max;
    return x;
}

float JPID_update(JPID *jpid, const float target, const float measurement)
{
    float error = target - measurement;

    printf("error:%lf\n", error);

    jpid->errorAccumulation = JPID_clip(jpid->errorAccumulation + error, jpid->errorAccumulationMin, jpid->errorAccumulationMax);

    float pTerm = jpid->kP * error;
    printf("pTerm:%lf\n", pTerm);
    float iTerm = jpid->kI * jpid->errorAccumulation;
    printf("iTerm:%lf\n", iTerm);
    float dTerm = jpid->kD * (error - jpid->prevError);
    printf("dTerm:%lf\n", dTerm);

    jpid->prevError = error;

    float outputUnclipped = pTerm + iTerm + dTerm;
    float outputClipped = JPID_clip(outputUnclipped, jpid->outputMin, jpid->outputMax);
    printf("clipped:%lf\n", outputClipped);

    jpid->errorAccumulation -= (outputUnclipped - outputClipped) * jpid->kA * jpid->kI;

    jpid->output = outputClipped;
    return jpid->output;
}