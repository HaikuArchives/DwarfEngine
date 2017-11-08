#ifndef _ST_LFO_
#define _ST_LFO_

#include <math.h>

class lfo {
public:
	lfo();
	lfo(float freq);
	void SetFreq(float freq);
	float Next();
private:
	float curr,step;	
};

#endif