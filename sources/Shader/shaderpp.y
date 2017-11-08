%name ShaderParser

/*! \todo rename file to ShaderParser.cpp */

// bison_pp -d -l -o Parser.cpp shaderpp.y

%header{
#include "Defs.h"
#include "RenderingDevice.h"
#include "String.h"
#include "List.h"
#include "Log.h"
#include "Directory.h"
#include "Texture.h"
#include "Shader.h"

using namespace dwarf;

%}
%union
{
	char						str[256];
	float						number;
	int							integer;
	int							blendmode;
	int							depthtest;
	int							alphagen;
	int							transform;
	Shader::vtx_transform		vtx_transform;
	struct vec2
	{
		float x;
		float y;
	}	vec2d;
	struct vec3
	{
		float x;
		float y;
		float z;
	}	vec3d;
	struct vec4
	{
		float x;
		float y;
		float z;
		float w;
	}	vec4d;
	Shader::Wave				*wave;
}

%{
	#include <string.h>
	//#include "Shader.h"
	Shader::Layer::TextureTransformation trans;
	Shader::VertexTransformation vtx_trans;
%}

%define MEMBERS \
				protected:\
				dwarf::HShader m_current_shader; \
				dwarf::Shader::Layer	m_current_layer; \
				dwarf::HShader m_shader; \
				bool m_active_shader; \
				bool m_shader_found;

//%define LSP_NEEDED
%define ERROR_BODY =0
%define LEX_BODY =0
%define PARSE_PARAM String shader_name

/* ------------------------------------------------------------------ */

%start shader_list

%token ERROR_TOKEN EOL
%token SHADER LAYER CULLING WIREFRAME SOLID FRONT BACK FILLMODE TEXTURE
%token NONE COLOR LINEWIDTH BTRUE BFALSE UVGEN ENVIRONMENT
%token BLEND ONE ZERO SRC_COLOR DST_COLOR SRC_ALPHA DST_ALPHA ONE_MINUS_SRC_ALPHA ONE_MINUS_DST_ALPHA
%token ONE_MINUS_SRC_COLOR ONE_MINUS_DST_COLOR
%token DEPTHTEST LESS GREATER EQUAL LESS_OR_EQUAL GREATER_OR_EQUAL NEVER NOT_EQUAL ALWAYS
%token ALPHAGEN RGBGEN SIN TRIANGLE SQUARE SAWTOOTH INVSAWTOOTH
%token UVTRANSFORM SCROLL ROTATE STRETCH LINEAR
%token VERTEXTRANSFORM MOVE SCALE AXISX AXISY AXISZ
%token WRAPMODE WRAP CLAMP
%token <number> FLOAT
%token <integer> INTEGER
%token <str> STRING

%type <number> s_float
%type <blendmode> blend_mode
%type <depthtest> depth_test
%type <alphagen> alpha_gen
%type <transform> uvtransform
%type <vec2d> vector2d
%type <vec3d> vector3d
%type <vec4d> vector4d
%type <wave> wave
%type <wave> wave_opt
%type <vtx_transform> vertex_transform

%left '+' '-'
%left '*' '/'
%left UNARY

/* --------------------------------------------------------------------- */

%%

shader_list
: shader					{}
| shader EOL shader_list	{}
;

shader
: SHADER STRING newline_opt '{' newline_opt 
{
	m_current_shader = dwarf::HShader(new Shader());

	if(shader_name == String($2)) m_active_shader = TRUE;
	else m_active_shader = FALSE;
}
global_properties_opt layer_list newline_opt '}'
{
	if(m_active_shader)
	{
		m_shader_found = TRUE;
		m_shader = m_current_shader;
	}
}
;

global_properties_opt
: global_properties
| /* empty */
;

global_properties
: global_property 					{}
| global_properties	global_property {}
;

global_property
: VERTEXTRANSFORM '=' '[' vertex_transform ',' '(' wave ',' wave ',' wave ')' ']' EOL
{
	vtx_trans.m_transform = $4;
	vtx_trans.m_wave[0] = *$7;
	vtx_trans.m_wave[1] = *$9;
	vtx_trans.m_wave[2] = *$11;
	m_current_shader->m_vertex_transformations.push_front(vtx_trans);
}
;


layer_list
: LAYER STRING newline_opt '{' newline_opt 
{
	m_current_layer = dwarf::Shader::Layer();
}
layer_property_list newline_opt '}' EOL
{
	m_current_shader->AddLayer(m_current_layer);
}

| layer_list
LAYER STRING newline_opt '{' newline_opt 
{ 
	m_current_layer = dwarf::Shader::Layer();
}
layer_property_list newline_opt '}' EOL
{
	m_current_shader->AddLayer(m_current_layer);
}
;

layer_property_list
	: layer_property	{}
	| layer_property_list layer_property {}
	;
	
