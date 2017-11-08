#ifndef D_SCENEVIEW_H_
#define D_SCENEVIEW_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "View.h"

namespace dwarf
{
	class SceneView
	{
	public:
		SceneView();
		~SceneView();

		void Tick(float frame_time);
		void Draw(RenderingDevice const& device) const;
	};
}

#endif // D_SCENEVIEW_H_