#ifndef D_TIMER_H_
#define D_TIMER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Math.h"

namespace dwarf
{

		/*!
			You can Start() it, Pause() it and Resume() it. If you want the current
			time, just call Current().
			It also supports some operators, so you can do something like this:
			
			static Timer mytime = 3.0f; // begin counting from 3
			if(mytime > 3.0f && mytime < 4.0f) mytime = 4.0f; // hop over 3 and 4

			\brief A timer class that works much like a stop watch
		*/

		class Timer
		{
		public:
			Timer();
			~Timer();

			void Start(float start_value = 0.0f);
			float Pause();
			void Resume();

			float Current() const;
			

			// operators and convenience functions

			Timer(float t);
			Timer(Timer& t);

			const Timer &operator=(const Timer &t)	{ Start(t.Current()); return *this; }
			const Timer &operator=(float t)			{ Start(t); return *this; }

			bool operator==(float val) const		{ return /*t_*/fabs(Current() - val) < D_EPSILON2; }
			bool operator!=(float val) const		{ return /*t_*/fabs(Current() - val) > D_EPSILON2; }
			bool operator <(float val) const		{ return Current() < val; }
			bool operator >(float val) const		{ return Current() > val; }
			bool operator<=(float val) const		{ return Current() <= val; }
			bool operator>=(float val) const		{ return Current() >= val; }

			bool operator==(const Timer& t) const	{ return /*t_*/fabs(Current() - t.Current()) < D_EPSILON2; }
			bool operator!=(const Timer& t) const	{ return /*t_*/fabs(Current() - t.Current()) > D_EPSILON2; }
			bool operator <(const Timer& t) const	{ return Current()  < t.Current(); }
			bool operator >(const Timer& t) const	{ return Current()  > t.Current(); }
			bool operator<=(const Timer& t) const	{ return Current() <= t.Current(); }
			bool operator>=(const Timer& t) const	{ return Current() >= t.Current(); }

			operator float () const					{ return Current(); }

		private:

		#ifdef WIN32
			void _init_winTimer();
			float AbsTime() const;
			double	secsPerTick;
			bool	performanceTimerEnabled;
			int64	performanceTimerStart;
			ULONG	MMTimerStart;
			float	baseTime;
			bool is_paused;
		#endif

		#ifdef __BEOS__
			bool m_paused;
			bigtime_t m_start_time;
		#endif
		};

}

#endif // D_TIMER_H_
