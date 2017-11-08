#ifndef D_THREAD_H_
#define D_THREAD_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "nommgr.h"
#include <queue>


#include "Defs.h"
#include "Math.h"

#include <WinBase.h>

namespace dwarf
{

	template <class P> class thread
	{
	public:

		enum Priority
		{
			HIGH = THREAD_PRIORITY_ABOVE_NORMAL,
			NORMAL = THREAD_PRIORITY_NORMAL,
			LOW = THREAD_PRIORITY_BELOW_NORMAL
		};

		thread(const P &param, const String &name, Priority priority = NORMAL);
		virtual ~thread();

		int32 wait() const;
		void resume();
		void suspend();

		void send(const Buffer &data) { m_queue.push(data); }

	protected:

		bool has_data() { return m_queue.size() != 0; };
		Buffer receive() { Buffer &buf = m_queue.front(); m_queue.pop(); return buf; }

		virtual int32 run(const P &param) = 0;

	private:

		static unsigned long thread_func(void *param) { return static_cast<thread*>(param)->run(static_cast<thread*>(param)->m_param); }

		HANDLE m_thread;
		const P &m_param;
		std::queue<Buffer> m_queue;

	};

	template <class P>
	thread<P>::thread(const P &param, const String &name, Priority priority):
		m_param(param)
	{
		m_thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)thread_func, this, CREATE_SUSPENDED, 0);
//		if (m_thread == 0) throw 

		SetThreadPriority(m_thread, priority);

	}

	template<class P> thread<P>::~thread()
	{
		unsigned long ret;
		TerminateThread(m_thread, ret);
	}

	template <class P>
	inline void thread<P>::suspend()
	{
		SuspendThread(m_thread);
	}

	template <class P>
	inline void thread<P>::resume()
	{
		ResumeThread(m_thread);
	}

	template <class P>
	inline int32 thread<P>::wait() const
	{
		unsigned long ret;

		do
		{
			GetExitCodeThread(m_thread, &ret);
			Sleep(10);
		} while (ret == STILL_ACTIVE);
		return ret;
	}
	

} // dwarf

#endif // D_THREAD_H_
