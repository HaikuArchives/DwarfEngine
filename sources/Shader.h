#ifndef D_SHADER_H_
#define D_SHADER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 *
 *
 */

#include "nommgr.h"
#include <list>
#include "mmgr.h"


#include "Defs.h"

#include "Handle.h"
#include "Texture.h"
#include "ListArray.h"

namespace dwarf
{
	class MyParser;
	class Shader;
	
	/*!
		\brief A shader describes the properties of a surface.

		This class is in most cases constructed by parsing a shader-script file.
		See the \ref shader_page for information how to write a shader
	*/
	class Shader : public RefCnt
	{
	friend class OpenGLDevice;
	public:

		//! \brief describes a waveform
		class Wave
		{
		public:

			enum wave_type
			{
				CONSTANT = 0,
				LINEAR = 1,
				SINUS = 2,
				TRIANGLE = 3,
				SQUARE = 4,
				SAWTOOTH = 5,
				INVERSE_SAWTOOTH = 6
			};

			Wave();
			Wave(const Wave &wave);
			Wave(wave_type type,float slope);
			Wave(wave_type type,float frequency,float amplitude,float phase,float bias);
			Wave(wave_type type,float frequency,float amplitude,float phase,float bias,float pwm);
			float operator()(float time) const;

		private:
			wave_type m_type;
			float m_frequency;
			float m_amplitude;
			float m_phase;
			float m_bias;
			float m_pwm;
		};	

				enum vtx_transform
		{
			VROTATE,
			VMOVE,
			VSCALE
		};

		struct VertexTransformation
		{
			VertexTransformation()
			{
			}

			VertexTransformation(const VertexTransformation &t)
			{
				m_transform = t.m_transform;
				m_wave[0] = t.m_wave[0];
				m_wave[1] = t.m_wave[1];
				m_wave[2] = t.m_wave[2];
			}

			const VertexTransformation &operator=(const VertexTransformation &t)
			{
				m_transform = t.m_transform;
				m_wave[0] = t.m_wave[0];
				m_wave[1] = t.m_wave[1];
				m_wave[2] = t.m_wave[2];
				return *this;
			}

			vtx_transform m_transform;
			Wave m_wave[3]; // x,y,z axis
		};

		// -----------------------------------------------------

		// describes a rendering pass/texture unit
		class Layer
		{
		public:
			
			enum uvgen
			{	
				NORMAL		= 1,
				ENVMAP		= 2
			};

			enum tex_transform
			{
				SCROLL,
				STRETCH,
				ROTATE
			};

			enum wave
			{
				SIN,
				SQUARE,
				SAWTOOTH,
				INVERSE_SAWTOOTH,
				TRIANGLE,
				NONE
			};

			struct TextureTransformation
			{

				TextureTransformation()
				{
				}

				TextureTransformation(const TextureTransformation &t)
				{
					m_transform = t.m_transform;
					m_wave = t.m_wave;
					m_direction = t.m_direction;
				}

				const TextureTransformation &operator=(const TextureTransformation &t)
				{
					m_transform = t.m_transform;
					m_wave = t.m_wave;
					m_direction = t.m_direction;

					return *this;
				}

				tex_transform m_transform;
				Wave m_wave;
				Point m_direction;
			};

			Layer(HTexture texture);
			Layer(const Layer &l);
			Layer();
			
			void SetColor(const Wave &r, const Wave &g, const Wave &b, const Wave &a)
			{
				m_rgba_wave[0] = r;
				m_rgba_wave[1] = g;
				m_rgba_wave[2] = b;
				m_rgba_wave[3] = a;
			}
	
			void SetColor(const Color &color)
			{
				m_rgba_wave[0] = Wave(Shader::Wave::CONSTANT, color.r);
				m_rgba_wave[1] = Wave(Shader::Wave::CONSTANT, color.g);
				m_rgba_wave[2] = Wave(Shader::Wave::CONSTANT, color.b);
				m_rgba_wave[3] = Wave(Shader::Wave::CONSTANT, color.a);
			}

			const Layer &operator=(const Layer &layer);
		
			ListArray<HTexture> m_textures;	// can be more than one for animated layers

			// depth function (LESS/GREATER/EQUAL etc)
			int32 m_depth_func;
			
			float m_alpha_val;

			int32 m_blend_src;
			int32 m_blend_dst;

