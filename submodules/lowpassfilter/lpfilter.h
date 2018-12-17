#ifndef LPFILTER_H_
#define LPFILTER_H_


typedef enum{
	LP_UNINIT	= 0,
	LP_ACTIVE	= 1,
	LP_STOP		= 2
} LPfilterInit_t;

typedef struct {
	LPfilterInit_t	LPfilterState;
	float           cutoff_freq;
	float           a1;
	float           a2;
	float           b0;
	float           b1;
	float           b2;
	float           delay_element_1;        // buffered sample -1
	float           delay_element_2;        // buffered sample -2
} LPfilter_t;

void LP_set_cutoff_frequency(LPfilter_t* lpfd, float sample_freq, float cutoff_freq);
float LPApply(LPfilter_t* lpfd, float sample);
float LPReset(LPfilter_t* lpfd, float sample);


#endif