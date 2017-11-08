#ifndef YY_ShaderParser_h_included
#define YY_ShaderParser_h_included
/* before anything */
#ifdef c_plusplus
#ifndef __cplusplus
#define __cplusplus
#endif
#endif
#ifdef __cplusplus
#ifndef YY_USE_CLASS
#define YY_USE_CLASS
#endif
#else
#endif
#include <stdio.h>

#include "Defs.h"
#include "RenderingDevice.h"
#include "String.h"
#include "List.h"
#include "Log.h"
#include "Directory.h"
#include "Texture.h"
#include "Shader.h"

using namespace dwarf;


typedef union
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
} yy_ShaderParser_stype;
#define YY_ShaderParser_STYPE yy_ShaderParser_stype
#define YY_ShaderParser_MEMBERS  \
				protected:\
				dwarf::HShader m_current_shader; \
				dwarf::Shader::Layer	m_current_layer; \
				dwarf::HShader m_shader; \
				bool m_active_shader; \
				bool m_shader_found;
#define YY_ShaderParser_ERROR_BODY  =0
#define YY_ShaderParser_LEX_BODY  =0
#define YY_ShaderParser_PARSE_PARAM  String shader_name
 /* %{ and %header{ and %union, during decl */
#ifndef YY_ShaderParser_COMPATIBILITY
#ifndef YY_USE_CLASS
#define  YY_ShaderParser_COMPATIBILITY 1
#else
#define  YY_ShaderParser_COMPATIBILITY 0
#endif
#endif

#if YY_ShaderParser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YYLTYPE
#ifndef YY_ShaderParser_LTYPE
#define YY_ShaderParser_LTYPE YYLTYPE
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_ShaderParser_STYPE 
#define YY_ShaderParser_STYPE YYSTYPE
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
/* use %define STYPE */
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_ShaderParser_DEBUG
#define  YY_ShaderParser_DEBUG YYDEBUG
/* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
/* use %define DEBUG */
#endif
#endif
#ifdef YY_ShaderParser_STYPE
#ifndef yystype
#define yystype YY_ShaderParser_STYPE
#endif
#endif
/* use goto to be compatible */
#ifndef YY_ShaderParser_USE_GOTO
#define YY_ShaderParser_USE_GOTO 1
#endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_ShaderParser_USE_GOTO
#define YY_ShaderParser_USE_GOTO 0
#endif

#ifndef YY_ShaderParser_PURE
/* YY_ShaderParser_PURE */
#endif
/* prefix */
#ifndef YY_ShaderParser_DEBUG
/* YY_ShaderParser_DEBUG */
#endif
#ifndef YY_ShaderParser_LSP_NEEDED
 /* YY_ShaderParser_LSP_NEEDED*/
#endif
/* DEFAULT LTYPE*/
#ifdef YY_ShaderParser_LSP_NEEDED
#ifndef YY_ShaderParser_LTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YY_ShaderParser_LTYPE yyltype
#endif
#endif
/* DEFAULT STYPE*/
#ifndef YY_ShaderParser_STYPE
#define YY_ShaderParser_STYPE int
#endif
/* DEFAULT MISCELANEOUS */
#ifndef YY_ShaderParser_PARSE
#define YY_ShaderParser_PARSE yyparse
#endif
#ifndef YY_ShaderParser_LEX
#define YY_ShaderParser_LEX yylex
#endif
#ifndef YY_ShaderParser_LVAL
#define YY_ShaderParser_LVAL yylval
#endif
#ifndef YY_ShaderParser_LLOC
#define YY_ShaderParser_LLOC yylloc
#endif
#ifndef YY_ShaderParser_CHAR
#define YY_ShaderParser_CHAR yychar
#endif
#ifndef YY_ShaderParser_NERRS
#define YY_ShaderParser_NERRS yynerrs
#endif
#ifndef YY_ShaderParser_DEBUG_FLAG
#define YY_ShaderParser_DEBUG_FLAG yydebug
#endif
#ifndef YY_ShaderParser_ERROR
#define YY_ShaderParser_ERROR yyerror
#endif

#ifndef YY_ShaderParser_PARSE_PARAM
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
#define YY_ShaderParser_PARSE_PARAM
#ifndef YY_ShaderParser_PARSE_PARAM_DEF
#define YY_ShaderParser_PARSE_PARAM_DEF
#endif
#endif
#endif
#endif
#ifndef YY_ShaderParser_PARSE_PARAM
#define YY_ShaderParser_PARSE_PARAM void
#endif
#endif

/* TOKEN C */
#ifndef YY_USE_CLASS

#ifndef YY_ShaderParser_PURE
extern YY_ShaderParser_STYPE YY_ShaderParser_LVAL;
#endif

