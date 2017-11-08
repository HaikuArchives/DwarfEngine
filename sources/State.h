/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#ifndef D_STATE_H_
#define D_STATE_H_

#include "Defs.h"

#include "RenderingDevice.h"

/*!
	\brief a set of 3d-engine states
*/
namespace dwarf
{

	class State
	{
	public:
		
		// it's very important that these are in the same order as in RenderingDevice
		enum render_state
		{
			ALPHA_TEST 		= 0x0000001,
			AUTO_NORMAL		= 0x0000002,
			BLEND			= 0x0000004,
	
			COLOR_MATERIAL	= 0x0000008,
			CULL_FACE		= 0x0000010,
			DEPTH_TEST		= 0x0000020,
			DITHER			= 0x0000040,
	
			FOG				= 0x0000080,
	
			LIGHTING		= 0x0000100,
			LINE_SMOOTH		= 0x0000200,
			LINE_STIPPLE	= 0x0000400,
			LOGIC_OP		= 0x0000800,
	
			POINT_SMOOTH	= 0x0001000,
			POLYGON_SMOOTH	= 0x0002000,
			POLYGON_STIPPLE	= 0x0004000,
			SCISSOR_TEST	= 0x0008000,
			STENCIL_TEST	= 0x0010000,
			TEXTURE_1D		= 0x0020000,
			TEXTURE_2D		= 0x0040000,
		};
	
		State();
	
		void Enable(uint32 states); // enables a set of states
		void Disable(uint32 states); // disables a set of states
		void Ignore(uint32 states); // ignores the given states
		void IgnoreNothing() { m_mask = 0xffffffff; }
	
		uint32 CurrentStates() const;	// returns the current states as a mask
		uint32 UsedStates() const;	// returns a mask where each bit represent a state, if a bit is set to one, it means that that state is used (not ignored)

		void Set(const State &state);

		void SetTexture(HTexture texture) { m_texture = texture; }
		void SetBlendMode(RenderingDevice::Blendmode src, RenderingDevice::Blendmode dst);
		void SetColor(const Color &color) { m_color = color; };
		void SetFillMode(RenderingDevice::Fillmode mode) { m_fillmode = mode; }
		void SetDepthTestFunction(RenderingDevice::TestFunction func);
		void SetAlphaTestFunction(RenderingDevice::TestFunction func, float reference);
		void SetShadeModel(RenderingDevice::ShadeModel shade) { m_shade_model = shade; }
		void SetCullMode(RenderingDevice::Faces faces) { m_cullmode = faces; }

		RenderingDevice::ShadeModel ShadeModel() const { return m_shade_model; }
		RenderingDevice::Blendmode DestinationBlend() const { return m_destination_blend; }
		RenderingDevice::Blendmode SourceBlend() const { return m_source_blend; }
		const Color &GetColor() const { return m_color; }
		HTexture GetTexture() const { return m_texture; }
		RenderingDevice::Fillmode GetFillmode() const { return m_fillmode; }
		RenderingDevice::Faces GetCullMode() const { return m_cullmode; }

		RenderingDevice::TestFunction GetDepthTestFunction() const { return m_depthfunction; }

	private:
	
		uint32 m_states;	// all the states
		uint32 m_mask;		// a mask defining the states that are used (including the states that's not stored in m_states)
	
		Color m_color;
		HTexture m_texture;
		RenderingDevice::Blendmode m_destination_blend;	// the destination blend mode (used when blend is enabled)
		RenderingDevice::Blendmode m_source_blend;		// the source blend mode (used when blend is enabled)
		RenderingDevice::Fillmode m_fillmode;			// the fillmode
		RenderingDevice::TestFunction m_depthfunction;	// the depth function (used when depth test is enabled)
		RenderingDevice::TestFunction m_alphafunction;	// the alpha function (used when alpha test is enabled)
		RenderingDevice::Faces m_cullmode;		// the face culling mode (used when cull face is enabled)
		RenderingDevice::ShadeModel m_shade_model;
	
		float m_alpha_reference;				// the reference value for the alpha test function
	};
	
	// inline definitions
	
	inline State::State():
		m_states(State::DEPTH_TEST + State::CULL_FACE),
		m_mask(0xfffff),	// ignore nothing by default
		m_destination_blend(RenderingDevice::ZERO),
		m_source_blend(RenderingDevice::ONE),
		m_fillmode(RenderingDevice::FILL),
		m_depthfunction(RenderingDevice::LESS),
		m_cullmode(RenderingDevice::BACK),
		m_shade_model(RenderingDevice::FLAT)
	{
	}
	
	inline void State::Enable(uint32 states)
	{
		m_mask |= states;
		m_states |= states;
	}
	
	inline void State::Disable(uint32 states)
	{
		m_mask |= states;
		m_states &= ~states;
	}
	
	inline void State::Ignore(uint32 states)
	{
		m_mask &= ~states;
	}
	
	inline uint32 State::CurrentStates() const
	{
		return m_states;
	}
	
	inline uint32 State::UsedStates() const
	{
		return m_mask;
	}

	inline void State::Set(const State &state)
	{
		m_mask |= state.m_mask;
		m_states = m_states & (~state.m_mask);
		m_states |= state.m_states;
	}

	inline void State::SetBlendMode(RenderingDevice::Blendmode src, RenderingDevice::Blendmode dst)
	{
		m_destination_blend = dst;
		m_source_blend = src;
	}

	inline void State::SetDepthTestFunction(RenderingDevice::TestFunction func)
	{
		m_depthfunction = func;
	}
	
	inline void State::SetAlphaTestFunction(RenderingDevice::TestFunction func, float reference)
	{
		m_alphafunction = func;
		m_alpha_reference = reference;
	}

} // dwarf

#endif // D_STATE_H_

