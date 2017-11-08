/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Texture.h"
#include "RenderingDevice.h"
#include "Buffer.h"
#include "File.h"
#include "Font.h"
#include "Bitmap.h"

using namespace dwarf;

/*!
	You never construct this class yourself, you load a font trough the uFontFactory
	\brief constructs a font
	\param texture_id the gl texture id number where the font is stored
	\param blocks is a list of unicode_block structures defining the characters that the texture contain
	\param the charset, defining the width and texture position of every character, the number of elements
	this array should contain is the number of characters
	\param descent the baseline position in the font
	\todo The font renderer seems to have problem with characters that are very thin (along the x-axis).
*/
/*
Font::Font(Texture texture_id, const unicode_block *blocks, int32 num_blocks, const Font::font_char *charset, int32 num_chars, float descent):
	m_texture(texture_id),
	m_descent(descent),
	m_blocks(num_blocks),
	m_charset(num_chars)
{
	// copy the block definitions
	memcpy(m_blocks.Data(), blocks, m_blocks.Length() * sizeof(unicode_block));

	// copy the charset definition
	memcpy(m_charset.Data(), charset, m_charset.Length() * sizeof(font_char));
	
	// testing different characters, if they are definied in the font
	// if one of these are, then it will be the default character for
	// non defined characters
	dchar test_list[] = {CHAR('?'), CHAR('.'), CHAR(' '), 0};
	m_default = 0xffff;
	int32 i = 0;
	while (m_default == 0xffff && test_list[i] != 0)
	{
		m_default = Id(test_list[i]);
		i++;
	}
	if (m_default == 0xffff) m_default = 0;
}
*/

Font::Font():
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
}

Font::~Font()
{
}

void Font::Load(File &file)
{
	// read the number of blocks in the font
	Buffer buffer;
	file.Read(buffer);
	
	uint16 num_blocks;
	buffer >> num_blocks >> m_descent;

	m_blocks.SetLength(num_blocks);

	// load every block
	for (int32 i = 0; i < num_blocks; i++)
	{
		buffer >> m_blocks[i].char_index >>
				m_blocks[i].first_character >>
				m_blocks[i].last_character;
		m_blocks[i].first_character = ToBigEndian(m_blocks[i].first_character);
		m_blocks[i].last_character = ToBigEndian(m_blocks[i].last_character);
	}

	// count the number of characters
	int32 char_count = 0;
	for (int32 j = 0; j < num_blocks; j++)
		char_count += m_blocks[j].last_character - m_blocks[j].first_character + 1;
	
	// allocate memory for the characters
	m_charset.SetLength(char_count);

	// load every character
	for (int32 k = 0; k < char_count; k++)
	{
		font_char &c = m_charset[k];
		buffer >> c.ratio >> c.displacement >> c.step;
		((InStream&)buffer) >> c.left_bottom >> c.right_top;
	}

	// read the texture size
	uint16 tex_width;
	uint16 tex_height;

	buffer >> tex_width >> tex_height;
	
	logger(TEXT("Loaded font \"%s\"with size (%d, %d)\n"), *file.Filename(), tex_width, tex_height);

	// the next thing in the file is the actual texture
	Bitmap bitmap(tex_width, tex_height, Bitmap::GRAYSCALE);
	buffer.Read((int8*)bitmap.Data(), bitmap.DataSize());

	m_texture = HTexture(bitmap);
	m_states.SetTexture(m_texture);
}

float Font::StringWidth(const String &string) const
{
	float width = 0;
	for(int32 i = 0; i < string.Length(); i++)
	{
		font_char &def = m_charset[Id(string[i])];
		width += def.step;
	}
	return width;
}

/*!
	By default the character size is 1.0 x 1.0. Before calling this method you must have previously
	called Begin(), and when you've made all Draw() and Printf() calls you must call End()
	\brief draws a string with this font at the given position
	\param pos the position of the bottom of the first character
	\param str the string to write
	\param size the height of the characters
	\sa Begin() End()
*/
void Font::DrawString(RenderingDevice &device, const Point &pos, const String &str, float size) const
{
	D_ASSERT_MESSAGE(m_texture.IsValid(), TEXT("You are trying to use an uninitialized font."));

	device.SetStates(m_states);
	
	float x = pos.x();
	float y = pos.y() - DescentRatio() * size;

	// render quads
	device.Begin(RenderingDevice::QUADS);

	// loop through the characters in the string
	for (int32 i = 0; i < str.Length(); i++)
	{
		// get the character
		uint16 letter = str[i];

		if (letter == CHAR('\n'))
		{
			x = pos.x();
			y -= size;
			continue;
		}

		// find out the character id (font specific) for that letter
		letter = Id(letter);

		font_char &def = m_charset[letter];

		float char_width = def.ratio * size;
		float draw_x = x + def.displacement * size;

		//Specify vertices and texture coordinates

		// should probably change the format of the fonts, to make the texture coordinates right
		// in there

		device.TexCoord(def.left_bottom);
		device.AddVertex(Point(draw_x, y));

		device.TexCoord(Point(def.right_top.x(), def.left_bottom.y()));
		device.AddVertex(Point(draw_x + char_width, y));

		device.TexCoord(def.right_top);
		device.AddVertex(Point(draw_x + char_width, y + size));

		device.TexCoord(Point(def.left_bottom.x(), def.right_top.y()));
		device.AddVertex(Point(draw_x, y + size));

/*
		device.TexCoord(Point(0,0));
		device.AddVertex(Point(0,0));

		device.TexCoord(Point(1,0));
		device.AddVertex(Point(10,0));

		device.TexCoord(Point(1,1));
		device.AddVertex(Point(10,10));

		device.TexCoord(Point(0,1));
		device.AddVertex(Point(0,10));
*/
		x += def.step * size;
	}

	// finish rendering quads
	device.End();
}

uint16 Font::Id(uint16 character) const
{
	for (int32 i = 0; i < m_blocks.Length(); i++)
	{
		unicode_block &block = m_blocks[i];
		if (character >= block.first_character && character <= block.last_character)
			return block.char_index + character - block.first_character;
	}
	return m_default;
}
