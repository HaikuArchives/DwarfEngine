#ifndef D_FONT_H_
#define D_FONT_H_
/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Color.h"
#include "Rect.h"
#include "Texture.h"
#include "Vertex.h"
#include "Array.h"
#include "String.h"
#include "Window.h"
#include "RenderingDevice.h"
#include "State.h"
#include "File.h"

namespace dwarf
{

		/*!
			\brief represents a font
			\todo document this class
			\todo this class doesn't seem to work on fonts containing only one unicode block.
		*/
		class Font
		{
		public:
			/*!
				\brief represents a set of unicode characters that are defined in the font
			*/
			struct unicode_block
			{
				uint16 char_index;
				uint16 first_character;
				uint16 last_character;
			};

			Font();
			Font(File &file);
			Font(FileSystem *dir, const String &filename);
			virtual ~Font();

			void Load(File &file);
			void Load(FileSystem *dir, const String &filename);
			bool IsValid() const { return m_texture.IsValid(); }

			State &DrawStates() { return m_states; }

			float StringWidth(const String &string) const;

			void DrawString(RenderingDevice &device, const Point& pos, const String &str, float size = 1.0) const;
		
			float CharRatio(uint16 character) const;
			float StepRatio(uint16 character) const;
			float DescentRatio() const;

			const Array<unicode_block> &DefinedCharacters();
		
		private:

			struct font_char
			{
				float ratio; // width / height of the character (0,1]
				float displacement; // displacement for the character (per height)
				float step; // stepsize for the character (per height)
				Point left_bottom;
				Point right_top;
			};
/*
			Font(HTexture texture, const unicode_block *blocks, 
				int32 num_blocks, const font_char *charset, 
				int32 num_chars, float descent);
*/		
			// converts a unicode character into font-specific index number
			uint16 Id(uint16 character) const;
		
			HTexture m_texture; // the font texture
			float m_descent;
		
			Array<unicode_block> m_blocks; // the blocks
			Array<font_char> m_charset; // character set of this font
		
			uint16 m_default;
			State m_states;
		};

// inline definitions

		inline Font::Font(File &file):
			m_texture(),
			m_descent(0.0),
			m_blocks(0),
			m_charset(0),
			m_default(0)
		{
			m_states.Enable(State::TEXTURE_2D + State::BLEND);
			m_states.Disable(State::DEPTH_TEST + State::CULL_FACE);
			m_states.SetBlendMode(RenderingDevice::ONE, RenderingDevice::ONE);
			m_states.SetColor(Color(1,1,1,1));
			Load(file);
		}

		inline Font::Font(FileSystem *dir, const String &filename):
			m_texture(),
			m_descent(0.0),
			m_blocks(0),
			m_charset(0),
			m_default(0)
		{
			m_states.Enable(State::TEXTURE_2D + State::BLEND);
			m_states.Disable(State::LIGHTING + State::CULL_FACE);
			m_states.SetBlendMode(RenderingDevice::ONE, RenderingDevice::ONE);
			m_states.SetColor(Color(1,1,1,1));

			File file(dir, filename);
			Load(file);
		}

		/*!
			Note that the width is just the width of the character itself, not the space
			around it. If you want to know how long the cursor moves when you print a
			character you have to call StepRatio().
			\brief lets you know the width/height ratio of a specific character
			\param character a unicode character you're insterested in
			\return the width / height ratio of that character
			\sa StepRatio()
		*/
		inline float Font::CharRatio(uint16 character) const
		{
			return m_charset[Id(character)].ratio;
		}
		
		inline float Font::StepRatio(uint16 character) const
		{
			return m_charset[Id(character)].step;
		}
		
		inline float Font::DescentRatio() const
		{
			return m_descent;
		}
		
		inline void Font::Load(FileSystem *dir, const String &filename)
		{
			File file(dir, filename);
			Load(file);
		}
		
		/*!
			remember that you can call Array::Length() to find out how many blocks there are
			in the array.
			\brief let's you know which characters are defined in the font
			\return an Array with unicode_blocks that together defines all the
			characters that you can find in the font.
		*/
		inline const Array<Font::unicode_block> &Font::DefinedCharacters()
		{
			return m_blocks;
		}

} // dwarf

#endif // D_FONT_H_
