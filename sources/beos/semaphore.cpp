/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "semaphore.h"

semaphore::semaphore(bool allow_nesting, bool locked):
	m_semaphore(create_sem(locked?0:1, "generic Semaphore instance")),
	m_thread_id(-1),
	m_nesting_depth(0),
	m_allow_nesting(allow_nesting)
{
}

semaphore::~semaphore()
{
	delete_sem(m_semaphore);
}

/*!
	\brief locks the semaphore
	\param timeout the timeout given in milliseconds, until the method is to give up
	trying to lock the semaphore. If a negative timeout is given it will wait forever, and not
	return until the semaphore could be locked (this is the default parameter)
	\return true if the semaphore was locked and false if it wasn't
*/
bool semaphore::lock(int32 timeout)
{
	if (!m_allow_nesting)
		if (timeout < 0)
			return acquire_sem(m_semaphore) == B_OK;
		else
			return acquire_sem_etc(m_semaphore, 1, 0, timeout/1000) == B_OK;

	if (m_thread_id == -1)
	{
		bool ret;
		if (timeout < 0)
			ret = acquire_sem(m_semaphore) == B_OK;
		else
			ret = acquire_sem_etc(m_semaphore, 1, 0, timeout/1000) == B_OK;
		if (ret) m_thread_id = find_thread(0);
		return ret;
	}
	else
	{
		if (m_thread_id == find_thread(0))
		{
			m_nesting_depth++;
			return true;
		}
		else if (timeout < 0)
			return acquire_sem(m_semaphore) == B_OK;
		else
			return acquire_sem_etc(m_semaphore, 1, 0, timeout/1000) == B_OK;
		}
		if (timeout < 0) return acquire_sem(m_semaphore) == B_OK; else return acquire_sem_etc(m_semaphore, 1, 0, timeout/1000) == B_OK;
}


/*!
	\brief unlocks the semaphore, making it available for somebody else to lock.
*/			
void semaphore::unlock()
{
	if (!m_allow_nesting)
		release_sem(m_semaphore);
	else if (m_nesting_depth == 0)
	{
		release_sem(m_semaphore);
		m_thread_id = -1;
	}
	else m_nesting_depth--;
}
