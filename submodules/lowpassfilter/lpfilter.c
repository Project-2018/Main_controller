#include "lpfilter.h"
#include "math.h"

#ifndef M_PI_F
#define M_PI_F 3.14159f
#endif


void LP_set_cutoff_frequency(LPfilter_t* lpfd, float sample_freq, float cutoff_freq)
{
	lpfd->cutoff_freq = cutoff_freq;

	if (lpfd->cutoff_freq <= 0.0f) {
		lpfd->LPfilterState = LP_STOP;
		return;
	}

	float fr = sample_freq / cutoff_freq;
	float ohm = tanf(M_PI_F / fr);
	float c = 1.0f + 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm;
	lpfd->b0 = ohm * ohm / c;
	lpfd->b1 = 2.0f * lpfd->b0;
	lpfd->b2 = lpfd->b0;
	lpfd->a1 = 2.0f * (ohm * ohm - 1.0f) / c;
	lpfd->a2 = (1.0f - 2.0f * cosf(M_PI_F / 4.0f) * ohm + ohm * ohm) / c;

	lpfd->LPfilterState = LP_ACTIVE;
}

float LPApply(LPfilter_t* lpfd, float sample)
{
	if (lpfd->cutoff_freq <= 0.0f || lpfd->LPfilterState != LP_ACTIVE) {
		// no filtering
		return sample;
	}

	// do the filtering
	float delay_element_0 = sample - lpfd->delay_element_1 * lpfd->a1 - lpfd->delay_element_2 * lpfd->a2;

	if (!isfinite(delay_element_0)) {
		// don't allow bad values to propagate via the filter
		delay_element_0 = sample;
	}

	float output = delay_element_0 * lpfd->b0 + lpfd->delay_element_1 * lpfd->b1 + lpfd->delay_element_2 * lpfd->b2;

	lpfd->delay_element_2 = lpfd->delay_element_1;
	lpfd->delay_element_1 = delay_element_0;

	// return the value.  Should be no need to check limits
	return output;
}

float LPReset(LPfilter_t* lpfd, float sample)
{
	float dval = sample / (lpfd->b0 + lpfd->b1 + lpfd->b2);
	lpfd->delay_element_1 = dval;
	lpfd->delay_element_2 = dval;
	return apply(sample);
}