#define	ERROR_TOKEN	258
#define	EOL	259
#define	SHADER	260
#define	LAYER	261
#define	CULLING	262
#define	WIREFRAME	263
#define	SOLID	264
#define	FRONT	265
#define	BACK	266
#define	FILLMODE	267
#define	TEXTURE	268
#define	NONE	269
#define	COLOR	270
#define	LINEWIDTH	271
#define	BTRUE	272
#define	BFALSE	273
#define	UVGEN	274
#define	ENVIRONMENT	275
#define	BLEND	276
#define	ONE	277
#define	ZERO	278
#define	SRC_COLOR	279
#define	DST_COLOR	280
#define	SRC_ALPHA	281
#define	DST_ALPHA	282
#define	ONE_MINUS_SRC_ALPHA	283
#define	ONE_MINUS_DST_ALPHA	284
#define	ONE_MINUS_SRC_COLOR	285
#define	ONE_MINUS_DST_COLOR	286
#define	DEPTHTEST	287
#define	LESS	288
#define	GREATER	289
#define	EQUAL	290
#define	LESS_OR_EQUAL	291
#define	GREATER_OR_EQUAL	292
#define	NEVER	293
#define	NOT_EQUAL	294
#define	ALWAYS	295
#define	ALPHAGEN	296
#define	RGBGEN	297
#define	SIN	298
#define	TRIANGLE	299
#define	SQUARE	300
#define	SAWTOOTH	301
#define	INVSAWTOOTH	302
#define	UVTRANSFORM	303
#define	SCROLL	304
#define	ROTATE	305
#define	STRETCH	306
#define	LINEAR	307
#define	VERTEXTRANSFORM	308
#define	MOVE	309
#define	SCALE	310
#define	AXISX	311
#define	AXISY	312
#define	AXISZ	313
#define	WRAPMODE	314
#define	WRAP	315
#define	CLAMP	316
#define	FLOAT	317
#define	INTEGER	318
#define	STRING	319
#define	UNARY	320

 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
#ifndef YY_ShaderParser_CLASS
#define YY_ShaderParser_CLASS ShaderParser
#endif

#ifndef YY_ShaderParser_INHERIT
#define YY_ShaderParser_INHERIT
#endif
#ifndef YY_ShaderParser_MEMBERS
#define YY_ShaderParser_MEMBERS 
#endif
#ifndef YY_ShaderParser_LEX_BODY
#define YY_ShaderParser_LEX_BODY  
#endif
#ifndef YY_ShaderParser_ERROR_BODY
#define YY_ShaderParser_ERROR_BODY  
#endif
#ifndef YY_ShaderParser_CONSTRUCTOR_PARAM
#define YY_ShaderParser_CONSTRUCTOR_PARAM
#endif
/* choose between enum and const */
#ifndef YY_ShaderParser_USE_CONST_TOKEN
#define YY_ShaderParser_USE_CONST_TOKEN 0
/* yes enum is more compatible with flex,  */
/* so by default we use it */ 
#endif
#if YY_ShaderParser_USE_CONST_TOKEN != 0
#ifndef YY_ShaderParser_ENUM_TOKEN
#define YY_ShaderParser_ENUM_TOKEN yy_ShaderParser_enum_token
#endif
#endif

class YY_ShaderParser_CLASS YY_ShaderParser_INHERIT
{
public: 
#if YY_ShaderParser_USE_CONST_TOKEN != 0
/* static const int token ... */
static const int ERROR_TOKEN;
static const int EOL;
static const int SHADER;
static const int LAYER;
static const int CULLING;
static const int WIREFRAME;
static const int SOLID;
static const int FRONT;
static const int BACK;
static const int FILLMODE;
static const int TEXTURE;
static const int NONE;
static const int COLOR;
static const int LINEWIDTH;
static const int BTRUE;
static const int BFALSE;
static const int UVGEN;
static const int ENVIRONMENT;
static const int BLEND;
static const int ONE;
static const int ZERO;
static const int SRC_COLOR;
static const int DST_COLOR;
static const int SRC_ALPHA;
static const int DST_ALPHA;
static const int ONE_MINUS_SRC_ALPHA;
static const int ONE_MINUS_DST_ALPHA;
static const int ONE_MINUS_SRC_COLOR;
static const int ONE_MINUS_DST_COLOR;
static const int DEPTHTEST;
static const int LESS;
static const int GREATER;
static const int EQUAL;
static const int LESS_OR_EQUAL;
static const int GREATER_OR_EQUAL;
static const int NEVER;
static const int NOT_EQUAL;
static const int ALWAYS;
static const int ALPHAGEN;
static const int RGBGEN;
static const int SIN;
static const int TRIANGLE;
static const int SQUARE;
static const int SAWTOOTH;
static const int INVSAWTOOTH;
static const int UVTRANSFORM;
static const int SCROLL;
static const int ROTATE;
static const int STRETCH;
static const int LINEAR;
static const int VERTEXTRANSFORM;
static const int MOVE;
static const int SCALE;
static const int AXISX;
static const int AXISY;
static const int AXISZ;
static const int WRAPMODE;
static const int WRAP;
static const int CLAMP;
static const int FLOAT;
static const int INTEGER;
static const int STRING;
static const int UNARY;

