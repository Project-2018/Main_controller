#include "ch.h"
#include "hal.h"
#include "measure.h"

#include "math.h"


static uint16_t measValue[MEAS_NUM_CH +2];
static adcsample_t samples[MEAS_NUM_CH * ADC_GRP1_BUF_DEPTH];


#ifndef M_PI_F
#define M_PI_F 3.14159f
#endif

float           curr_cutoff_freq;
float           curr_a1 = 0.0;
float           curr_a2 = 0.0;
float           curr_b0 = 0.0;
float           curr_b1 = 0.0;
float           curr_b2 = 0.0;
float           curr_delay_element_1 = 0.0;        // buffered sample -1
float           curr_delay_element_2 = 0.0;        // buffered sample -2

void CurrLP_set_cutoff_frequency(float sample_freq, float cutoff_freq)
{
	curr_cutoff_freq = cutoff_freq;

	if (curr_cutoff_freq <= 0.0f) {
		// no filtering
		return;
	}

	float fr = sample_freq / curr_cutoff_freq;
	float ohm = tanf(M_PI_F / fr);
	float c = 1.0f + 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm;
	curr_b0 = ohm * ohm / c;
	curr_b1 = 2.0f * curr_b0;
	curr_b2 = curr_b0;
	curr_a1 = 2.0f * (ohm * ohm - 1.0f) / c;
	curr_a2 = (1.0f - 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm) / c;
}

float CurrLPApply(float sample)
{
	if (curr_cutoff_freq <= 0.0f) {
		// no filtering
		return sample;
	}

	// do the filtering
	float delay_element_0 = sample - curr_delay_element_1 * curr_a1 - curr_delay_element_2 * curr_a2;

	if (!isfinite(delay_element_0)) {
		// don't allow bad values to propagate via the filter
		delay_element_0 = sample;
	}

	float output = delay_element_0 * curr_b0 + curr_delay_element_1 * curr_b1 + curr_delay_element_2 * curr_b2;

	curr_delay_element_2 = curr_delay_element_1;
	curr_delay_element_1 = delay_element_0;

	// return the value.  Should be no need to check limits
	return output;
}

float CurrLPReset(float sample)
{
	float dval = sample / (curr_b0 + curr_b1 + curr_b2);
	curr_delay_element_1 = dval;
	curr_delay_element_2 = dval;
	return apply(sample);
}




static const ADCConversionGroup adcgrpcfg = {
  FALSE,
  MEAS_NUM_CH,
  NULL,
  NULL,
  0,                                                                       /* CR1 */
  ADC_CR2_SWSTART,                                                         /* CR2 */
  ADC_SMPR1_SMP_AN12(ADC_SAMPLE_15) |
  ADC_SMPR1_SMP_AN10(ADC_SAMPLE_15) | ADC_SMPR1_SMP_AN11(ADC_SAMPLE_15),
  NULL,
  ADC_SQR1_NUM_CH(MEAS_NUM_CH),
  NULL,
  ADC_SQR3_SQ3_N(ADC_CHANNEL_IN12) |
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN10) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11)
};


static THD_WORKING_AREA(waSampleThread, 512);
static THD_FUNCTION(SampleThread, arg) {
  chRegSetThreadName("SampleThread");
  systime_t time;
  time = chVTGetSystemTime();
  while(true)
  {
    time += TIME_MS2I(SAMPLE_FREQUENCY_MS);

    uint16_t avg, ch, i;

    adcConvert(&ADCD1, &adcgrpcfg, samples, ADC_GRP1_BUF_DEPTH);

    for(ch = 0; ch < MEAS_NUM_CH; ch++) {
      avg = 0;
      for(i = 0; i < ADC_GRP1_BUF_DEPTH; i++) {
        avg += samples[ch + MEAS_NUM_CH * i];
      }
      avg /= ADC_GRP1_BUF_DEPTH;

      if(ch == MEAS_BATT_CURRENT){
      	avg = CurrLPApply((float)avg);
      }

      //chSysLock();
      measValue[ch] = avg;
      //chSysUnlock();
    }


    chThdSleepUntil(time);
  }
}

int16_t measGetValue(enum measChannels ch){
      return measValue[ch];
}

void InitMeasures(void){
  CurrLP_set_cutoff_frequency(100, 5);
  adcStart(&ADCD1, NULL);
  chThdCreateStatic(waSampleThread, sizeof(waSampleThread), NORMALPRIO, SampleThread, NULL);

}