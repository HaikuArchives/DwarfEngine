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

		semaphore(bool allow_nesting = false, bool locked = false);
		virtual ~semaphore();

		bool lock(int32 timeout = -1);
		void unlock();

	private:
			
		sem_id m_semaphore;
		thread_id m_thread_id;
		int32 m_nesting_depth;
		bool m_allow_nesting;

	};

} // dwarf

#endif // D_SEMAPHORE_H_
