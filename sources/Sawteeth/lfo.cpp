#include "lfo.h"

lfo::lfo()
{
	curr = 0;
	step = 6.28 * .00002267573696145124;
}

lfo::lfo(float freq)
{
	curr = 0;
	step = freq * 6.28 * .00002267573696145124;
}

void lfo::SetFreq(float freq)
{
	step = freq * 6.28 * .00002267573696145124;
}

float lfo::Next()
{
	return cos( curr += step );
}
