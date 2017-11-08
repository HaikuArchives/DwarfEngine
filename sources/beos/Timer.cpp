/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <OS.h>

#include "Defs.h"

#include "Timer.h"

using namespace dwarf;

/*!
	\brief initializes the time to 0 and the Timer to be paused.
*/
Timer::Timer():
	m_paused(true),
	m_start_time(0)
{
}

/*!
	\brief just the destructor
*/
Timer::~Timer()
{
}

/*!
	\brief starts the timer
	\param start_value If you want you can supply a start value, the timer will then start at the
	specified time (given in seconds). (If you call Current() directly after Start() it will probably answer something very near
	start_value)
	\sa Current()
*/
void Timer::Start(float start_value)
{
	m_start_time = system_time() + static_cast<bigtime_t>(start_value * 1000000.0f);
	m_paused = false;
}

/*!
	All calls to Current() will return the same time, until somebody calls Resume()
	\brief Pauses the timer
	\return The current time
	\sa Resume()
*/
float Timer::Pause()
{
	if (m_paused) return m_start_time / 1000000.0f;
	m_paused = true;
	m_start_time = system_time() - m_start_time;
	return m_start_time / 1000000.0f;
}

/*!
	The timer will continue where it was when it was paused.
	\brief Resumes the timer, after a previous call to Pause() (or after the defalt initialization)
	\sa Pause()
*/
void Timer::Resume()
{
	m_start_time += system_time();
	m_paused = false;
}

/*!
	\brief Queries the current time
	\return the current time in seconds
*/
float Timer::Current() const
{
	if(m_paused) return m_start_time / 1000000.0f;
	return (system_time() - m_start_time) / 1000000.0f;
}
