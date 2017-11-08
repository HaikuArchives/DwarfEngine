%{
#include "Shader/Lexer.h"
#include "Shader/Parser.h"
#include "Math.h"

using namespace dwarf;

#undef YY_DECL
#define YY_DECL int ShaderLexer::yylex(YY_ShaderParser_STYPE *val)
%}

%option c++
%option case-insensitive
%option noyywrap
%option stdinit
%option never-interactive
%option yyclass="ShaderLexer"

LETTER		[a-zA-Z_]
DIGIT		[0-9]
/*IDENT		{LETTER}({LETTER}|{DIGIT})*	*/
STRING		\"[^\"]*\"
WSPACE		[ \t]+
INTEGER		{DIGIT}+
FLOAT		{DIGIT}*"."{INTEGER}

%x one_line_comment
%x multi_line_comment
%x line_wrap_mode

%%

"//"					{ BEGIN(one_line_comment); }
<one_line_comment>"."
<one_line_comment>"\n"	{ BEGIN(INITIAL); unput('\n'); }

"/*"					{ BEGIN(multi_line_comment); }
<multi_line_comment>[^*\n]*
<multi_line_comment>"*"+[^*/\n]*
<multi_line_comment>"\n"
<multi_line_comment>"*/" { BEGIN(INITIAL); }

"\\"	{ BEGIN(line_wrap_mode); }
<line_wrap_mode>"."
<line_wrap_mode>"\n"	{ BEGIN(INITIAL); }


"shader"	{ return ShaderParser::SHADER; }
"layer"		{ return ShaderParser::LAYER; }
"culling"	{ return ShaderParser::CULLING; }
"front"		{ return ShaderParser::FRONT; }
"back"		{ return ShaderParser::BACK; }
"none"		{ return ShaderParser::NONE; }
"fillmode"	{ return ShaderParser::FILLMODE; }
"wireframe"	{ return ShaderParser::WIREFRAME; }
"solid"		{ return ShaderParser::SOLID; }
"texture"	{ return ShaderParser::TEXTURE; }
"color"		{ return ShaderParser::COLOR; }
"true"		{ return ShaderParser::BTRUE; }
"false"		{ return ShaderParser::BFALSE; }
"linewidth"	{ return ShaderParser::LINEWIDTH; }
"uvgen"		{ return ShaderParser::UVGEN; }
"environment" { return ShaderParser::ENVIRONMENT; }

"blendmode"	{ return ShaderParser::BLEND; }
"one"		{ return ShaderParser::ONE; }
"zero"		{ return ShaderParser::ZERO; }
"src_color"	{ return ShaderParser::SRC_COLOR; }
"dst_color"	{ return ShaderParser::DST_COLOR; }
"src_alpha"	{ return ShaderParser::SRC_ALPHA; }
"dst_alpha"	{ return ShaderParser::DST_ALPHA; }
"one_minus_src_alpha"	{ return ShaderParser::ONE_MINUS_SRC_ALPHA; }
"one_minus_dst_alpha"	{ return ShaderParser::ONE_MINUS_DST_ALPHA; }
"one_minus_src_color"	{ return ShaderParser::ONE_MINUS_SRC_COLOR; }
"one_minus_dst_color"	{ return ShaderParser::ONE_MINUS_DST_COLOR; }

"depthtest"	{ return ShaderParser::DEPTHTEST; }
"less"	{ return ShaderParser::LESS; }
"equal"	{ return ShaderParser::EQUAL; }
"greater"	{ return ShaderParser::GREATER; }
"less_or_equal"	{ return ShaderParser::LESS_OR_EQUAL; }
"greater_or_equal"	{ return ShaderParser::GREATER_OR_EQUAL; }
"always"			{ return ShaderParser::ALWAYS; }
"never"				{ return ShaderParser::NEVER; }
"not_equal"			{ return ShaderParser::NOT_EQUAL; }

"alphagen"	{ return ShaderParser::ALPHAGEN; }
"rgbgen"	{ return ShaderParser::RGBGEN; }
"sin"		{ return ShaderParser::SIN; }
"triangle"	{ return ShaderParser::TRIANGLE; }
"square"	{ return ShaderParser::SQUARE; }
"sawtooth"	{ return ShaderParser::SAWTOOTH; }
"inverse_sawtooth" { return ShaderParser::INVSAWTOOTH; }

"uv_transform"	{ return ShaderParser::UVTRANSFORM; }
"scroll"		{ return ShaderParser::SCROLL; }
"rotate"		{ return ShaderParser::ROTATE; }
"stretch"		{ return ShaderParser::STRETCH; }
"linear"		{ return ShaderParser::LINEAR; }

"vertex_transform" { return ShaderParser::VERTEXTRANSFORM; }
"move"				{ return ShaderParser::MOVE; }
"scale"				{ return ShaderParser::SCALE; }
"x-axis"			{ return ShaderParser::AXISX; }
"pitch"				{ return ShaderParser::AXISX; }
"y-axis"			{ return ShaderParser::AXISY; }
"yaw"				{ return ShaderParser::AXISY; }
"z-axis"			{ return ShaderParser::AXISZ; }
"bank"				{ return ShaderParser::AXISZ; }

"{"			{ return '{'; }
"}"			{ return '}'; }
"="			{ return '='; }
"("			{ return '('; }
")"			{ return ')'; }
","			{ return ','; }
"["			{ return '['; }
"]"			{ return ']'; }
"*"			{ return '*'; }
"/"			{ return '/'; }
"+"			{ return '+'; }
"-"			{ return '-'; }

"wrapmode"	{ return ShaderParser::WRAPMODE; }
"wrap"		{ return ShaderParser::WRAP; }
"clamp"		{ return ShaderParser::CLAMP; }

"pi"		{ val->number = D_PI; return ShaderParser::FLOAT; }

{FLOAT}	{
				val->number = atof(YYText());
				return ShaderParser::FLOAT;
}

{INTEGER}	{
				val->integer = atoi(YYText());
				return ShaderParser::INTEGER;
}

{STRING}	{
				int32 i = 0;
				for (; i < min((int)strlen(YYText()+1)-1, 255); i++)
					val->str[i] = YYText()[i+1];
				val->str[i] = 0;
				return ShaderParser::STRING;
}

{WSPACE}	{}
^"\n"		{ logger(TEXT("Empty line!!!")); }
"\n"		{ return ShaderParser::EOL; }
"."			{ return ShaderParser::ERROR_TOKEN; }

<<EOF>> { yyterminate();}

%%