 /* decl const */
#else
enum YY_ShaderParser_ENUM_TOKEN { YY_ShaderParser_NULL_TOKEN=0
	,ERROR_TOKEN=258
	,EOL=259
	,SHADER=260
	,LAYER=261
	,CULLING=262
	,WIREFRAME=263
	,SOLID=264
	,FRONT=265
	,BACK=266
	,FILLMODE=267
	,TEXTURE=268
	,NONE=269
	,COLOR=270
	,LINEWIDTH=271
	,BTRUE=272
	,BFALSE=273
	,UVGEN=274
	,ENVIRONMENT=275
	,BLEND=276
	,ONE=277
	,ZERO=278
	,SRC_COLOR=279
	,DST_COLOR=280
	,SRC_ALPHA=281
	,DST_ALPHA=282
	,ONE_MINUS_SRC_ALPHA=283
	,ONE_MINUS_DST_ALPHA=284
	,ONE_MINUS_SRC_COLOR=285
	,ONE_MINUS_DST_COLOR=286
	,DEPTHTEST=287
	,LESS=288
	,GREATER=289
	,EQUAL=290
	,LESS_OR_EQUAL=291
	,GREATER_OR_EQUAL=292
	,NEVER=293
	,NOT_EQUAL=294
	,ALWAYS=295
	,ALPHAGEN=296
	,RGBGEN=297
	,SIN=298
	,TRIANGLE=299
	,SQUARE=300
	,SAWTOOTH=301
	,INVSAWTOOTH=302
	,UVTRANSFORM=303
	,SCROLL=304
	,ROTATE=305
	,STRETCH=306
	,LINEAR=307
	,VERTEXTRANSFORM=308
	,MOVE=309
	,SCALE=310
	,AXISX=311
	,AXISY=312
	,AXISZ=313
	,WRAPMODE=314
	,WRAP=315
	,CLAMP=316
	,FLOAT=317
	,INTEGER=318
	,STRING=319
	,UNARY=320

 /* enum token */
     }; /* end of enum declaration */
#endif
public:
 int YY_ShaderParser_PARSE(YY_ShaderParser_PARSE_PARAM);
 virtual void YY_ShaderParser_ERROR(char *msg) YY_ShaderParser_ERROR_BODY;
#ifdef YY_ShaderParser_PURE
#ifdef YY_ShaderParser_LSP_NEEDED
 virtual int  YY_ShaderParser_LEX(YY_ShaderParser_STYPE *YY_ShaderParser_LVAL,YY_ShaderParser_LTYPE *YY_ShaderParser_LLOC) YY_ShaderParser_LEX_BODY;
#else
 virtual int  YY_ShaderParser_LEX(YY_ShaderParser_STYPE *YY_ShaderParser_LVAL) YY_ShaderParser_LEX_BODY;
#endif
#else
 virtual int YY_ShaderParser_LEX() YY_ShaderParser_LEX_BODY;
 YY_ShaderParser_STYPE YY_ShaderParser_LVAL;
#ifdef YY_ShaderParser_LSP_NEEDED
 YY_ShaderParser_LTYPE YY_ShaderParser_LLOC;
#endif
 int YY_ShaderParser_NERRS;
 int YY_ShaderParser_CHAR;
#endif
#if YY_ShaderParser_DEBUG != 0
public:
 int YY_ShaderParser_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
#endif
public:
 YY_ShaderParser_CLASS(YY_ShaderParser_CONSTRUCTOR_PARAM);
public:
 YY_ShaderParser_MEMBERS 
};
/* other declare folow */
#endif


#if YY_ShaderParser_COMPATIBILITY != 0
/* backward compatibility */
#ifndef YYSTYPE
#define YYSTYPE YY_ShaderParser_STYPE
#endif

#ifndef YYLTYPE
#define YYLTYPE YY_ShaderParser_LTYPE
#endif
#ifndef YYDEBUG
#ifdef YY_ShaderParser_DEBUG 
#define YYDEBUG YY_ShaderParser_DEBUG
#endif
#endif

#endif
/* END */
#endif
