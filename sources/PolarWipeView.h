#ifndef D_VIEW_H_
#define D_VIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "MessageReceiver.h"
#include "InputReceiver.h"
#include "List.h"
#include "Rect.h"
#include "View.h"
#include "WindowBase.h"

namespace dwarf
{

		class PolarWipeView;

		class PolarWipeView : public View
		{
		friend class WindowBase;
		friend class Window;
		public:
		
			PolarWipeView(Rect frame, float time)
				View(frame),
				m_time(0.0f),
				m_started(false),
				m_max_time(time)
			{
			}

			virtual ~PolarWipeView();

			virtual void Tick(float frame_time)
			{
				if (m_started) m_time += frame_time;
			}

			virtual void Draw(RenderingDevice &device) const
			{
				if (!m_started) return;

				const float fraction = m_time / m_max_time;

				device.Set2DMode(Rect(-1, -1, 2, 2));

				device.Begin(RenderingDevice::TRIANGLE_FAN);
				device.AddVertex(Point(0, 0));
				device.AddVertex(Point(0, 1));

				if (fraction > .125) device.AddVertex(1, 1);
				else
				{
					device.AddVertex(Point(sin(fraction*2_PI), 1));
					
				}

			
				device.End();
			}

		private:

			float m_time;
			bool m_started;
			const float m_max_time;
		};

} // dwarf

#endif // D_VIEW_H_
