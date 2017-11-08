#ifndef D_DWARF_H_
#define D_DWARF_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 *
 *	Main external library frontend.
 */


/*!
	\mainpage DwarfEngine
	\section DwarfEngine
	\image DwarfLogo.jpeg
*/

#include <list>
#include <queue>
#include <algorithm>
#include <map>

/*!
	\namespace dwarf
	The namespace is used by the library so that class 
	names won't have to be decorated.
	\brief Everything exported by this library will be declared inside
	this namespace.
*/

// Configuration & Debugging
#include "Defs.h"
#include "Exception.h"
#include "Log.h"
// Collections
#include "Array.h"
#include "List.h"
#include "Pair.h"
#include "String.h"
// Data handling
#include "Buffer.h"
// Interface
#include "Window.h"
#include "View.h"
#include "RenderingDevice.h"
#include "Font.h"

// support views
#include "ClearView.h"
#include "TunnelView.h"
#include "NoiseView.h"
#include "DrawImageView.h"
#include "TextView.h"
#include "BitmapView.h"
#include "TextBuffer.h"
#include "MorphView.h"
#include "ConsoleView.h"
#include "StringView.h"
#include "CameraView.h"

#include "Sound.h"
#include "SoundFile.h"
#include "Sawteeth/SawteethSound.h"
#include "SoundPlayer.h"

#include "Codec.h"


#include "Box.h"
#include "Color.h"
#include "Plane.h"
#include "Quaternion.h"
#include "Rect.h"
#include "Vertex.h"
#include "Vector.h"
#include "Line.h"
// space handling
#include "RawMesh.h"
#include "Mesh.h"
#include "Material.h"
#include "Object.h"
#include "Entity.h"
#include "Camera.h"



#include "File.h"
#include "FileSystem.h"
#include "Directory.h"
#include "PakFile.h"
#include "Resource.h"

#include "semaphore.h"
#include "thread.h"


#endif // D_DWARF_H_
