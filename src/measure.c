#include "ch.h"
#include "hal.h"
#include "measure.h"

static uint16_t measValue[MEAS_NUM_CH];
static adcsample_t samples[MEAS_NUM_CH * ADC_GRP1_BUF_DEPTH];

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

    adcConvert(&ADCD1, &adcgrpcfg, &samples, ADC_GRP1_BUF_DEPTH);

    for(ch = 0; ch < MEAS_NUM_CH; ch++) {
      avg = 0;
      for(i = 0; i < ADC_GRP1_BUF_DEPTH; i++) {
        avg += samples[ch + MEAS_NUM_CH * i];
      }
      avg /= ADC_GRP1_BUF_DEPTH;

      measValue[ch] = avg;
    }


    chThdSleepUntil(time);
  }
}

int16_t measGetValue(enum measChannels ch){
      return measValue[ch];
}

void InitMeasures(void){
  adcStart(&ADCD1, NULL);
  chThdCreateStatic(waSampleThread, sizeof(waSampleThread), NORMALPRIO, SampleThread, NULL);

}