texture_list
: STRING
{
	if(m_active_shader) 
	{
		m_current_layer.m_textures.AddItem(HTexture(String((char *)$1)));
	}
}
| texture_list ',' STRING
{
	if(m_active_shader) 
	{
		m_current_layer.m_textures.AddItem(HTexture(String((char *)$3)));
	}
}
;

/*! \todo fixa enterslag helvetet! */

layer_property
	: CULLING '=' FRONT EOL					{ m_current_layer.m_cull_faces = dwarf::RenderingDevice::FRONT; m_current_layer.m_cull = true; }
	| CULLING '=' BACK EOL					{ m_current_layer.m_cull_faces = dwarf::RenderingDevice::BACK; m_current_layer.m_cull = true; }
	| CULLING '=' NONE EOL					{ m_current_layer.m_cull = false; }
	| FILLMODE '=' SOLID EOL				{ m_current_layer.m_fillmode = dwarf::RenderingDevice::FILL; }
	| FILLMODE '=' WIREFRAME EOL			{ m_current_layer.m_fillmode = dwarf::RenderingDevice::LINE; }
	| TEXTURE '=' STRING EOL				
	{ 
		if(m_active_shader) 
		{
			m_current_layer.m_textures.AddItem(HTexture(String((char *)$3)));
		}
	}
	| TEXTURE '=' '[' texture_list ',' s_float ']' EOL
	{
		m_current_layer.m_is_animated = TRUE;
		m_current_layer.m_framerate = $6;
	}
	| LINEWIDTH '=' FLOAT EOL				{ m_current_layer.m_line_width = $3; }
	| LINEWIDTH '=' INTEGER EOL				{ m_current_layer.m_line_width = $3; }
	| UVGEN '=' ENVIRONMENT EOL { m_current_layer.m_uvgen = Shader::Layer::ENVMAP; }
	| UVGEN '=' NONE EOL { m_current_layer.m_uvgen = Shader::Layer::NORMAL; }
	| BLEND '=' '[' blend_mode ',' blend_mode ']' EOL	{ m_current_layer.m_blend_src = $4; m_current_layer.m_blend_dst = $6; }
	| DEPTHTEST '=' depth_test EOL { m_current_layer.m_depth_func = $3; m_current_layer.m_depth_test = true; }
	| UVTRANSFORM '=' '[' uvtransform ',' wave ',' vector2d ']' EOL
	{
		trans.m_transform = static_cast<Shader::Layer::tex_transform>($4);
		trans.m_wave = Shader::Wave(*$6);
		delete $6;
		trans.m_direction = Point($8.x,$8.y);
		m_current_layer.m_uv_transformations.push_front(trans);
	}
	| UVTRANSFORM '=' '[' ROTATE ',' wave ']' EOL
	{
		trans.m_transform = Shader::Layer::ROTATE;
		trans.m_wave = *$6;
		delete $6;
		m_current_layer.m_uv_transformations.push_front(trans);
	}

	| COLOR '=' '(' wave_opt ',' wave_opt ',' wave_opt ')' EOL
	{
		if($4) m_current_layer.m_rgba_wave[0] = *$4;
		if($6) m_current_layer.m_rgba_wave[1] = *$6;
		if($8) m_current_layer.m_rgba_wave[2] = *$8;
		m_current_layer.m_rgba_wave[3] = Shader::Wave(Shader::Wave::CONSTANT,1.0f);

		delete $4;
		delete $6;
		delete $8;
	}
	| COLOR '=' '(' wave_opt ',' wave_opt ',' wave_opt ',' wave_opt ')' EOL
	{
		if($4) m_current_layer.m_rgba_wave[0] = *$4;
		if($6) m_current_layer.m_rgba_wave[1] = *$6;
		if($8) m_current_layer.m_rgba_wave[2] = *$8;
		if($10) m_current_layer.m_rgba_wave[3] = *$10;

		delete $4;
		delete $6;
		delete $8;
		delete $10;
	}

	| WRAPMODE '=' WRAP WRAP EOL { m_current_layer.m_wrapmode = Texture::WRAP_S_WRAP_T; }
	| WRAPMODE '=' WRAP CLAMP EOL { m_current_layer.m_wrapmode = Texture::WRAP_S_CLAMP_T; }
	| WRAPMODE '=' CLAMP WRAP EOL { m_current_layer.m_wrapmode = Texture::CLAMP_S_WRAP_T; }
	| WRAPMODE '=' CLAMP CLAMP EOL { m_current_layer.m_wrapmode = Texture::CLAMP_S_CLAMP_T; }
	;


vertex_transform
: ROTATE { $$ = Shader::VROTATE; }
| MOVE	{ $$ = Shader::VMOVE; }
| SCALE	{ $$ = Shader::VSCALE; }
;
/*

  uv_transform = scroll linear (0,0.1)
  uv_transform = scroll sin

*/

wave_opt
:			{ $$ = NULL; }
| wave		{ $$ = $1; }
;

