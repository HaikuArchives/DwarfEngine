#include <stdio.h>
#include "wave.h"

#define ABS(x) ((x)>0)?(x):(-x)
/////////////////////////////////////////
//	prototypes
unsigned long jng_rand();
/////////////////////////////////////////
//	own random-function
#define BIGMOD 0x7fffffff
#define W 127773
#define C 2836
long jng_seed=1;

unsigned long jng_rand()
{
	jng_seed = 16807*(jng_seed % W) - (jng_seed/W)*C;
	if (jng_seed < 0) jng_seed+=BIGMOD;
	return jng_seed;
}



Wave::Wave(float freq, uint8 waveform)
{
	fromamp = curr = pwm = step = sqrval = currval = 0.0;
	limit = 1.0;
	SetForm(waveform);
	SetFreq(freq);
}

Wave::Wave()
{
	fromamp = curr = pwm = step = sqrval = currval = 0.0;
	limit = 1.0;

	SetForm(SAW);
	SetFreq(440);
}


void Wave::SetFreq(float freq)
{
	step = freq * .00002267573696145124;	//	skall bort!
	if (step > 1.9){
//		fprintf(stderr,"to high freq=%f\n",freq);
		step = 1.9;
	}
}

const float pwmlim = 0.9;
void Wave::SetPWM(float p)
{
   //	fprintf(stderr,"(Wave)pwm = %f\n",p);
   if (p > pwmlim)
         pwm = pwmlim;
   else
	   if (p < -pwmlim)
	         pwm = -pwmlim;
		else
		      pwm = p;
		    
//fprintf(stderr,"(Wave)pwm = %f\n",pwm);
}

void Wave::SetAmp(float a)
{
	amp = a;
}

void Wave::SetForm(uint8 waveform){
//	fprintf(stderr,"(Wave)form = %d\n",waveform);
	if (currval > 2) currval = 0.0;// limit
	form = waveform;
}

void Wave::NoInterp()
{
	fromamp = amp;
}

bool Wave::Next(float *buffer, uint32 count)
{
//	fprintf(stderr, "\nWave::Next(%p,%ld)\n",buffer,count);
	if (amp < 0.001) return false;

	switch (form){
	case SAW:	FillSaw(buffer, count, amp); break;
	case SQR:	FillSquare(buffer, count, amp); break;
	case NOS:	FillNoise(buffer, count, amp); break;
	case TRI:	FillTri(buffer, count, amp); break;
	default: return false;
	}

//	fprintf(stderr,"(Wave)%p\tform=%d\tcval=%f\tcurr = %f\n",this,form,currval,curr);
return true;
}

inline void Wave::FillSaw(float *b, uint32 count, float amp)
{
	float astep = (amp-fromamp)/(float)count;
	amp = fromamp;
	
	float limit = 1.0-(step/2);
	while (count --){
		currval += step;
		if (currval>limit){
		// aliasing noise-killing coming up
			float in = currval-step;
			float red = (currval-limit) / step;
//			if (red>1.0) red = 1.0;

			currval-=2.0; // currval är ut
			*b = amp * ( red*currval + (1-red)*in);
		}else
			*b = amp * currval;
		b++;
		amp+=astep;
	}
	fromamp = amp;
}

inline void Wave::FillSquare(float *b, uint32 count, float amp)
{
/*	float *stop = b + count;
	while (b < stop){
		*b = (pwm < currval)?(-1):(1); // pulse
		currval += step;
		if (currval>1.0) currval-=2.0;
		b++;
	};
	return;
*/
	float astep = (amp-fromamp)/(float)count;
	amp = fromamp;

	if (limit != 1.0){
		sqrval = -amp;
		limit = pwm;
	}else
		sqrval = amp;

	float *stop = b + count;
	while (b < stop){
		currval += step;
		if (currval > limit){
		// aliasing noise-killing coming up
			float red = (currval-limit) / step;
			if (red>1.0) red = 1.0;

			*b = amp * ( red*(-sqrval) + (1-red)*sqrval);
//if (*b > 1.0 ) fprintf(stderr,"*b=%f, red=%f, cv=%f, lim=%f, step=%f\n",*b,red,currval,limit, step);
			sqrval = -sqrval;
			if (limit == pwm){
				limit = 1.0;
			}else{
				limit = pwm;
				currval -= 2.0;
			}
		}else
		*b = amp * sqrval; // pulse
		b++;
		amp+=astep;
	}
	fromamp = amp;
}

inline void Wave::FillNoise(float *b, uint32 count, float amp)
{
	float astep = (amp-fromamp)/(float)count;
	amp = fromamp;

	float *stop = b + count;
	while (b < stop){
		currval += step;
		if (currval>1.0){
			curr = amp * ((jng_rand()/(64.0*256.0*256.0*256.0))-1);
			//if (curr > 1.0) fprintf(stderr,"%f step=(%f)\n",curr,step);
			currval-=2.0;
		}
		*b = curr;
		b++;
		amp+=astep;
	}
	fromamp = amp;
}

inline void Wave::FillTri(float *b, uint32 count, float amp)
{
/*	OLD CODE WORKING (ANTIALIAS)
	float *stop = b + count;
	while (b < stop){
		*b = amp * (2 * ((currval> 0 )?(-currval):(currval))+1.0);
		currval += step;
		if (currval>1.0) currval-=2.0;
		b++;
	}
*/
	float astep = (amp-fromamp)/(float)count;
	amp = fromamp;
	
	float *stop = b + count;
	while (b < stop){
		*b = amp * (2 * ((currval> 0 )?(-currval):(currval))+1.0);
//if (*b > 1.0 ) fprintf(stderr,"TRI *b=%f, cv=%f, lim=%f, step=%f\n",*b,currval,limit, step);
		currval += step;
		if (currval>1.0) currval-=2.0;
		b++;

		amp+=astep;
	}
	fromamp = amp;
}
