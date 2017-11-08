/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 *
 *
 */


#include "Defs.h"


#include "Shader.h"
#include "RenderingDevice.h"
#include "Shader/ShaderParser.h"

using namespace dwarf;

/*!
	\brief constructs a layer with the given texture
	All other attributes are set to the default.
*/
Shader::Layer::Layer(HTexture texture):
	m_uvgen(NORMAL),
	m_depth_func(RenderingDevice::LESS),
	m_alpha_test(false),
	m_blend_src(RenderingDevice::ONE),
	m_blend_dst(RenderingDevice::ZERO),
	m_fillmode(RenderingDevice::FILL),
	m_cull_faces(RenderingDevice::BACK),
	m_cull(true),
	m_depth_test(true),
	m_depth_write(true),
	m_lighting(false),
	m_line_width(1.0),
	m_uv_transformations(),
	m_wrapmode(Texture::WRAP_S_WRAP_T),
	m_is_animated(false),
	m_framerate(0.0f)
{
	m_textures.AddItem(texture);
}

/*!
	\brief constructs a layer with only the default attributes, white
	If this layer is used it will appear to be all white.
*/
Shader::Layer::Layer():
	m_uvgen(NORMAL),
//	m_textures(1),
	m_depth_func(RenderingDevice::LESS),
	m_alpha_test(false),
	m_blend_src(RenderingDevice::ONE),
	m_blend_dst(RenderingDevice::ZERO),
	m_fillmode(RenderingDevice::FILL),
	m_cull_faces(RenderingDevice::BACK),
	m_cull(true),
	m_depth_test(true),
	m_depth_write(true),
	m_lighting(false),
	m_line_width(1.0),
	m_uv_transformations(),
	m_wrapmode(Texture::WRAP_S_WRAP_T),
	m_is_animated(false),
	m_framerate(0.0f)
{
}

/*!
	\brief copies a layer
*/
Shader::Layer::Layer(const Layer &l):
	m_uvgen(l.m_uvgen),
	m_textures(l.m_textures),
	m_depth_func(l.m_depth_func),
	m_alpha_test(l.m_alpha_test),
	m_blend_src(l.m_blend_src),
	m_blend_dst(l.m_blend_dst),
	m_fillmode(l.m_fillmode),
	m_cull_faces(l.m_cull_faces),
	m_cull(l.m_cull),
	m_depth_test(l.m_depth_test),
	m_depth_write(l.m_depth_write),
	m_lighting(l.m_lighting),
	m_line_width(l.m_line_width),
//	m_rgba_wave(l.m_rgba_wave),
	m_uv_transformations(l.m_uv_transformations),
	m_wrapmode(l.m_wrapmode),
	m_is_animated(l.m_is_animated),
	m_framerate(l.m_framerate)
{
	m_rgba_wave[0] = l.m_rgba_wave[0];
	m_rgba_wave[1] = l.m_rgba_wave[1];
	m_rgba_wave[2] = l.m_rgba_wave[2];
	m_rgba_wave[3] = l.m_rgba_wave[3];
}

const Shader::Layer &Shader::Layer::operator=(const Shader::Layer &layer)
{
	m_uvgen = layer.m_uvgen;
	m_depth_func = layer.m_depth_func;
	m_alpha_test = layer.m_alpha_test;
	m_blend_src = layer.m_blend_src;
	m_blend_dst = layer.m_blend_dst;
	m_fillmode = layer.m_fillmode;
	m_cull_faces = layer.m_cull_faces;
	m_cull = layer.m_cull;
	m_depth_test = layer.m_depth_test;
	m_depth_write = layer.m_depth_write;
	m_lighting = layer.m_lighting;
	m_line_width = layer.m_line_width;
	m_uv_transformations = layer.m_uv_transformations;
	m_is_animated = layer.m_is_animated;
	m_framerate = layer.m_framerate;
	m_textures = layer.m_textures;

	m_rgba_wave[0] = layer.m_rgba_wave[0];
	m_rgba_wave[1] = layer.m_rgba_wave[1];
	m_rgba_wave[2] = layer.m_rgba_wave[2];
	m_rgba_wave[3] = layer.m_rgba_wave[3];

	return *this;
}

/*!
	\brief constructs an empty shader
*/
Shader::Shader():
	m_flags(0),
	m_layers(1),
	m_vertex_transformations()
{
}

