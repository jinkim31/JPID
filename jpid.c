//
// Created by Jin Kim on 2021-12-05.
//

#include "jpid.h"

void JPID_init(JPID *jpid)
{
    jpid->kP = 0.0f;
    jpid->kI = 0.0f;
    jpid->kD = 0.0f;
    jpid->kA = 0.0f;
    jpid->errorAccumulation = 0.0f;
    jpid->prevError = 0.0f;
    jpid->errorAccumulationMinMax.min= -FLT_MAX;
    jpid->errorAccumulationMinMax.max = FLT_MAX;
    jpid->output = 0.0f;
    jpid->outputMinMax.min = -FLT_MAX;
    jpid->outputMinMax.max = FLT_MAX;
}

void JPID_setGainPID(JPID *jpid, const float kP, const float kI, const float kD)
{
    jpid->kP = kP;
    jpid->kI = kI;
    jpid->kD = kD;
}

void JPID_setGainPIDA(JPID* jpid, const float kP, const float kI, const float kD,const float kA)
{
    jpid->kP = kP;
    jpid->kI = kI;
    jpid->kD = kD;
    jpid->kA = kA;
}

void JPID_setAntiWindup(JPID *jpid, const float kA)
{
    jpid->kA = kA;
}

void JPID_setOutputMinMax(JPID *jpid, const float min, const float max)
{
    jpid->outputMinMax.min = min;
    jpid->outputMinMax.max = max;
}

void JPID_setErrorAccumulationMinMax(JPID *jpid, const float min, const float max)
{
    jpid->errorAccumulationMinMax.min = min;
    jpid->errorAccumulationMinMax.max = max;
}

const float JPID_clip(const float x, const MinMax* minMax)
{
    if(x < minMax->min) return minMax->min;
    if(minMax->max < x) return minMax->max;
    return x;
}

const float JPID_update(JPID *jpid, const float target, const float measurement)
{
    float error = target - measurement;

    jpid->errorAccumulation = JPID_clip(jpid->errorAccumulation + error, &jpid->errorAccumulationMinMax);

    float pTerm = jpid->kP * error;
    float iTerm = jpid->kI * jpid->errorAccumulation;
    float dTerm = jpid->kD * (error - jpid->prevError);

    jpid->prevError = error;

    float outputUnclipped = pTerm + iTerm + dTerm;
    float outputClipped = JPID_clip(outputUnclipped, &jpid->outputMinMax);

    jpid->errorAccumulation -= (outputUnclipped - outputClipped) * jpid->kA * jpid->kI;

    jpid->output = outputClipped;
    return jpid->output;
}

const float JPID_getOutput(JPID *jpid)
{
    return jpid->output;
}
