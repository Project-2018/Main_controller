#ifndef MEASURE_H_INCLUDED
#define MEASURE_H_INCLUDED


#define SAMPLE_FREQUENCY_MS		100
#define ADC_GRP1_BUF_DEPTH      16

enum measChannels
{
	MEAS_CHG_VOLTAGE,
	MEAS_BATT_CURRENT,
	MEAS_BATT_TEMP,
	MEAS_NUM_CH
};

int16_t measGetValue(enum measChannels ch);
void InitMeasures(void);

#endif