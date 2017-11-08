#ifndef WAVE_H
#define WAVE_H

#include "types.h"

enum {	HOLD = 0,
		SAW,
		SQR,
		TRI,
		NOS
};


class Wave
{
public:
	Wave(float freq, uint8 waveform);
	Wave();

	// returns if buffer is updated
	bool Next(float *buffer, uint32 count);

	void SetFreq(	float freq);
	void SetPWM(	float p);
	void SetForm(	uint8 waveform);
	void SetAmp(	float a);

	void NoInterp();

private:
	void FillSaw(	float *buffer, uint32 count, float amp);
	void FillSquare(float *buffer, uint32 count, float amp);
	void FillNoise(	float *buffer, uint32 count, float amp);
	void FillTri(	float *buffer, uint32 count, float amp);

	uint8 form;

	float amp,fromamp;

	float pwm;		//	mellan 0 och 1
	float currval;	//	räknare mellan -1 och 1 var i cykeln man är
	
	float curr;
	float step;

	// SQR & TRI-STUFF
	float sqrval;	//	amp eller -amp
	float fstep;	//	2*step || -2*step
	float limit;	//	antingen pwm eller 1

};

#endif
