#ifndef D_CAMERAVIEW_H_
#define D_CAMERAVIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "View.h"
#include "Rect.h"
#include "Camera.h"
#include "Plane.h"

namespace dwarf
{

	class Object;
	class RenderingDevice;

	/*!
		\brief A view that shows what a camera sees
	*/
	
	class CameraView : public View
	{
	public:
	
		/*!
			\brief Initializes the view with the given size and to look through the given camera
			\param frame the size of teh view
			\param camera the camera to look though
			\warning you may not delete the camera object while this view-object exists
		*/
		CameraView(Rect frame, int32 resize_mode, const Camera &camera):
			View(frame, resize_mode, View::SPACE_3D),
			m_camera(camera)
		{}

		virtual void Draw(RenderingDevice &device) const
		{
			m_camera.Draw(device);
		}

	private:


		const Camera &m_camera;

	};

		
} // dwarf

#endif // D_CAMERAVIEW_H_
