
/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include <UTF8.h>
#include <String.h>
#include <Screen.h>
#include <View.h>
#include <GLView.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Rect.h>

#include "OpenGLDevice.h"
#include "Plane.h"
#include "Vertex.h"
#include "Quaternion.h"
#include "Rect.h"
#include "Color.h"
#include "Window.h"
#include "Array.h"
#include "InputServer.h"
#include "InputServerBase.h"
#include "Matrix4x4.h"
#include "OpenGLWindow.h"
#include "VertexArray.h"

using namespace dwarf;

/********************************************************************/
// OpenGLDevice

#include "../OpenGLDevice/OpenGLDevice_common_code.h"

OpenGLDevice::OpenGLDevice(Window *window, const Rect &frame, uint32 flags, uint32 options, BView *view):
	m_window(window, this, frame, flags, options, view)
{
	logger(TEXT("Creating default texture\n"));

	char data[] = {0, 255, 255, 0};

	Bitmap null_bitmap(2, 2, Bitmap::GRAYSCALE);
	memcpy(null_bitmap.Data(), data, 4);
	
	Lock();
	m_null_texture = HTexture(null_bitmap, Texture::NO_MIPMAPS);
	Unlock();

	// Start timer
	m_timer.Start();

	#ifdef _DEBUG
	m_locked = false;
	m_in_begin = false;
	#endif

// initialize the current state
	uint32 states = 0;
	GLboolean enabled;
//	GLint state;

//	alpha test
	glGetBooleanv(GL_ALPHA_TEST, &enabled);
	if (enabled) states |= State::ALPHA_TEST;
//	auto normal
	glGetBooleanv(GL_AUTO_NORMAL, &enabled);
	if (enabled) states |= State::AUTO_NORMAL;
//	blend
	glGetBooleanv(GL_BLEND, &enabled);
	if (enabled) states |= State::BLEND;
//	color material
	glGetBooleanv(GL_COLOR_MATERIAL, &enabled);
	if (enabled) states |= State::COLOR_MATERIAL;
//	face culling
	glGetBooleanv(GL_CULL_FACE, &enabled);
	if (enabled) states |= State::CULL_FACE;
//	depth test
	glGetBooleanv(GL_DEPTH_TEST, &enabled);
	if (enabled) states |= State::DEPTH_TEST;
//	dither
	glGetBooleanv(GL_DITHER, &enabled);
	if (enabled) states |= State::DITHER;
//	fog
	glGetBooleanv(GL_FOG, &enabled);
	if (enabled) states |= State::FOG;

//	lighting
	glGetBooleanv(GL_LIGHTING, &enabled);
	if (enabled) states |= State::LIGHTING;
//	line smooth
	glGetBooleanv(GL_LINE_SMOOTH, &enabled);
	if (enabled) states |= State::LINE_SMOOTH;
//	line stipple
	glGetBooleanv(GL_LINE_STIPPLE, &enabled);
	if (enabled) states |= State::LINE_STIPPLE;
//	logic op
	glGetBooleanv(GL_LOGIC_OP, &enabled);
	if (enabled) states |= State::LOGIC_OP;
//	point smooth
	glGetBooleanv(GL_POINT_SMOOTH, &enabled);
	if (enabled) states |= State::POINT_SMOOTH;
//	polygon smooth
	glGetBooleanv(GL_POLYGON_SMOOTH, &enabled);
	if (enabled) states |= State::POLYGON_SMOOTH;
//	polygon stipple
	glGetBooleanv(GL_POLYGON_STIPPLE, &enabled);
	if (enabled) states |= State::POLYGON_STIPPLE;
//	scissor test
	glGetBooleanv(GL_SCISSOR_TEST, &enabled);
	if (enabled) states |= State::SCISSOR_TEST;
//	stencil test
	glGetBooleanv(GL_STENCIL_TEST, &enabled);
	if (enabled) states |= State::STENCIL_TEST;
//	texture 1D
	glGetBooleanv(GL_TEXTURE_1D, &enabled);
	if (enabled) states |= State::TEXTURE_1D;
//	texture 2D
	glGetBooleanv(GL_TEXTURE_2D, &enabled);
	if (enabled) states |= State::TEXTURE_2D;
/*
//	shade model
	glGetIntegerv(GL_SHADE_MODEL, &state);
	if (state == GL_SMOOTH) states |= U_SMOOTH_SHADE_MODEL;

//	destination blend mode
	glGetIntegerv(GL_BLEND_DST, &state);
	m_destination_blend = state;

//	source blend mode
	glGetIntegerv(GL_BLEND_SRC, &state);
	m_source_blend = state;

//	fillmode
	glGetIntegerv(GL_BLEND_SRC, &state);
	m_source_blend = state;
	

//	face culling mode
	glGetIntegerv(GL_CULL_FACE_MODE, &state);
	m_cullmode = state;
*/



	m_current_state.Enable(states);



/*
	glGetPolygonStripple(m_polygon_stipple);

//	light
	glGetBooleanv(GL_LIGHT0, &enabled);
	if (enabled) states |= U_LIGHT0;
	glGetBooleanv(GL_LIGHT1, &enabled);
	if (enabled) states |= U_LIGHT1;
	glGetBooleanv(GL_LIGHT2, &enabled);
	if (enabled) states |= U_LIGHT2;
	glGetBooleanv(GL_LIGHT3, &enabled);
	if (enabled) states |= U_LIGHT3;
	glGetBooleanv(GL_LIGHT4, &enabled);
	if (enabled) states |= U_LIGHT4;
	glGetBooleanv(GL_LIGHT5, &enabled);
	if (enabled) states |= U_LIGHT5;
	glGetBooleanv(GL_LIGHT6, &enabled);
	if (enabled) states |= U_LIGHT6;
	glGetBooleanv(GL_LIGHT7, &enabled);
	if (enabled) states |= U_LIGHT7;

//	the clip planes
	glGetBooleanv(GL_CLIP_PLANE0, &enabled);
	if (enabled) states |= U_CLIP_PLANE0;
	glGetBooleanv(GL_CLIP_PLANE1, &enabled);
	if (enabled) states |= U_CLIP_PLANE1;
	glGetBooleanv(GL_CLIP_PLANE2, &enabled);
	if (enabled) states |= U_CLIP_PLANE2;
	glGetBooleanv(GL_CLIP_PLANE3, &enabled);
	if (enabled) states |= U_CLIP_PLANE3;
	glGetBooleanv(GL_CLIP_PLANE4, &enabled);
	if (enabled) states |= U_CLIP_PLANE4;
	glGetBooleanv(GL_CLIP_PLANE5, &enabled);
	if (enabled) states |= U_CLIP_PLANE5;
*/

}

OpenGLDevice::~OpenGLDevice()
{
}

WindowBase *OpenGLDevice::RenderWindow()
{
	return &m_window;
}

void OpenGLDevice::Lock()
{
	m_window.LockGL();
	#ifdef _DEBUG
	m_locked = true;
	#endif
}

void OpenGLDevice::Unlock()
{
	m_window.UnlockGL();
	#ifdef _DEBUG
	m_locked = false;
	#endif
}

void OpenGLDevice::SwapBuffers()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_window.SwapBuffers();
	m_time = m_timer.Current();
}

void OpenGLDevice::SetFullScreen(bool fullscreen, int32 width, int32 height)
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	m_window.SetFullScreen(fullscreen, width, height);
}

bool OpenGLDevice::IsFullScreen()
{
	D_ASSERT_MESSAGE(m_locked, TEXT("You are trying to access the renderingdevice without locking it"));

	return m_window.IsFullScreen();
}

bool OpenGLDevice::CanSetFullScreen()
{
	return true;
}

HTexture OpenGLDevice::NullTexture()
{
	return m_null_texture;
}