			int32 m_fillmode;

			int32 m_cull_faces;
			float m_line_width; // the width of lines

			// flags and on/off states
			uvgen m_uvgen;	
			bool m_alpha_test;		// alpha test, not alpha blend (reject fragments based on alpha value)
			bool m_depth_write;		// true if this layer writes to the depth buffer
			bool m_cull;			// is culling enabled?
			bool m_depth_test;		// enables depthtesting
			bool m_lighting;		// enable gl-lighting

			Wave m_rgba_wave[4];

			// texture transformations
			std::list<TextureTransformation> m_uv_transformations;

			// clamp/wrap
			int32 m_wrapmode;

			// animation
			bool m_is_animated;
			float m_framerate;
		};

		Shader();
		Shader(const Layer &layer);

		void AddLayer(const Layer &layer) { m_layers.AddItem(layer); }
		ListArray<Layer>::ConstIterator Layers() const { return m_layers.Begin(); }

		// add surface deformation info here

		static MyParser *ShaderParser();

		std::list<VertexTransformation> m_vertex_transformations; // vertex transformations

	private:
		uint32 m_flags;
		ListArray<Layer> m_layers; // the list where all layers are stored

		// add Vertex<3> shading here
	};

	class HShader : public Handle<Shader>
	{
	public:
		HShader():
			Handle<Shader>()
		{}

		HShader(Shader *shader):
			Handle<Shader>(shader)
		{}
		  

		HShader(const Shader::Layer &layer):
			Handle<Shader>(new Shader(layer))
		{}

		HShader(HShader const& shader):
			Handle<Shader>(shader)
		{}

		HShader(const String &shader_name);

		~HShader()
		{
		}
	};




// inline definitions for Shader::Wave

	inline Shader::Wave::Wave():
		m_type(CONSTANT),
		m_frequency(0.0f),
		m_amplitude(1.0f),
		m_phase(0.0f),
		m_bias(0.0f)
	{
	}

	inline Shader::Wave::Wave(const Wave &wave):
		m_type(wave.m_type),
		m_frequency(wave.m_frequency),
		m_amplitude(wave.m_amplitude),
		m_phase(wave.m_phase),
		m_bias(wave.m_bias),
		m_pwm(wave.m_pwm)
	{
	}

	inline Shader::Wave::Wave(wave_type type,float slope) : m_type(type),m_amplitude(slope)
	{
		D_ASSERT(type == LINEAR || type == CONSTANT);
	}

	inline Shader::Wave::Wave(wave_type type,float frequency,float amplitude,float phase,float bias) :
		m_type(type),
		m_frequency(frequency),
		m_amplitude(amplitude),
		m_phase(phase),
		m_bias(bias)
	{
	}

	inline Shader::Wave::Wave(wave_type type,float frequency,float amplitude,float phase,float bias,float pwm) :
		m_type(type),
		m_frequency(frequency),
		m_amplitude(amplitude),
		m_phase(phase),
		m_bias(bias),
		m_pwm(pwm)
	{
		D_ASSERT(type == SQUARE);
	}
	
	inline float Shader::Wave::operator()(float time) const
	{
		float tri;

		switch(m_type)
		{
		case CONSTANT:
			return m_amplitude;
			break;
		case LINEAR:
			return time * m_amplitude;
			break;
		case SINUS:
			return ((sin((time + m_phase) * D_PI * m_frequency) + 1.0f) * 0.5f * m_amplitude) + m_bias;
			break;
		case TRIANGLE:
			tri = (fmodf(time + m_phase, 1.0f/m_frequency) / (1.0f/m_frequency)) * 2.0f;
			return ((tri > 1.0f ? 2.0f - tri : tri) * m_amplitude + m_bias);
			break;
		case SQUARE:
			return ((fmodf(time + m_phase, 1.0f/m_frequency) > (1.0f/m_frequency * m_pwm)) ? 0.0f : m_amplitude) + m_bias;
			break;
		case SAWTOOTH:
			return fmodf(time * m_frequency * m_amplitude + m_bias,m_amplitude);
			break;
		case INVERSE_SAWTOOTH:
			return m_amplitude - (fmodf(time * m_frequency * m_amplitude + m_bias,m_amplitude));
			break;
		default:
			return 0.0f;
			break;
		}
	}


} // dwarf


#endif // D_SHADER_H_