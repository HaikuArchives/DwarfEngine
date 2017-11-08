/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#define NO_MMGR
#include "../Defs.h"
#include <mmsystem.h>
#ifdef _DEBUG
#include "../mmgr.h"
#else
#include "../nommgr.h"
#endif

#include "../Timer.h"
#include "../Log.h"

using namespace dwarf;


Timer::Timer()
{
	_init_winTimer();

	is_paused = true;
}

Timer::Timer(float t)
{
	_init_winTimer();

	Start(t);
}

Timer::Timer(Timer& t)
{
	_init_winTimer();

	Start(t.Current());
}

Timer::~Timer()
{
}

void Timer::_init_winTimer()
{
	baseTime = 0;
	MMTimerStart = 0;
	performanceTimerEnabled = false;
	performanceTimerStart = 0;
	secsPerTick = 0;

	int64 ticksPerSec = 0;
	// Setup timer, ripped ( ;) ) from chris hargreaves cotc code
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSec))
	{ // no performance counter available
		logger(TEXT("No performance counter, using timeGetTime()."));
		MMTimerStart = ::timeGetTime();
		secsPerTick = 1.0f/1000.0f;
		performanceTimerEnabled = false;
	}
	else
	{ // performance counter is available, use it instead of multimedia timer
		QueryPerformanceCounter((LARGE_INTEGER *)&performanceTimerStart);
		secsPerTick = ((double)1.0)/((double)ticksPerSec);
		performanceTimerEnabled = true;
	}
}

float Timer::AbsTime() const
{
	__int64 time;

	if (performanceTimerEnabled)
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&time);
		return (((float)(time-performanceTimerStart)) * secsPerTick);
	}
	else
	{
		return (((float)(::timeGetTime()-MMTimerStart)) * secsPerTick);
	}
}

void Timer::Start(float start_value)
{
	baseTime = AbsTime() + start_value;
	is_paused = false;
	logger(TEXT("Timer: Started with a start value of %f seconds."), start_value);
}

float Timer::Pause()
{
	if(is_paused)
		return baseTime;
	baseTime = Current();
	is_paused = true;
	logger(TEXT("Timer: Paused at %f seconds."), baseTime);
	return baseTime;
}

void Timer::Resume()
{
	Start(baseTime);
}

float Timer::Current() const
{
	if(is_paused)
		return baseTime;

	return AbsTime() - baseTime;
}
