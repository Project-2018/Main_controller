#include "ch.h"
#include "hal.h"
#include "measure.h"
#include "calc.h"
#include "esc_comm.h"
#include "comm_uart.h"

float BattCurr_CalibOUT[] = {
  -30.0f,
  -25.0f,
  -20.0f,
  -17.5f,
  -15.0f,
  -12.5f,
  -10.0f,
  -7.5f,
  -5.0f,
  -4.0f,
  -3.0f,
  -2.0f,
  -1.0f,
  -0.5f,
  0.0f,
  0.0f,
  0.5f,
  1.0f,
  2.0f,
  3.0f,
  4.0f,
  5.0f,
  7.5f,
  10.0f,
  12.5f,
  15.0f,
  17.5f,
  20.0f,
  25.0f,
  30.0f
};

float BattCurr_CalibIN[] = {
	1157.0f,
	1245.0f,
	1348.0f,
	1405.0f,
	1468.0f,
	1536.0f,
	1611.0f,
	1692.0f,
	1781.0f,
	1818.0f,
	1856.0f,
	1896.0f,
	1938.0f,
	1959.0f,
	1977.0f,
	1980.0f,
	2001.0f,
	2022.0f,
	2064.0f,
	2104.0f,
	2142.0f,
	2179.0f,
	2268.0f,
	2349.0f,
	2424.0f,
	2492.0f,
	2555.0f,
	2612.0f,
	2715.0f,
	2803.0f
};

float CurrWeight0_IN[] = {
  0.2f,
  2.0f
};

float CurrWeight0_OUT[] = {
  0.0f,
  10.0f
};



float CurrWeight1_IN[] = {
  0.2f,
  0.6f,
  2.0f
};

float CurrWeight1_OUT[] = {
  0.0f,
  5.0f,
  10.0f
};



float CurrWeight2_IN[] = {
  0.2f,
  10.0f,
  20.0f
};

float CurrWeight2_OUT[] = {
  0.0f,
  85.0f,
  190.0f
};



float CurrentmultiMap(float val, float* _in, float* _out, uint16_t size)
{
  // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size-1]) return _out[size-1];

  // search right interval
  uint8_t pos = 1;  // _in[0] allready tested
  while(val > _in[pos]) pos++;

  // this will handle all exact "points" in the _in array
  if (val == _in[pos]) return _out[pos];

  // interpolate in the right segment for the rest
  return (val - _in[pos-1]) * (_out[pos] - _out[pos-1]) / (_in[pos] - _in[pos-1]) + _out[pos-1];
}


float GetChargeVoltage(void){
	return (float)measGetValue(MEAS_CHG_VOLTAGE) * 0.009833f;
}

float GetBatteryCurrent(void){
	return CurrentmultiMap((float)measGetValue(MEAS_BATT_CURRENT), &BattCurr_CalibIN[0], &BattCurr_CalibOUT[0], 30);
}

float GetLiftedWeightSpd0(void){
  float ACcurrent = (float)escGetAcCurrent();
  return CurrentmultiMap(ACcurrent, &CurrWeight0_IN[0], &CurrWeight0_OUT[0], 2);
}

float GetLiftedWeightSpd1(void){
  float ACcurrent = (float)escGetAcCurrent();
  return CurrentmultiMap(ACcurrent, &CurrWeight1_IN[0], &CurrWeight1_OUT[0], 3);
}

float GetLiftedWeightSpd2(void){
  float ACcurrent = (float)escGetAcCurrent();
  return CurrentmultiMap(ACcurrent, &CurrWeight2_IN[0], &CurrWeight2_OUT[0], 3);
}

int16_t GetBatteryTemp(void){

	return 0;
}