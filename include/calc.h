#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED


float GetChargeVoltage(void);
float GetBatteryCurrent(void);
int16_t GetBatteryTemp(void);

float GetLiftedWeightSpd0(void);
float GetLiftedWeightSpd1(void);
float GetLiftedWeightSpd2(void);

void InitAcCurrentFilter(float smpfreq, float cutoff);
void CalcFilteredACcurrent(void);

#endif