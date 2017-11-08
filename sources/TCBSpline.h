#ifndef D_TCBSPLINE_H
#define D_TCBSPLINE_H

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "List.h"
#include "Log.h"

namespace dwarf
{
	template<class T> class TCBSpline
	{
	public:
		TCBSpline() : m_key_list() {};
		virtual ~TCBSpline() {};

		void calculate_tcb();

		// add and delete is at end of array
		// and time is not deltatime, but has to be
		// a time larger than the time that the
		// key before the added one has.
		void add_key(const T &val, float tension,float continuity,float bias,float time,bool linear = false)
		{
			Key key;

			key.val = val;
			key.tension = tension;
			key.continuity = continuity;
			key.bias = bias;
			key.linear = linear;
			key.time = time;
			m_key_list.push_back(key);
		}


		// deletes the last key
		void del_key();

	private:

		class Key
		{
			public:
			T val;
			T incomming_tangent;
			T outgoing_tangent;
			float tension;
			float continuity;
			float bias;
			bool linear;
			float time;
		};

	public:

		class const_iterator
		{
		public:
			const_iterator &operator=(const const_iterator &i)
			{
				m_iterator = i.m_iterator;
				m_start = i.m_start;
				m_current_time = i.m_current_time;
				m_before = i.m_before;
				m_after = i.m_after;
				m_spline = i.m_spline;

				return *this;
			}

			void step(float delta_time)
			{
				// increase the time
				m_current_time += delta_time;
				// if there are no more keys
				while (m_current_time > m_after.time)
				{
					m_before = m_after;
					if (m_iterator != m_spline->end())
					{
						++m_iterator;
						if (m_iterator != m_spline->end()) m_after = *m_iterator;
						else m_after.time = m_current_time;
					}
					else m_after.time = m_current_time;
				}
			}

			const T value()
			{
				// t: [0, 1]
				float t;
				if (m_after.time == m_before.time) t = 1.0;
				else t = (m_current_time - m_before.time) / (m_after.time - m_before.time);

				// Hermit coef's

				float t2 = t*t;
				float t3 = t2*t;

				/*float h1 =  2*t3 - 3*t2 + 1;
				float h2 = -2*t3 + 3*t2;
				float h3 =   t3 - 2*t2 + t;
				float h4 =   t3 -  t2;*/

				float h1,h2,h3,h4;

				h2 = 3.0f * t2 - t3 - t3;
				h1 = 1.0f - h2;
				h4 = t3 - t2;
				h3 = h4 - t2 + t;


				//T ret = m_before.val * t + m_after.val * (1 - t);

				T ret;

				if(m_after.linear)
				{
					ret =	m_before.val + (m_after.val - m_before.val) * t;
				}
				else
				{
					ret =	m_before.val * h1 +
							m_after.val * h2 +
							m_before.outgoing_tangent * h3 +
							m_after.incomming_tangent * h4;
				}

				return ret;
			}

			const_iterator(const std::list<Key> &spline, const std::list<Key>::const_iterator &iterator):
				m_iterator(iterator),
				m_start(iterator),
				m_spline(&spline),
				m_current_time(0.0)
			{
				// if the iterator is totally empty
				if (m_iterator == m_spline->end())
				{
					m_after.time = 1.0;
					m_before = m_after;
					return;
				}

				m_before = *m_iterator;
				m_iterator++;
				if (m_iterator == m_spline->end())
				{
					m_iterator = m_start;
					m_after = m_before;
					return;
				}
				m_after = *m_iterator;
			}

			const_iterator() {};
			float time() { return m_current_time; };

		private:

			std::list<Key>::const_iterator m_iterator;
			std::list<Key>::const_iterator m_start;
			const std::list<Key> *m_spline;
			float m_current_time;
			Key m_before;
			Key m_after;
		};


		TCBSpline<T>::const_iterator begin() const { return const_iterator(m_key_list, m_key_list.begin()); };

	private:
		std::list<Key> m_key_list;
	};

	template<class T>
	inline void TCBSpline<T>::calculate_tcb()
	{
		Key *cur,*prev = 0,*prev2 = prev;
		
		for(std::list<Key>::iterator i = m_key_list.begin(); i != m_key_list.end(); i++)
		{
			cur = &(*i);
			
			if(prev)
			{
				float c0,c1;
				T normal = cur->val - prev->val;
		
				c0 = ((1.0 - cur->tension) * (1.0 - cur->continuity) * (1.0 + cur->bias))/2;
				c1 = ((1.0 - cur->tension) * (1.0 + cur->continuity) * (1.0 - cur->bias))/2;
				
				if(cur == &m_key_list.back())
				{
					cur->incomming_tangent = normal * 0.5 * (c0 + c1);
				}
				else
				{
					std::list<Key>::iterator j = i;
					Key *next = &(*(++j));
					
					cur->incomming_tangent = (cur->val - prev->val) * c0 + (next->val - cur->val) * c1;
				}

				
				// Outgoing tangent
				
				/*c0 = (1.0 - prev->tension) * (1.0 + prev->continuity) * (1.0 + prev->bias);
				c1 = (1.0 - prev->tension) * (1.0 - prev->continuity) * (1.0 - prev->bias);
				
				  if(!prev2)
				  {
				  Log(TEXT("First"));
				  prev->outgoing_tangent = normal * 0.5 * (c0 + c1);
				  }
				  else
				  {
				  float adjust = length / (cur->time - prev2->time);
				  
					prev->outgoing_tangent = (normal * c1 + (prev->val - prev2->val) * c0) * adjust;
			}*/
				
				c0 = ((1.0 - prev->tension) * (1.0 + prev->continuity) * (1.0 + prev->bias))/2;
				c1 = ((1.0 - prev->tension) * (1.0 - prev->continuity) * (1.0 - prev->bias))/2;
				
				if(!prev2)
				{
					prev->outgoing_tangent = normal * 0.5 * (c0 + c1);
				}
				else
				{
					prev->outgoing_tangent = (prev->val - prev2->val) * c0 + (cur->val - prev->val) * c1;
				}
			}
			prev2 = prev;
			prev = cur;
		}
	}

} // dwarf

#endif