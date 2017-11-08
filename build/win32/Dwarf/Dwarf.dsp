# Microsoft Developer Studio Project File - Name="Dwarf" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Dwarf - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Dwarf.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Dwarf.mak" CFG="Dwarf - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Dwarf - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Dwarf - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Dwarf - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /Ob2 /I "win32" /I ".." /I "..\win32" /I "..\.." /I "..\..\." /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../Dwarf.lib"

!ELSEIF  "$(CFG)" == "Dwarf - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /GR /GX /ZI /Od /I "win32" /I ".." /I "..\win32" /I "..\.." /I "..\..\." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_LIB" /FAs /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../Dwarf.lib"

!ENDIF 

# Begin Target

# Name "Dwarf - Win32 Release"
# Name "Dwarf - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;h"
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\sources\minizip\unzip.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\minizip\unzip.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\zlib.h
# End Source File
# End Group
# Begin Group "ilj"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\sources\win32\ijl.h
# End Source File
# End Group
# Begin Group "libpng"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\sources\libpng\png.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\png.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngasmrd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngerror.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pnggccrd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngget.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngmem.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngpread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngread.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngrio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngrtran.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngrutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngset.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngtest.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngtrans.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngvcrd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngwio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngwrite.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngwtran.c
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\libpng\pngwutil.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\sources\3ds\3ds.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\3ds\3ds.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Array.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Bitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Bitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\BitmapView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Box.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\BSPNode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Buffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Camera.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\CameraView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\ClearView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Codec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Codec3DS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Codec3DS.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Color.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\CompositeDirectory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\CompositeDirectory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\ConsoleView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\ConsoleView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\CullingPortal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\CullingPortal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Defs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Defs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\Directory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\Directory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\DrawFPS.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\DrawImageView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\DrawImageView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\DrawNumberView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\DrawNumberView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Dwarf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Entity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Entity.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Euler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Exception.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Exception.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\File.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\File.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\FileObject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\FileSystem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Font.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Font.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\FractalTreeView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\FractalTreeView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Frustum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Frustum.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\GenericPortal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\GenericPortal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Handle.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\IFFBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\IFFBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\InputHandlers.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\InputReceiver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\InputServer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\InputServer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\InputServerBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\InputServerBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\InStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\JPEGCodec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\JPEGCodec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Shader\Lexer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Line.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\List.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\ListArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Log.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Log.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\LwobCodec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\LwobCodec.h
# End Source File
# Begin Source File

SOURCE="..\..\..\sources\win32\main-Win32.cpp"
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Material.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Material.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Math.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Matrix4x4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Matrix4x4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MD2Codec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MD2Codec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Mesh.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Message.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Message.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MessageFilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MessageFilter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MessageReceiver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MessageReceiver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MirrorMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MirrorPortal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MirrorPortal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\mmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\mmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MorphView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\MorphView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\NoiseView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\NoiseView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\nommgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Object.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Object.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\OpenGLDevice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\OpenGLDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\OpenGLDevice\OpenGLDevice_common.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\OpenGLDevice\OpenGLDevice_common_code.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\OpenGLWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\OpenGLWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\OutStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Pair.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\PakFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\PakFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Shader\Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Plane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\PNGCodec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\PNGCodec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Polygon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Polygon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Portal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Portal.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\QuakeSector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\QuakeSector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Quaternion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Quaternion.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\RawMesh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\RawMesh.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Rect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Rect.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\RenderingDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\ResizeView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\Resource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\Resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Sawteeth\SawteethSound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Scene.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Scene.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Sector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Sector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\Semaphore.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Serializable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Shader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Shader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Shader\ShaderParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\Sound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\Sound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\SoundFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\SoundFile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\SoundPlayer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Sphere.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Stack.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\State.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\StaticMesh.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\String.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\String.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\StringTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\StringTokenizer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\StringView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\TCBSpline.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\TextBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\TextBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\TextOutStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\TextOutStream.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Texture.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Texture.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\TextView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\TextView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\TGACodec.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\TGACodec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\thread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\win32\Timer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Timer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\TunnelView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\TunnelView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Vector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Vertex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\VertexArray.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\View.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\View.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Widget.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Window.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Window.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\WindowBase.h
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\WindowDefs.h
# End Source File
# End Group
# Begin Group "Extern"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=..\..\..\sources\Shader\shaderpp.l
# End Source File
# Begin Source File

SOURCE=..\..\..\sources\Shader\shaderpp.y
# End Source File
# End Target
# End Project