/*!
	\brief constructs a shader and adds the given layer to it
*/
Shader::Shader(const Layer &layer):
	m_flags(0),
	m_layers(1),
	m_vertex_transformations()
{
	m_layers.AddItem(layer);
}

MyParser *Shader::ShaderParser()
{
	static MyParser parser;
	return &parser;
}


HShader::HShader(const String &shader_name)
{
	MyParser &parser = *Shader::ShaderParser();
	HShader shader = parser(shader_name);
	operator=(shader);
}

/*!
	\page shader_page The shader language
	First of all, you should remember that // are used for comments (just like in c++),
	everything on that line, after the //, will be considered as a comment by the parser.

	A shader is declared using the keyword "shader". The syntax is as follows:

\code
shader "the name of the shader"
{
}
\endcode

	Where {} should contain all the properties of the shader. The main shader property
	are layers. A shader can contain any number of layers (you
	should try to keep the number of layers down though, for performance). A layer
	is one rendering pass, so the order of your layer declarations will matter,
	the layers declared first will be drawn first.

	\section shaders_layers Layers
	A layer is a rendering pass, and the syntax to describe a layer is as follows:

\code
layer "the name of the layer"
{
}
\endcode

	Where {} should contain the layer's properties. Properties are separated
	with a carrige return (newline). Valid properties are:

	\subsection shader_layers_texture texture = <image_path>
	Makes the layer use the specified image as a texture. Where \e <image_path>
	is a string (surrounded by quotation marks), that is the path, and filename, of
	an image to use as the texture.

	\subsection shader_layers_color color = (<red>, <green>, <blue>, <alpha>)
	Gives the shader the given color, where the four parameters (<red>, <green>,
	<blue> and <alpha>) is the intensity of their respective color.

	The intensity can be given as a constant expression (e.g. 0.5) or as a function
	(see the \ref shader_functions section for details on functions).

	The intensity is given in the range [0,1] where 1 is full intensity and
	0 is no intensity. If there is a texture specified, each texel on the texture
	will be multiplied with this color when drawn. The alpha channel can be omitted,
	and defaults to 1. It describes how transparent the material is, 1 is opaque
	and 0 is totally transparent (invisible). The alpha channel can, of course, be
	used in other ways in the blendmode (you can have 1 mean transparent and 0 opaque,
	but that's not the standard way).

	If you don't supply a color, the default is white (1,1,1,1), and
	when multiplied with a texture, the texture is unchanged.

	\subsection shader_layers_culling culling = <cull_mode>
	The \e <cullmode> can be two things
	\li \e back all polygons facing away from the camera will be culled (not drawn)
	\li \e front all polygons facing the camera will be culled.
	\li \e none no polygons will be culled.

	\subsection shader_layers_fillmode fillmode = <fillmode>
	The \e <fillmode> can be three things
	\li \e solid (default) polygons will be drawn as normal, filled, polygons.
	\li \e wireframe only the edges of the polygons will be drawn. If
	there is a texture specified it will be used when drawing the lines,
	otherwise the color will be used.
	\li \e points only the points (between the edges) will be drawn,
	as small dots.
 
	\subsection shader_layers_uvgen uvgen = <uvgen_mode>
	\e <uvgen_mode> can be two things:
	\li \e none everything will appear as normal
	\li \e environment all texture coordinates will be generated by using the normal
	of each Vertex, thus creating the illusion that the material is reflecting
	the environment (hence the name).
	
	The reason that this is called uvgen instead
	of texcoordgen or something is that the texture coordinates are usually called
	(u,v) instead of (x,y) which normal points are usually called.

	\subsection shader_layers_linewidth linewidth = <width>
	Sets the width of lines. This is of course only interesting if
	you have set the fillmode to wireframe or if the mesh contains lines.
	The <width> can be anything greater than zero, but on most hardware the lines
	looks bad if the width is greater than 5.

	\subsection shader_layers_blendmode blendmode = [<src_multiplication>, <dest_multiplication>]
	Sets the mode in which all pixels are drawn to the screen. The \e <src_multiplication>
	tells the renderer what to multiply the source color with. The source color is the color of
	the polygon that is being drawn, it's color is determined from it's texture and/or it's color.

	The \e <dest_multiplication> defines what the destination color should be multiplied with. The
	destination color is the color already drawn on the screen, from earlier polygons drawn.
	
	When both the source color and the destination color (of the pixel in question) are multiplied with
	their respective factors, they are added together, and finally drawn to the screen. It could be described
	with the following formula:

	\code
	screen_color = source_color * <src_multiplication> + destination_color * <dst_multiplication>
	\endcode

	Now, both the \e <src_multiplication> and \e <dst_multiplication> can have any of the following values:
	\li \e one This is one, if you multiply a color with one, it is unchanged.
	\li \e zero If you multiply any color with zero, it will alwayd be black.
	\li \e src_alpha multiplies with the source color's alpha channel
	\li \e one_minus_src_alpha multiplies with one minus the source alpha channel
	\li \e dst_alpha the destination color's alpha channel
	\li \e one_minus_dst_alpha multiplies with one minus the destination alpha (the destination may
	not always have an alpha channel)
	\li \e src_color
	\li \e one_minus_src_color
	\li \e dst_color
	\li \e one_minus_dst_color
	
	The default mode is \e one \e zero, that is, draw only the source color, from the polygon
	we are drawing, and ignore what's already on the screen.

	So, if you want to add the source color to the destination color you simply set the blendmode to:
	\code
	blendmode = [one, one]
	\endcode
	Another popular combination is to blend the drawn texture using it's alpha channel
	(where 0 is totally transparent and 1 is opaque).
	\code
	blendmode = [src_alpha, one_minus_src_alpha]
	\endcode
	If you just want to invert the colors where you draw, you can set
	\code
	color = (1,1,1)
	blendmode = [one_minus_dst_color, zero]
	\endcode
	It's important that the color is white (at least not black), because the inverted
	destination color is multiplied with it.

	\section shader_functions Functions
	There are a number of functions that can be used.

	\subsection shader_functions_sinus sin(<frequency>, <amplitude>, <bias>, <phase>)
	This is (hopefully to no surprise) a sinus wave. It is a slightly modified
	sinus wave to match our usage better,the \e ampitude parameter tells the maximum
	value the function can return, and the minimum value is always 0 (as long as
	\e bias is set to 0).
	\param frequency the frequency of the function, the number of periods per second.
	\param amplitude sets the maximum value the function can return.
	\param bias the \e bias is added to the function's return value.
	\param phase the \e phase is added to the variable before applied to the function. The
	variable is always time. In other words, it shifts the function along the time-axis.

	\subsection shader_functions_triangle triangle(<frequency>, <aplitude>, <bias>, <phase>)
	This function forms a triangle wave.
	\param frequency The frequency of the function, the number of periods per second.
	\param amplitude the maximum value of the function's return value. (The minimum is 0 as
	long as \e bias is 0).
	\param bias The \e bias is added to the function's return value.
	\param phase Shifts the function along the time-axis.

	\subsection shader_functions_square square(<frequency>, <amplitude>, <bias>, <phase>, <duty_cycle>)
	\param frequency the functions frequency, the number of cycles per second.
	\param amplitude The maximum value of the function.
	\param bias The \e bias is added to the function's return value.
	\param phase Shifts the function along the time-axis.
	\param duty_cycle This paramter can be ommitted, it sets the duty cycle, which is the ratio between up-time
	and down-time. It set to .1, it means that the function will be high only 10 % of one period, if set
	to .9 the function will be high 90% of the period. The default (if parameter is ommitted) is .5, which
	means that the function will be low 50% of the period and high the remaining 50%.

	\subsection shader_functions_linear linear(<slope>)
	This function is special because it's not periodic, most applications of this function
	will probably mod the result, to make it stay inside some boundaries.
	\param slope The spedd with which this linear function will grow.

	\subsection shader_functions_sawtooth sawtooth()

	\subsection shader_functions_invsawtooth inv_sawtooth()

	\section shader_examples Example shaders


	Here's an example shader that draws the object as a cartoon.

\code
shader "cartoon"
{
	layer "shadow"
	{
		texture = "toonenv.jpg"
		uvgen = environment
		color = (0.7, 0, 0.9)
		culling = back
	}
	layer "outline"
	{
		fillmode = wireframe
		color = (0, 0, 0)
		linewidth = 4
		depthtest = less_or_equal
		culling = front
	}
}
\endcode

	  Here's an example shader that makes the material look really shiny.

\code
shader "shiny"
{
	layer "texture"
	{
		texture = "tex2.jpg"
	}
	layer "reflection"
	{
		texture = "env.jpg"
		uvgen = environment
		depthtest = equal
		color = (1, 1, 1, 0.3)
		blendmode = [src_alpha, one_minus_src_alpha]
	}
}
\endcode



*/