vector2d
: '(' s_float ',' s_float ')'	{ $$.x = $2; $$.y = $4; }
;

vector3d
: '(' s_float ',' s_float ',' s_float ')'	{ $$.x = $2; $$.y = $4; $$.z = $6; }
;

vector4d
: '(' s_float ',' s_float ',' s_float ',' s_float ')'	{ $$.x = $2; $$.y = $4; $$.z = $6; $$.w = $8; }
;

uvtransform
: SCROLL	{ $$ = Shader::Layer::SCROLL; }
| STRETCH	{ $$ = Shader::Layer::STRETCH; }
;

newline_opt
:						{}
| newline_opt EOL		{}
;

s_float
: FLOAT					{ $$ = $1; }
| INTEGER				{ $$ = $1; }
| s_float '*' s_float	{ $$ = $1 * $3; }
| s_float '/' s_float	{ $$ = $1 / $3; }
| s_float '+' s_float	{ $$ = $1 + $3; }
| s_float '-' s_float	{ $$ = $1 - $3; }
| '-' s_float %prec UNARY { $$ = -$2; }
| '+' s_float %prec UNARY { $$ = $2; }
| '(' s_float ')'		{ $$ = $2; }
;

blend_mode
: ONE { $$ = RenderingDevice::ONE; }
| ZERO { $$ = RenderingDevice::ZERO; }
| SRC_ALPHA { $$ = RenderingDevice::SRC_ALPHA; }
| DST_ALPHA	{ $$ = RenderingDevice::DST_ALPHA; }
| SRC_COLOR	{ $$ = RenderingDevice::SRC_COLOR; }
| DST_COLOR { $$ = RenderingDevice::DST_COLOR; }
| ONE_MINUS_SRC_ALPHA	{ $$ = RenderingDevice::ONE_MINUS_SRC_ALPHA; }
| ONE_MINUS_DST_ALPHA	{ $$ = RenderingDevice::ONE_MINUS_DST_ALPHA; }
| ONE_MINUS_SRC_COLOR	{ $$ = RenderingDevice::ONE_MINUS_SRC_COLOR; }
| ONE_MINUS_DST_COLOR	{ $$ = RenderingDevice::ONE_MINUS_DST_COLOR; }
;

depth_test
: LESS				{ $$ = RenderingDevice::LESS; }
| GREATER			{ $$ = RenderingDevice::GREATER; }
| EQUAL				{ $$ = RenderingDevice::EQUAL; }
| LESS_OR_EQUAL		{ $$ = RenderingDevice::LESS_OR_EQUAL; }
| GREATER_OR_EQUAL	{ $$ = RenderingDevice::GREATER_OR_EQUAL; }
| NOT_EQUAL			{ $$ = RenderingDevice::NOT_EQUAL; }
| ALWAYS			{ $$ = RenderingDevice::ALWAYS; }
| NEVER				{ $$ = RenderingDevice::NEVER; }
;

alpha_gen
: SIN				{ $$ = Shader::Layer::SIN; }
| SQUARE			{ $$ = Shader::Layer::SQUARE; }
| TRIANGLE			{ $$ = Shader::Layer::TRIANGLE; }
| SAWTOOTH			{ $$ = Shader::Layer::SAWTOOTH; }
| INVSAWTOOTH		{ $$ = Shader::Layer::INVERSE_SAWTOOTH; }
;

wave
: SIN '(' s_float ',' s_float ',' s_float ',' s_float ')'
{
	$$ = new Shader::Wave(Shader::Wave::SINUS,$3,$5,$7,$9);
}
| LINEAR '(' s_float ')'
{
	$$ = new Shader::Wave(Shader::Wave::LINEAR,$3);
}
| TRIANGLE '(' s_float ',' s_float ',' s_float ',' s_float ')'
{
	$$ = new Shader::Wave(Shader::Wave::TRIANGLE,$3,$5,$7,$9);
}
| SAWTOOTH '(' s_float ',' s_float ',' s_float ',' s_float ')'
{
	$$ = new Shader::Wave(Shader::Wave::SAWTOOTH,$3,$5,$7,$9);
}
| INVSAWTOOTH '(' s_float ',' s_float ',' s_float ',' s_float ')'
{
	$$ = new Shader::Wave(Shader::Wave::INVERSE_SAWTOOTH,$3,$5,$7,$9);
}
| SQUARE '(' s_float ',' s_float ',' s_float ',' s_float ')'
{
	$$ = new Shader::Wave(Shader::Wave::SQUARE,$3,$5,$7,$9,0.5f);
}
| SQUARE '(' s_float ',' s_float ',' s_float ',' s_float ',' s_float ')'
{
	$$ = new Shader::Wave(Shader::Wave::SQUARE,$3,$5,$7,$9,$11);
}
| s_float
{
	$$ = new Shader::Wave(Shader::Wave::CONSTANT,$1);
}
;