#ifndef D_SEMAPHORE_H_
#define D_SEMAPHORE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

namespace dwarf
{

	/*!
		\brief exception baseclass
	*/
	class semaphore
	{
		public:
	
		semaphore(bool allow_nesting = false, bool locked = false):
			m_semaphore(CreateSemaphore(0, locked?0:1, 500, 0)),
			m_thread_id(0),
			m_nesting_depth(0),
			m_allow_nesting(allow_nesting)
		{}
		virtual ~semaphore() { CloseHandle(m_semaphore); }

		/*!
			\brief locks the semaphore
			\param timeout the timeout given in milliseconds, until the method is to give up
			trying to lock the semaphore. If a negative timeout is given it will wait forever, and not
			return until the semaphore could be locked (this is the default parameter)
			\return true if the semaphore was locked and false if it wasn't
		*/
		bool lock(int32 timeout = -1)
		{
			if (!m_allow_nesting) return WaitForSingleObject(m_semaphore, (timeout < 0) ? INFINITE : timeout) != WAIT_FAILED;
			if (m_thread_id == 0)
			{
				bool ret;
				ret = WaitForSingleObject(m_semaphore, (timeout < 0) ? INFINITE : timeout) != WAIT_FAILED;
				if (ret) m_thread_id = GetCurrentThreadId();
				return ret;
			}
			else
			{
				if (m_thread_id == GetCurrentThreadId())
				{
					m_nesting_depth++;
					return true;
				}
				else return WaitForSingleObject(m_semaphore, (timeout < 0) ? INFINITE : timeout) != WAIT_FAILED;
			}
		}
		/*!
			\brief unlocks the semaphore, making it available for somebody else to lock.
		*/			
		void unlock()
		{
			if (!m_allow_nesting)
				ReleaseSemaphore(m_semaphore, 1, 0);
			else if (m_nesting_depth == 0)
			{
				ReleaseSemaphore(m_semaphore, 1, 0);
				m_thread_id = 0;
			}
			else m_nesting_depth--;
		}

	private:
			
		HANDLE m_semaphore;		// the semaphore
		DWORD m_thread_id;		// id to the holding thread, or -1
		int32 m_nesting_depth;	// is increased if the holding thread locks the semaphore again
		bool m_allow_nesting;	// false if nesting is not allowed

	};

} // dwarf

#endif // D_SEMAPHORE_H_
