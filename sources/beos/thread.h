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
#include <OS.h>

#include "Defs.h"

#include "Math.h"
#include "Buffer.h"

namespace dwarf
{

	template <class P> class thread
	{
	public:

		enum Priority
		{
			HIGH = B_URGENT_DISPLAY_PRIORITY,
			NORMAL = B_NORMAL_PRIORITY,
			LOW = B_LOW_PRIORITY
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

		static int32 thread_func(void *param) { return static_cast<thread*>(param)->run(static_cast<thread*>(param)->m_param); }

		thread_id m_thread;
		const P &m_param;
		std::queue<Buffer> m_queue;

	};

	template <class P>
	thread<P>::thread(const P &param, const String &name, Priority priority):
		m_param(param)
	{
		char *buffer[B_OS_NAME_LENGTH];
		name.ToChar(buffer, B_OS_NAME_LENGTH);
		
		m_thread = spawn_thread(thread_func, buffer, priority, this);
	}

	template<class P> thread<P>::~thread()
	{
		kill_thread(m_thread);
	}

	template <class P>
	inline void thread<P>::suspend()
	{
		suspend_thread(m_thread);
	}

	template <class P>
	inline void thread<P>::resume()
	{
		resume_thread(m_thread);
	}

	template <class P>
	inline int32 thread<P>::wait() const
	{
		unsigned long ret;
		wait_for_thread(m_thread, ret);
		return ret;
	}
	

} // dwarf

#endif // D_THREAD_H_
