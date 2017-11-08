#define YY_ShaderParser_h_included

/*  A Bison++ parser, made from shaderpp.y  */

 /* with Bison++ version bison++ Version 1.21-8, adapted from GNU bison by coetmeur@icdc.fr
  */

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Bob Corbett and Richard Stallman

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 1, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* HEADER SECTION */
#if defined( _MSDOS ) || defined(MSDOS) || defined(__MSDOS__) 
#define __MSDOS_AND_ALIKE
#endif
#if defined(_WINDOWS) && defined(_MSC_VER)
#define __HAVE_NO_ALLOCA
#define __MSDOS_AND_ALIKE
#endif

#ifndef alloca
#if defined( __GNUC__)
#define alloca __builtin_alloca

#elif (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc)  || defined (__sgi)
#include <alloca.h>

#elif defined (__MSDOS_AND_ALIKE)
#include <malloc.h>
#ifndef __TURBOC__
/* MS C runtime lib */
#define alloca _alloca
#endif

#elif defined(_AIX)
#include <malloc.h>
#pragma alloca

#elif defined(__hpux)
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */

#endif /* not _AIX  not MSDOS, or __TURBOC__ or _AIX, not sparc.  */
#endif /* alloca not defined.  */
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
#ifndef __STDC__
#define const
#endif
#endif
#include <stdio.h>
#define YYBISON 1  

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

	#include <string.h>
	//#include "Shader.h"
	Shader::Layer::TextureTransformation trans;
	Shader::VertexTransformation vtx_trans;
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
#define YY_ShaderParser_BISON 1
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
#endif
#endif
#ifdef YYSTYPE
#ifndef YY_ShaderParser_STYPE 
#define YY_ShaderParser_STYPE YYSTYPE
#endif
#endif
#ifdef YYDEBUG
#ifndef YY_ShaderParser_DEBUG
#define  YY_ShaderParser_DEBUG YYDEBUG
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
/*  YY_ShaderParser_PURE */
#endif

/* section apres lecture def, avant lecture grammaire S2 */
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
      /* We used to use `unsigned long' as YY_ShaderParser_STYPE on MSDOS,
	 but it seems better to be consistent.
	 Most programs should declare their own type anyway.  */

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
#if YY_ShaderParser_COMPATIBILITY != 0
/* backward compatibility */
#ifdef YY_ShaderParser_LTYPE
#ifndef YYLTYPE
#define YYLTYPE YY_ShaderParser_LTYPE
#else
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
#endif
#endif
#ifndef YYSTYPE
#define YYSTYPE YY_ShaderParser_STYPE
#else
/* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
#endif
#ifdef YY_ShaderParser_PURE
#ifndef YYPURE
#define YYPURE YY_ShaderParser_PURE
#endif
#endif
#ifdef YY_ShaderParser_DEBUG
#ifndef YYDEBUG
#define YYDEBUG YY_ShaderParser_DEBUG 
#endif
#endif
#ifndef YY_ShaderParser_ERROR_VERBOSE
#ifdef YYERROR_VERBOSE
#define YY_ShaderParser_ERROR_VERBOSE YYERROR_VERBOSE
#endif
#endif
#ifndef YY_ShaderParser_LSP_NEEDED
#ifdef YYLSP_NEEDED
#define YY_ShaderParser_LSP_NEEDED YYLSP_NEEDED
#endif
#endif
#endif
#ifndef YY_USE_CLASS
/* TOKEN C */
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

 /* #defines tokens */
#else
/* CLASS */
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
#ifndef YY_ShaderParser_CONSTRUCTOR_CODE
#define YY_ShaderParser_CONSTRUCTOR_CODE
#endif
#ifndef YY_ShaderParser_CONSTRUCTOR_INIT
#define YY_ShaderParser_CONSTRUCTOR_INIT
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
 int YY_ShaderParser_PARSE (YY_ShaderParser_PARSE_PARAM);
 virtual void YY_ShaderParser_ERROR(char *msg) YY_ShaderParser_ERROR_BODY;
#ifdef YY_ShaderParser_PURE
#ifdef YY_ShaderParser_LSP_NEEDED
 virtual int  YY_ShaderParser_LEX (YY_ShaderParser_STYPE *YY_ShaderParser_LVAL,YY_ShaderParser_LTYPE *YY_ShaderParser_LLOC) YY_ShaderParser_LEX_BODY;
#else
 virtual int  YY_ShaderParser_LEX (YY_ShaderParser_STYPE *YY_ShaderParser_LVAL) YY_ShaderParser_LEX_BODY;
#endif
#else
 virtual int YY_ShaderParser_LEX() YY_ShaderParser_LEX_BODY;
 YY_ShaderParser_STYPE YY_ShaderParser_LVAL;
#ifdef YY_ShaderParser_LSP_NEEDED
 YY_ShaderParser_LTYPE YY_ShaderParser_LLOC;
#endif
 int   YY_ShaderParser_NERRS;
 int    YY_ShaderParser_CHAR;
#endif
#if YY_ShaderParser_DEBUG != 0
 int YY_ShaderParser_DEBUG_FLAG;   /*  nonzero means print parse trace     */
#endif
public:
 YY_ShaderParser_CLASS(YY_ShaderParser_CONSTRUCTOR_PARAM);
public:
 YY_ShaderParser_MEMBERS 
};
/* other declare folow */
#if YY_ShaderParser_USE_CONST_TOKEN != 0
const int YY_ShaderParser_CLASS::ERROR_TOKEN=258;
const int YY_ShaderParser_CLASS::EOL=259;
const int YY_ShaderParser_CLASS::SHADER=260;
const int YY_ShaderParser_CLASS::LAYER=261;
const int YY_ShaderParser_CLASS::CULLING=262;
const int YY_ShaderParser_CLASS::WIREFRAME=263;
const int YY_ShaderParser_CLASS::SOLID=264;
const int YY_ShaderParser_CLASS::FRONT=265;
const int YY_ShaderParser_CLASS::BACK=266;
const int YY_ShaderParser_CLASS::FILLMODE=267;
const int YY_ShaderParser_CLASS::TEXTURE=268;
const int YY_ShaderParser_CLASS::NONE=269;
const int YY_ShaderParser_CLASS::COLOR=270;
const int YY_ShaderParser_CLASS::LINEWIDTH=271;
const int YY_ShaderParser_CLASS::BTRUE=272;
const int YY_ShaderParser_CLASS::BFALSE=273;
const int YY_ShaderParser_CLASS::UVGEN=274;
const int YY_ShaderParser_CLASS::ENVIRONMENT=275;
const int YY_ShaderParser_CLASS::BLEND=276;
const int YY_ShaderParser_CLASS::ONE=277;
const int YY_ShaderParser_CLASS::ZERO=278;
const int YY_ShaderParser_CLASS::SRC_COLOR=279;
const int YY_ShaderParser_CLASS::DST_COLOR=280;
const int YY_ShaderParser_CLASS::SRC_ALPHA=281;
const int YY_ShaderParser_CLASS::DST_ALPHA=282;
const int YY_ShaderParser_CLASS::ONE_MINUS_SRC_ALPHA=283;
const int YY_ShaderParser_CLASS::ONE_MINUS_DST_ALPHA=284;
const int YY_ShaderParser_CLASS::ONE_MINUS_SRC_COLOR=285;
const int YY_ShaderParser_CLASS::ONE_MINUS_DST_COLOR=286;
const int YY_ShaderParser_CLASS::DEPTHTEST=287;
const int YY_ShaderParser_CLASS::LESS=288;
const int YY_ShaderParser_CLASS::GREATER=289;
const int YY_ShaderParser_CLASS::EQUAL=290;
const int YY_ShaderParser_CLASS::LESS_OR_EQUAL=291;
const int YY_ShaderParser_CLASS::GREATER_OR_EQUAL=292;
const int YY_ShaderParser_CLASS::NEVER=293;
const int YY_ShaderParser_CLASS::NOT_EQUAL=294;
const int YY_ShaderParser_CLASS::ALWAYS=295;
const int YY_ShaderParser_CLASS::ALPHAGEN=296;
const int YY_ShaderParser_CLASS::RGBGEN=297;
const int YY_ShaderParser_CLASS::SIN=298;
const int YY_ShaderParser_CLASS::TRIANGLE=299;
const int YY_ShaderParser_CLASS::SQUARE=300;
const int YY_ShaderParser_CLASS::SAWTOOTH=301;
const int YY_ShaderParser_CLASS::INVSAWTOOTH=302;
const int YY_ShaderParser_CLASS::UVTRANSFORM=303;
const int YY_ShaderParser_CLASS::SCROLL=304;
const int YY_ShaderParser_CLASS::ROTATE=305;
const int YY_ShaderParser_CLASS::STRETCH=306;
const int YY_ShaderParser_CLASS::LINEAR=307;
const int YY_ShaderParser_CLASS::VERTEXTRANSFORM=308;
const int YY_ShaderParser_CLASS::MOVE=309;
const int YY_ShaderParser_CLASS::SCALE=310;
const int YY_ShaderParser_CLASS::AXISX=311;
const int YY_ShaderParser_CLASS::AXISY=312;
const int YY_ShaderParser_CLASS::AXISZ=313;
const int YY_ShaderParser_CLASS::WRAPMODE=314;
const int YY_ShaderParser_CLASS::WRAP=315;
const int YY_ShaderParser_CLASS::CLAMP=316;
const int YY_ShaderParser_CLASS::FLOAT=317;
const int YY_ShaderParser_CLASS::INTEGER=318;
const int YY_ShaderParser_CLASS::STRING=319;
const int YY_ShaderParser_CLASS::UNARY=320;

 /* const YY_ShaderParser_CLASS::token */
#endif
/*apres const  */
YY_ShaderParser_CLASS::YY_ShaderParser_CLASS(YY_ShaderParser_CONSTRUCTOR_PARAM) YY_ShaderParser_CONSTRUCTOR_INIT
{
#if YY_ShaderParser_DEBUG != 0
YY_ShaderParser_DEBUG_FLAG=0;
#endif
YY_ShaderParser_CONSTRUCTOR_CODE;
};
#endif


#define	YYFINAL		251
#define	YYFLAG		-32768
#define	YYNTBASE	78

#define YYTRANSLATE(x) ((unsigned)(x) <= 320 ? yytranslate[x] : 99)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    75,
    76,    67,    65,    74,    66,     2,    68,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    72,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    73,     2,    77,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    70,     2,    71,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    58,    59,    60,    61,    62,    63,    64,    69
};

#if YY_ShaderParser_DEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     6,     7,    18,    20,    21,    23,    26,    41,
    42,    53,    54,    66,    68,    71,    73,    77,    82,    87,
    92,    97,   102,   107,   116,   121,   126,   131,   136,   145,
   150,   161,   170,   181,   194,   200,   206,   212,   218,   220,
   222,   224,   225,   227,   233,   241,   251,   253,   255,   256,
   259,   261,   263,   267,   271,   275,   279,   282,   285,   289,
   291,   293,   295,   297,   299,   301,   303,   305,   307,   309,
   311,   313,   315,   317,   319,   321,   323,   325,   327,   329,
   331,   333,   335,   346,   351,   362,   373,   384,   395,   408
};

static const short yyrhs[] = {    79,
     0,    79,     4,    78,     0,     0,     5,    64,    94,    70,
    94,    80,    81,    84,    94,    71,     0,    82,     0,     0,
    83,     0,    82,    83,     0,    53,    72,    73,    90,    74,
    75,    98,    74,    98,    74,    98,    76,    77,     4,     0,
     0,     6,    64,    94,    70,    94,    85,    87,    94,    71,
     4,     0,     0,    84,     6,    64,    94,    70,    94,    86,
    87,    94,    71,     4,     0,    89,     0,    87,    89,     0,
    64,     0,    88,    74,    64,     0,     7,    72,    10,     4,
     0,     7,    72,    11,     4,     0,     7,    72,    14,     4,
     0,    12,    72,     9,     4,     0,    12,    72,     8,     4,
     0,    13,    72,    64,     4,     0,    13,    72,    73,    88,
    74,    95,    77,     4,     0,    16,    72,    62,     4,     0,
    16,    72,    63,     4,     0,    19,    72,    20,     4,     0,
    19,    72,    14,     4,     0,    21,    72,    73,    96,    74,
    96,    77,     4,     0,    32,    72,    97,     4,     0,    48,
    72,    73,    93,    74,    98,    74,    92,    77,     4,     0,
    48,    72,    73,    50,    74,    98,    77,     4,     0,    15,
    72,    75,    91,    74,    91,    74,    91,    76,     4,     0,
    15,    72,    75,    91,    74,    91,    74,    91,    74,    91,
    76,     4,     0,    59,    72,    60,    60,     4,     0,    59,
    72,    60,    61,     4,     0,    59,    72,    61,    60,     4,
     0,    59,    72,    61,    61,     4,     0,    50,     0,    54,
     0,    55,     0,     0,    98,     0,    75,    95,    74,    95,
    76,     0,    75,    95,    74,    95,    74,    95,    76,     0,
    75,    95,    74,    95,    74,    95,    74,    95,    76,     0,
    49,     0,    51,     0,     0,    94,     4,     0,    62,     0,
    63,     0,    95,    67,    95,     0,    95,    68,    95,     0,
    95,    65,    95,     0,    95,    66,    95,     0,    66,    95,
     0,    65,    95,     0,    75,    95,    76,     0,    22,     0,
    23,     0,    26,     0,    27,     0,    24,     0,    25,     0,
    28,     0,    29,     0,    30,     0,    31,     0,    33,     0,
    34,     0,    35,     0,    36,     0,    37,     0,    39,     0,
    40,     0,    38,     0,    43,     0,    45,     0,    44,     0,
    46,     0,    47,     0,    43,    75,    95,    74,    95,    74,
    95,    74,    95,    76,     0,    52,    75,    95,    76,     0,
    44,    75,    95,    74,    95,    74,    95,    74,    95,    76,
     0,    46,    75,    95,    74,    95,    74,    95,    74,    95,
    76,     0,    47,    75,    95,    74,    95,    74,    95,    74,
    95,    76,     0,    45,    75,    95,    74,    95,    74,    95,
    74,    95,    76,     0,    45,    75,    95,    74,    95,    74,
    95,    74,    95,    74,    95,    76,     0,    95,     0
};

#endif

#if YY_ShaderParser_DEBUG != 0
static const short yyrline[] = { 0,
   110,   111,   115,   122,   133,   134,   138,   139,   143,   155,
   159,   164,   169,   176,   177,   181,   188,   200,   201,   202,
   203,   204,   205,   212,   217,   218,   219,   220,   221,   222,
   223,   231,   239,   250,   263,   264,   265,   266,   271,   272,
   273,   283,   284,   288,   292,   296,   300,   301,   305,   306,
   310,   311,   312,   313,   314,   315,   316,   317,   318,   322,
   323,   324,   325,   326,   327,   328,   329,   330,   331,   335,
   336,   337,   338,   339,   340,   341,   342,   346,   347,   348,
   349,   350,   354,   358,   362,   366,   370,   374,   378,   382
};

static const char * const yytname[] = {   "$","error","$illegal.","ERROR_TOKEN",
"EOL","SHADER","LAYER","CULLING","WIREFRAME","SOLID","FRONT","BACK","FILLMODE",
"TEXTURE","NONE","COLOR","LINEWIDTH","BTRUE","BFALSE","UVGEN","ENVIRONMENT",
"BLEND","ONE","ZERO","SRC_COLOR","DST_COLOR","SRC_ALPHA","DST_ALPHA","ONE_MINUS_SRC_ALPHA",
"ONE_MINUS_DST_ALPHA","ONE_MINUS_SRC_COLOR","ONE_MINUS_DST_COLOR","DEPTHTEST",
"LESS","GREATER","EQUAL","LESS_OR_EQUAL","GREATER_OR_EQUAL","NEVER","NOT_EQUAL",
"ALWAYS","ALPHAGEN","RGBGEN","SIN","TRIANGLE","SQUARE","SAWTOOTH","INVSAWTOOTH",
"UVTRANSFORM","SCROLL","ROTATE","STRETCH","LINEAR","VERTEXTRANSFORM","MOVE",
"SCALE","AXISX","AXISY","AXISZ","WRAPMODE","WRAP","CLAMP","FLOAT","INTEGER",
"STRING","'+'","'-'","'*'","'/'","UNARY","'{'","'}'","'='","'['","','","'('",
"')'","']'","shader_list","shader","@1","global_properties_opt","global_properties",
"global_property","layer_list","@2","@3","layer_property_list","texture_list",
"layer_property","vertex_transform","wave_opt","vector2d","uvtransform","newline_opt",
"s_float","blend_mode","depth_test","wave","depth_test"
};
#endif

static const short yyr1[] = {     0,
    78,    78,    80,    79,    81,    81,    82,    82,    83,    85,
    84,    86,    84,    87,    87,    88,    88,    89,    89,    89,
    89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
    89,    89,    89,    89,    89,    89,    89,    89,    90,    90,
    90,    91,    91,    92,    -1,    -1,    93,    93,    94,    94,
    95,    95,    95,    95,    95,    95,    95,    95,    95,    96,
    96,    96,    96,    96,    96,    96,    96,    96,    96,    97,
    97,    97,    97,    97,    97,    97,    97,    -1,    -1,    -1,
    -1,    -1,    98,    98,    98,    98,    98,    98,    98,    98
};

static const short yyr2[] = {     0,
     1,     3,     0,    10,     1,     0,     1,     2,    14,     0,
    10,     0,    11,     1,     2,     1,     3,     4,     4,     4,
     4,     4,     4,     8,     4,     4,     4,     4,     8,     4,
    10,     8,    10,    12,     5,     5,     5,     5,     1,     1,
     1,     0,     1,     5,     7,     9,     1,     1,     0,     2,
     1,     1,     3,     3,     3,     3,     2,     2,     3,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,    10,     4,    10,    10,    10,    10,    12,     1
};

static const short yydefact[] = {     0,
     0,     1,    49,     0,     0,     2,    50,    49,     3,     6,
     0,     0,     5,     7,     0,     0,    49,     8,     0,    49,
     0,     0,    39,    40,    41,     0,     0,    49,     4,     0,
    49,     0,     0,    10,    49,     0,     0,     0,     0,     0,
     0,    51,    52,     0,     0,     0,    90,     0,     0,    12,
     0,     0,     0,     0,     0,     0,    58,    57,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    49,    14,     0,     0,     0,     0,
     0,     0,     0,    59,    55,    56,    53,    54,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    15,
     0,    49,     0,     0,     0,     0,     0,    84,     0,     0,
     0,     0,     0,     0,     0,     0,    42,     0,     0,     0,
     0,     0,    70,    71,    72,    73,    74,    77,    75,    76,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    18,    19,    20,    22,    21,    23,    16,     0,
     0,    43,    25,    26,    28,    27,    60,    61,    64,    65,
    62,    63,    66,    67,    68,    69,     0,    30,    47,     0,
    48,     0,     0,     0,     0,     0,    11,     0,     0,     0,
     0,     0,     0,     0,     0,    42,     0,     0,     0,    35,
    36,    37,    38,    13,     0,     0,     0,     0,     0,     0,
    17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     9,     0,    42,     0,     0,     0,     0,     0,     0,
     0,     0,    24,     0,    29,    32,     0,     0,    83,    85,
     0,    88,    86,    87,    42,     0,     0,     0,     0,     0,
    33,     0,    31,    89,     0,     0,    34,    44,     0,     0,
     0
};

static const short yydefgoto[] = {     6,
     2,    10,    12,    13,    14,    17,    49,    77,    75,   150,
    76,    26,   151,   228,   172,     5,    47,   167,   131,   152
};

static const short yypact[] = {    47,
   -33,    44,-32768,    47,    11,-32768,-32768,-32768,    61,    21,
    -5,    64,    21,-32768,    12,    38,    72,-32768,    14,-32768,
    40,     0,-32768,-32768,-32768,    36,    16,-32768,-32768,    50,
-32768,    17,    51,    61,-32768,    52,    65,    82,    86,    97,
   102,-32768,-32768,    43,    43,    43,   -23,    55,    34,    61,
    43,    43,    43,    43,    43,    43,-32768,-32768,    66,    43,
    43,    43,    43,    51,    81,    90,   106,   109,   110,   113,
   114,   124,   125,   128,    34,-32768,    34,   161,   165,   175,
   179,   189,    78,-32768,     9,     9,-32768,-32768,   134,   101,
    75,   -50,   105,    37,    18,   129,   305,   136,    63,-32768,
     1,    34,    43,    43,    43,    43,    43,-32768,    51,   197,
   206,   208,   209,   210,   230,   147,    51,   232,   233,   234,
   244,   194,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
   246,     7,    87,    95,   247,     2,   193,   203,   207,   217,
   221,   176,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   188,
   190,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   191,-32768,-32768,   192,
-32768,   202,   274,   275,   276,   286,-32768,   288,    43,    43,
    43,    43,    43,   216,    26,    51,   194,    51,    51,-32768,
-32768,-32768,-32768,-32768,   231,   235,   245,   249,   259,   290,
-32768,   -38,   248,   227,   229,   258,    43,    43,    43,    43,
    43,-32768,   303,    51,   304,   314,   260,    84,   103,    54,
   108,   123,-32768,   -39,-32768,-32768,    43,   243,-32768,-32768,
    43,-32768,-32768,-32768,    51,   317,   263,   330,   127,   270,
-32768,    43,-32768,-32768,   332,   139,-32768,-32768,   347,   348,
-32768
};

static const short yypgoto[] = {   349,
-32768,-32768,-32768,-32768,   337,-32768,-32768,-32768,   277,-32768,
   -51,-32768,  -160,-32768,-32768,     5,   -44,   164,-32768,   -30
};


#define	YYLAST		354


static const short yytable[] = {    57,
    58,    59,    48,     7,     7,     7,    78,    79,    80,    81,
    82,    83,     9,   115,     7,    85,    86,    87,    88,     7,
     7,    22,   116,   100,    27,   203,    60,    61,    62,    63,
     3,   120,    32,    89,   235,    34,   236,   121,   213,    50,
    65,    60,    61,    62,    63,    66,    67,     4,    68,    69,
   100,     1,    70,   224,    71,   169,   170,   171,   137,   138,
   139,   140,   141,    23,     7,    72,    15,    24,    25,    16,
    29,   135,   178,    11,   240,    62,    63,    21,   142,   101,
     8,    73,   113,   114,    19,    31,    35,    42,    43,   201,
    44,    45,    74,    36,    37,    38,    39,    40,   118,   119,
    46,    20,    41,    28,    42,    43,   136,    44,    45,    30,
   110,   111,    42,    43,   112,    44,    45,    46,    60,    61,
    62,    63,   133,   134,    33,    46,    51,   231,    64,   232,
    60,    61,    62,    63,   195,   196,   197,   198,   199,    52,
   202,    84,    60,    61,    62,    63,   173,   174,    60,    61,
    62,    63,    90,   108,   175,   176,    53,   205,   206,   229,
    54,    91,   218,   219,   220,   221,   222,    60,    61,    62,
    63,    55,    60,    61,    62,    63,    56,    92,   230,   117,
    93,    94,   237,   233,    95,    96,   239,    60,    61,    62,
    63,    60,    61,    62,    63,    97,    98,   246,   234,    99,
   143,   122,   244,    60,    61,    62,    63,   109,   132,   144,
   149,   145,   146,   147,   248,   157,   158,   159,   160,   161,
   162,   163,   164,   165,   166,    60,    61,    62,    63,    60,
    61,    62,    63,   148,   103,   153,   154,   155,   104,    60,
    61,    62,    63,    60,    61,    62,    63,   156,   105,   168,
   177,   184,   106,    60,    61,    62,    63,    60,    61,    62,
    63,   185,   107,   186,   187,   188,   179,    60,    61,    62,
    63,    60,    61,    62,    63,   189,   180,   190,   191,   192,
   181,    60,    61,    62,    63,    60,    61,    62,    63,   193,
   182,   194,   200,   212,   183,    60,    61,    62,    63,    60,
    61,    62,    63,   215,   207,   216,   223,   225,   208,    60,
    61,    62,    63,    60,    61,    62,    63,   226,   209,   238,
   241,   214,   210,    60,    61,    62,    63,    60,    61,    62,
    63,   217,   211,   243,   227,   247,   242,   123,   124,   125,
   126,   127,   128,   129,   130,   245,   250,   251,   249,    18,
   204,     0,     0,   102
};

static const short yycheck[] = {    44,
    45,    46,    33,     4,     4,     4,    51,    52,    53,    54,
    55,    56,     8,    64,     4,    60,    61,    62,    63,     4,
     4,    17,    73,    75,    20,   186,    65,    66,    67,    68,
    64,    14,    28,    64,    74,    31,    76,    20,    77,    35,
     7,    65,    66,    67,    68,    12,    13,     4,    15,    16,
   102,     5,    19,   214,    21,    49,    50,    51,   103,   104,
   105,   106,   107,    50,     4,    32,    72,    54,    55,     6,
    71,    71,    71,    53,   235,    67,    68,     6,   109,    75,
    70,    48,     8,     9,    73,    70,    70,    62,    63,    64,
    65,    66,    59,    43,    44,    45,    46,    47,    62,    63,
    75,    64,    52,    64,    62,    63,   102,    65,    66,    74,
    10,    11,    62,    63,    14,    65,    66,    75,    65,    66,
    67,    68,    60,    61,    75,    75,    75,    74,    74,    76,
    65,    66,    67,    68,   179,   180,   181,   182,   183,    75,
   185,    76,    65,    66,    67,    68,    60,    61,    65,    66,
    67,    68,    72,    76,    60,    61,    75,   188,   189,    76,
    75,    72,   207,   208,   209,   210,   211,    65,    66,    67,
    68,    75,    65,    66,    67,    68,    75,    72,    76,    75,
    72,    72,   227,    76,    72,    72,   231,    65,    66,    67,
    68,    65,    66,    67,    68,    72,    72,   242,    76,    72,
     4,    73,    76,    65,    66,    67,    68,    74,    73,     4,
    64,     4,     4,     4,    76,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    65,    66,    67,    68,    65,
    66,    67,    68,     4,    74,     4,     4,     4,    74,    65,
    66,    67,    68,    65,    66,    67,    68,     4,    74,     4,
     4,    76,    74,    65,    66,    67,    68,    65,    66,    67,
    68,    74,    74,    74,    74,    74,    74,    65,    66,    67,
    68,    65,    66,    67,    68,    74,    74,     4,     4,     4,
    74,    65,    66,    67,    68,    65,    66,    67,    68,     4,
    74,     4,    77,     4,    74,    65,    66,    67,    68,    65,
    66,    67,    68,    77,    74,    77,     4,     4,    74,    65,
    66,    67,    68,    65,    66,    67,    68,     4,    74,    77,
     4,    74,    74,    65,    66,    67,    68,    65,    66,    67,
    68,    74,    74,     4,    75,     4,    74,    33,    34,    35,
    36,    37,    38,    39,    40,    76,     0,     0,     0,    13,
   187,    -1,    -1,    77
};
 /* fattrs + tables */

/* parser code folow  */


/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: dollar marks section change
   the next  is replaced by the list of actions, each action
   as one case of the switch.  */ 

#if YY_ShaderParser_USE_GOTO != 0
/* 
 SUPRESSION OF GOTO : on some C++ compiler (sun c++)
  the goto is strictly forbidden if any constructor/destructor
  is used in the whole function (very stupid isn't it ?)
 so goto are to be replaced with a 'while/switch/case construct'
 here are the macro to keep some apparent compatibility
*/
#define YYGOTO(lb) {yy_gotostate=lb;continue;}
#define YYBEGINGOTO  enum yy_labels yy_gotostate=yygotostart; \
                     for(;;) switch(yy_gotostate) { case yygotostart: {
#define YYLABEL(lb) } case lb: {
#define YYENDGOTO } } 
#define YYBEGINDECLARELABEL enum yy_labels {yygotostart
#define YYDECLARELABEL(lb) ,lb
#define YYENDDECLARELABEL  };
#else
/* macro to keep goto */
#define YYGOTO(lb) goto lb
#define YYBEGINGOTO 
#define YYLABEL(lb) lb:
#define YYENDGOTO
#define YYBEGINDECLARELABEL 
#define YYDECLARELABEL(lb)
#define YYENDDECLARELABEL 
#endif
/* LABEL DECLARATION */
YYBEGINDECLARELABEL
  YYDECLARELABEL(yynewstate)
  YYDECLARELABEL(yybackup)
/* YYDECLARELABEL(yyresume) */
  YYDECLARELABEL(yydefault)
  YYDECLARELABEL(yyreduce)
  YYDECLARELABEL(yyerrlab)   /* here on detecting error */
  YYDECLARELABEL(yyerrlab1)   /* here on error raised explicitly by an action */
  YYDECLARELABEL(yyerrdefault)  /* current state does not do anything special for the error token. */
  YYDECLARELABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */
  YYDECLARELABEL(yyerrhandle)  
YYENDDECLARELABEL
/* ALLOCA SIMULATION */
/* __HAVE_NO_ALLOCA */
#ifdef __HAVE_NO_ALLOCA
int __alloca_free_ptr(char *ptr,char *ref)
{if(ptr!=ref) free(ptr);
 return 0;}

#define __ALLOCA_alloca(size) malloc(size)
#define __ALLOCA_free(ptr,ref) __alloca_free_ptr((char *)ptr,(char *)ref)

#ifdef YY_ShaderParser_LSP_NEEDED
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		    __ALLOCA_free(yyls,yylsa)+\
		   (num))
#else
#define __ALLOCA_return(num) \
            return( __ALLOCA_free(yyss,yyssa)+\
		    __ALLOCA_free(yyvs,yyvsa)+\
		   (num))
#endif
#else
#define __ALLOCA_return(num) return(num)
#define __ALLOCA_alloca(size) alloca(size)
#define __ALLOCA_free(ptr,ref) 
#endif

/* ENDALLOCA SIMULATION */

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (YY_ShaderParser_CHAR = YYEMPTY)
#define YYEMPTY         -2
#define YYEOF           0
#define YYACCEPT        __ALLOCA_return(0)
#define YYABORT         __ALLOCA_return(1)
#define YYERROR         YYGOTO(yyerrlab1)
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL          YYGOTO(yyerrlab)
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do                                                              \
  if (YY_ShaderParser_CHAR == YYEMPTY && yylen == 1)                               \
    { YY_ShaderParser_CHAR = (token), YY_ShaderParser_LVAL = (value);                 \
      yychar1 = YYTRANSLATE (YY_ShaderParser_CHAR);                                \
      YYPOPSTACK;                                               \
      YYGOTO(yybackup);                                            \
    }                                                           \
  else                                                          \
    { YY_ShaderParser_ERROR ("syntax error: cannot back up"); YYERROR; }   \
while (0)

#define YYTERROR        1
#define YYERRCODE       256

#ifndef YY_ShaderParser_PURE
/* UNPURE */
#define YYLEX           YY_ShaderParser_LEX()
#ifndef YY_USE_CLASS
/* If nonreentrant, and not class , generate the variables here */
int     YY_ShaderParser_CHAR;                      /*  the lookahead symbol        */
YY_ShaderParser_STYPE      YY_ShaderParser_LVAL;              /*  the semantic value of the */
				/*  lookahead symbol    */
int YY_ShaderParser_NERRS;                 /*  number of parse errors so far */
#ifdef YY_ShaderParser_LSP_NEEDED
YY_ShaderParser_LTYPE YY_ShaderParser_LLOC;   /*  location data for the lookahead     */
			/*  symbol                              */
#endif
#endif


#else
/* PURE */
#ifdef YY_ShaderParser_LSP_NEEDED
#define YYLEX           YY_ShaderParser_LEX(&YY_ShaderParser_LVAL, &YY_ShaderParser_LLOC)
#else
#define YYLEX           YY_ShaderParser_LEX(&YY_ShaderParser_LVAL)
#endif
#endif
#ifndef YY_USE_CLASS
#if YY_ShaderParser_DEBUG != 0
int YY_ShaderParser_DEBUG_FLAG;                    /*  nonzero means print parse trace     */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif
#endif



/*  YYINITDEPTH indicates the initial size of the parser's stacks       */

#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif


#if __GNUC__ > 1                /* GNU C and GNU C++ define this.  */
#define __yy_bcopy(FROM,TO,COUNT)       __builtin_memcpy(TO,FROM,COUNT)
#else                           /* not GNU C or C++ */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */

#ifdef __cplusplus
static void __yy_bcopy (char *from, char *to, int count)
#else
#ifdef __STDC__
static void __yy_bcopy (char *from, char *to, int count)
#else
static void __yy_bcopy (from, to, count)
     char *from;
     char *to;
     int count;
#endif
#endif
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}
#endif

int
#ifdef YY_USE_CLASS
 YY_ShaderParser_CLASS::
#endif
     YY_ShaderParser_PARSE(YY_ShaderParser_PARSE_PARAM)
#ifndef __STDC__
#ifndef __cplusplus
#ifndef YY_USE_CLASS
/* parameter definition without protypes */
YY_ShaderParser_PARSE_PARAM_DEF
#endif
#endif
#endif
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YY_ShaderParser_STYPE *yyvsp;
  int yyerrstatus;      /*  number of tokens to shift before error messages enabled */
  int yychar1=0;          /*  lookahead token as an internal (translated) token number */

  short yyssa[YYINITDEPTH];     /*  the state stack                     */
  YY_ShaderParser_STYPE yyvsa[YYINITDEPTH];        /*  the semantic value stack            */

  short *yyss = yyssa;          /*  refer to the stacks thru separate pointers */
  YY_ShaderParser_STYPE *yyvs = yyvsa;     /*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YY_ShaderParser_LSP_NEEDED
  YY_ShaderParser_LTYPE yylsa[YYINITDEPTH];        /*  the location stack                  */
  YY_ShaderParser_LTYPE *yyls = yylsa;
  YY_ShaderParser_LTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YY_ShaderParser_PURE
  int YY_ShaderParser_CHAR;
  YY_ShaderParser_STYPE YY_ShaderParser_LVAL;
  int YY_ShaderParser_NERRS;
#ifdef YY_ShaderParser_LSP_NEEDED
  YY_ShaderParser_LTYPE YY_ShaderParser_LLOC;
#endif
#endif

  YY_ShaderParser_STYPE yyval;             /*  the variable used to return         */
				/*  semantic values from the action     */
				/*  routines                            */

  int yylen;
/* start loop, in which YYGOTO may be used. */
YYBEGINGOTO

#if YY_ShaderParser_DEBUG != 0
  if (YY_ShaderParser_DEBUG_FLAG)
    fprintf(stderr, "Starting parse\n");
#endif
  yystate = 0;
  yyerrstatus = 0;
  YY_ShaderParser_NERRS = 0;
  YY_ShaderParser_CHAR = YYEMPTY;          /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YY_ShaderParser_LSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
YYLABEL(yynewstate)

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YY_ShaderParser_STYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YY_ShaderParser_LSP_NEEDED
      YY_ShaderParser_LTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YY_ShaderParser_LSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YY_ShaderParser_LSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  YY_ShaderParser_ERROR("parser stack overflow");
	  __ALLOCA_return(2);
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) __ALLOCA_alloca (yystacksize * sizeof (*yyssp));
      __yy_bcopy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      __ALLOCA_free(yyss1,yyssa);
      yyvs = (YY_ShaderParser_STYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yyvsp));
      __yy_bcopy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
      __ALLOCA_free(yyvs1,yyvsa);
#ifdef YY_ShaderParser_LSP_NEEDED
      yyls = (YY_ShaderParser_LTYPE *) __ALLOCA_alloca (yystacksize * sizeof (*yylsp));
      __yy_bcopy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
      __ALLOCA_free(yyls1,yylsa);
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YY_ShaderParser_LSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YY_ShaderParser_DEBUG != 0
      if (YY_ShaderParser_DEBUG_FLAG)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YY_ShaderParser_DEBUG != 0
  if (YY_ShaderParser_DEBUG_FLAG)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  YYGOTO(yybackup);
YYLABEL(yybackup)

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* YYLABEL(yyresume) */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yydefault);

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (YY_ShaderParser_CHAR == YYEMPTY)
    {
#if YY_ShaderParser_DEBUG != 0
      if (YY_ShaderParser_DEBUG_FLAG)
	fprintf(stderr, "Reading a token: ");
#endif
      YY_ShaderParser_CHAR = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (YY_ShaderParser_CHAR <= 0)           /* This means end of input. */
    {
      yychar1 = 0;
      YY_ShaderParser_CHAR = YYEOF;                /* Don't call YYLEX any more */

#if YY_ShaderParser_DEBUG != 0
      if (YY_ShaderParser_DEBUG_FLAG)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(YY_ShaderParser_CHAR);

#if YY_ShaderParser_DEBUG != 0
      if (YY_ShaderParser_DEBUG_FLAG)
	{
	  fprintf (stderr, "Next token is %d (%s", YY_ShaderParser_CHAR, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, YY_ShaderParser_CHAR, YY_ShaderParser_LVAL);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    YYGOTO(yydefault);

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrlab);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrlab);

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YY_ShaderParser_DEBUG != 0
  if (YY_ShaderParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting token %d (%s), ", YY_ShaderParser_CHAR, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (YY_ShaderParser_CHAR != YYEOF)
    YY_ShaderParser_CHAR = YYEMPTY;

  *++yyvsp = YY_ShaderParser_LVAL;
#ifdef YY_ShaderParser_LSP_NEEDED
  *++yylsp = YY_ShaderParser_LLOC;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  YYGOTO(yynewstate);

/* Do the default action for the current state.  */
YYLABEL(yydefault)

  yyn = yydefact[yystate];
  if (yyn == 0)
    YYGOTO(yyerrlab);

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
YYLABEL(yyreduce)
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YY_ShaderParser_DEBUG != 0
  if (YY_ShaderParser_DEBUG_FLAG)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
{;
    break;}
case 2:
{;
    break;}
case 3:
{
	m_current_shader = dwarf::HShader(new Shader());

	if(shader_name == String(yyvsp[-3].str)) m_active_shader = TRUE;
	else m_active_shader = FALSE;
;
    break;}
case 4:
{
	if(m_active_shader)
	{
		m_shader_found = TRUE;
		m_shader = m_current_shader;
	}
;
    break;}
case 7:
{;
    break;}
case 8:
{;
    break;}
case 9:
{
	vtx_trans.m_transform = yyvsp[-10].vtx_transform;
	vtx_trans.m_wave[0] = *yyvsp[-7].wave;
	vtx_trans.m_wave[1] = *yyvsp[-5].wave;
	vtx_trans.m_wave[2] = *yyvsp[-3].wave;
	m_current_shader->m_vertex_transformations.push_front(vtx_trans);
;
    break;}
case 10:
{
	m_current_layer = dwarf::Shader::Layer();
;
    break;}
case 11:
{
	m_current_shader->AddLayer(m_current_layer);
;
    break;}
case 12:
{ 
	m_current_layer = dwarf::Shader::Layer();
;
    break;}
case 13:
{
	m_current_shader->AddLayer(m_current_layer);
;
    break;}
case 14:
{;
    break;}
case 15:
{;
    break;}
case 16:
{
	if(m_active_shader) 
	{
		m_current_layer.m_textures.AddItem(HTexture(String((char *)yyvsp[0].str)));
	}
;
    break;}
case 17:
{
	if(m_active_shader) 
	{
		m_current_layer.m_textures.AddItem(HTexture(String((char *)yyvsp[0].str)));
	}
;
    break;}
case 18:
{ m_current_layer.m_cull_faces = dwarf::RenderingDevice::FRONT; m_current_layer.m_cull = true; ;
    break;}
case 19:
{ m_current_layer.m_cull_faces = dwarf::RenderingDevice::BACK; m_current_layer.m_cull = true; ;
    break;}
case 20:
{ m_current_layer.m_cull = false; ;
    break;}
case 21:
{ m_current_layer.m_fillmode = dwarf::RenderingDevice::FILL; ;
    break;}
case 22:
{ m_current_layer.m_fillmode = dwarf::RenderingDevice::LINE; ;
    break;}
case 23:
{ 
		if(m_active_shader) 
		{
			m_current_layer.m_textures.AddItem(HTexture(String((char *)yyvsp[-1].str)));
		}
	;
    break;}
case 24:
{
		m_current_layer.m_is_animated = TRUE;
		m_current_layer.m_framerate = yyvsp[-2].number;
	;
    break;}
case 25:
{ m_current_layer.m_line_width = yyvsp[-1].number; ;
    break;}
case 26:
{ m_current_layer.m_line_width = yyvsp[-1].integer; ;
    break;}
case 27:
{ m_current_layer.m_uvgen = Shader::Layer::ENVMAP; ;
    break;}
case 28:
{ m_current_layer.m_uvgen = Shader::Layer::NORMAL; ;
    break;}
case 29:
{ m_current_layer.m_blend_src = yyvsp[-4].blendmode; m_current_layer.m_blend_dst = yyvsp[-2].blendmode; ;
    break;}
case 30:
{ m_current_layer.m_depth_func = yyvsp[-1].depthtest; m_current_layer.m_depth_test = true; ;
    break;}
case 31:
{
		trans.m_transform = static_cast<Shader::Layer::tex_transform>(yyvsp[-6].transform);
		trans.m_wave = Shader::Wave(*yyvsp[-4].wave);
		delete yyvsp[-4].wave;
		trans.m_direction = Point(yyvsp[-2].vec2d.x,yyvsp[-2].vec2d.y);
		m_current_layer.m_uv_transformations.push_front(trans);
	;
    break;}
case 32:
{
		trans.m_transform = Shader::Layer::ROTATE;
		trans.m_wave = *yyvsp[-2].wave;
		delete yyvsp[-2].wave;
		m_current_layer.m_uv_transformations.push_front(trans);
	;
    break;}
case 33:
{
		if(yyvsp[-6].wave) m_current_layer.m_rgba_wave[0] = *yyvsp[-6].wave;
		if(yyvsp[-4].wave) m_current_layer.m_rgba_wave[1] = *yyvsp[-4].wave;
		if(yyvsp[-2].wave) m_current_layer.m_rgba_wave[2] = *yyvsp[-2].wave;
		m_current_layer.m_rgba_wave[3] = Shader::Wave(Shader::Wave::CONSTANT,1.0f);

		delete yyvsp[-6].wave;
		delete yyvsp[-4].wave;
		delete yyvsp[-2].wave;
	;
    break;}
case 34:
{
		if(yyvsp[-8].wave) m_current_layer.m_rgba_wave[0] = *yyvsp[-8].wave;
		if(yyvsp[-6].wave) m_current_layer.m_rgba_wave[1] = *yyvsp[-6].wave;
		if(yyvsp[-4].wave) m_current_layer.m_rgba_wave[2] = *yyvsp[-4].wave;
		if(yyvsp[-2].wave) m_current_layer.m_rgba_wave[3] = *yyvsp[-2].wave;

		delete yyvsp[-8].wave;
		delete yyvsp[-6].wave;
		delete yyvsp[-4].wave;
		delete yyvsp[-2].wave;
	;
    break;}
case 35:
{ m_current_layer.m_wrapmode = Texture::WRAP_S_WRAP_T; ;
    break;}
case 36:
{ m_current_layer.m_wrapmode = Texture::WRAP_S_CLAMP_T; ;
    break;}
case 37:
{ m_current_layer.m_wrapmode = Texture::CLAMP_S_WRAP_T; ;
    break;}
case 38:
{ m_current_layer.m_wrapmode = Texture::CLAMP_S_CLAMP_T; ;
    break;}
case 39:
{ yyval.vtx_transform = Shader::VROTATE; ;
    break;}
case 40:
{ yyval.vtx_transform = Shader::VMOVE; ;
    break;}
case 41:
{ yyval.vtx_transform = Shader::VSCALE; ;
    break;}
case 42:
{ yyval.wave = NULL; ;
    break;}
case 43:
{ yyval.wave = yyvsp[0].wave; ;
    break;}
case 44:
{ yyval.vec2d.x = yyvsp[-3].number; yyval.vec2d.y = yyvsp[-1].number; ;
    break;}
case 45:
{ yyval.vec3d.x = yyvsp[-5].number; yyval.vec3d.y = yyvsp[-3].number; yyval.vec3d.z = yyvsp[-1].number; ;
    break;}
case 46:
{ yyval.vec4d.x = yyvsp[-7].number; yyval.vec4d.y = yyvsp[-5].number; yyval.vec4d.z = yyvsp[-3].number; yyval.vec4d.w = yyvsp[-1].number; ;
    break;}
case 47:
{ yyval.transform = Shader::Layer::SCROLL; ;
    break;}
case 48:
{ yyval.transform = Shader::Layer::STRETCH; ;
    break;}
case 49:
{;
    break;}
case 50:
{;
    break;}
case 51:
{ yyval.number = yyvsp[0].number; ;
    break;}
case 52:
{ yyval.number = yyvsp[0].integer; ;
    break;}
case 53:
{ yyval.number = yyvsp[-2].number * yyvsp[0].number; ;
    break;}
case 54:
{ yyval.number = yyvsp[-2].number / yyvsp[0].number; ;
    break;}
case 55:
{ yyval.number = yyvsp[-2].number + yyvsp[0].number; ;
    break;}
case 56:
{ yyval.number = yyvsp[-2].number - yyvsp[0].number; ;
    break;}
case 57:
{ yyval.number = -yyvsp[0].number; ;
    break;}
case 58:
{ yyval.number = yyvsp[0].number; ;
    break;}
case 59:
{ yyval.number = yyvsp[-1].number; ;
    break;}
case 60:
{ yyval.blendmode = RenderingDevice::ONE; ;
    break;}
case 61:
{ yyval.blendmode = RenderingDevice::ZERO; ;
    break;}
case 62:
{ yyval.blendmode = RenderingDevice::SRC_ALPHA; ;
    break;}
case 63:
{ yyval.blendmode = RenderingDevice::DST_ALPHA; ;
    break;}
case 64:
{ yyval.blendmode = RenderingDevice::SRC_COLOR; ;
    break;}
case 65:
{ yyval.blendmode = RenderingDevice::DST_COLOR; ;
    break;}
case 66:
{ yyval.blendmode = RenderingDevice::ONE_MINUS_SRC_ALPHA; ;
    break;}
case 67:
{ yyval.blendmode = RenderingDevice::ONE_MINUS_DST_ALPHA; ;
    break;}
case 68:
{ yyval.blendmode = RenderingDevice::ONE_MINUS_SRC_COLOR; ;
    break;}
case 69:
{ yyval.blendmode = RenderingDevice::ONE_MINUS_DST_COLOR; ;
    break;}
case 70:
{ yyval.depthtest = RenderingDevice::LESS; ;
    break;}
case 71:
{ yyval.depthtest = RenderingDevice::GREATER; ;
    break;}
case 72:
{ yyval.depthtest = RenderingDevice::EQUAL; ;
    break;}
case 73:
{ yyval.depthtest = RenderingDevice::LESS_OR_EQUAL; ;
    break;}
case 74:
{ yyval.depthtest = RenderingDevice::GREATER_OR_EQUAL; ;
    break;}
case 75:
{ yyval.depthtest = RenderingDevice::NOT_EQUAL; ;
    break;}
case 76:
{ yyval.depthtest = RenderingDevice::ALWAYS; ;
    break;}
case 77:
{ yyval.depthtest = RenderingDevice::NEVER; ;
    break;}
case 78:
{ yyval.alphagen = Shader::Layer::SIN; ;
    break;}
case 79:
{ yyval.alphagen = Shader::Layer::SQUARE; ;
    break;}
case 80:
{ yyval.alphagen = Shader::Layer::TRIANGLE; ;
    break;}
case 81:
{ yyval.alphagen = Shader::Layer::SAWTOOTH; ;
    break;}
case 82:
{ yyval.alphagen = Shader::Layer::INVERSE_SAWTOOTH; ;
    break;}
case 83:
{
	yyval.wave = new Shader::Wave(Shader::Wave::SINUS,yyvsp[-7].number,yyvsp[-5].number,yyvsp[-3].number,yyvsp[-1].number);
;
    break;}
case 84:
{
	yyval.wave = new Shader::Wave(Shader::Wave::LINEAR,yyvsp[-1].number);
;
    break;}
case 85:
{
	yyval.wave = new Shader::Wave(Shader::Wave::TRIANGLE,yyvsp[-7].number,yyvsp[-5].number,yyvsp[-3].number,yyvsp[-1].number);
;
    break;}
case 86:
{
	yyval.wave = new Shader::Wave(Shader::Wave::SAWTOOTH,yyvsp[-7].number,yyvsp[-5].number,yyvsp[-3].number,yyvsp[-1].number);
;
    break;}
case 87:
{
	yyval.wave = new Shader::Wave(Shader::Wave::INVERSE_SAWTOOTH,yyvsp[-7].number,yyvsp[-5].number,yyvsp[-3].number,yyvsp[-1].number);
;
    break;}
case 88:
{
	yyval.wave = new Shader::Wave(Shader::Wave::SQUARE,yyvsp[-7].number,yyvsp[-5].number,yyvsp[-3].number,yyvsp[-1].number,0.5f);
;
    break;}
case 89:
{
	yyval.wave = new Shader::Wave(Shader::Wave::SQUARE,yyvsp[-9].number,yyvsp[-7].number,yyvsp[-5].number,yyvsp[-3].number,yyvsp[-1].number);
;
    break;}
case 90:
{
	yyval.wave = new Shader::Wave(Shader::Wave::CONSTANT,yyvsp[0].number);
;
    break;}
}
   /* the action file gets copied in in place of this dollarsign  */
  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YY_ShaderParser_LSP_NEEDED
  yylsp -= yylen;
#endif

#if YY_ShaderParser_DEBUG != 0
  if (YY_ShaderParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YY_ShaderParser_LSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = YY_ShaderParser_LLOC.first_line;
      yylsp->first_column = YY_ShaderParser_LLOC.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  YYGOTO(yynewstate);

YYLABEL(yyerrlab)   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++YY_ShaderParser_NERRS;

#ifdef YY_ShaderParser_ERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      YY_ShaderParser_ERROR(msg);
	      free(msg);
	    }
	  else
	    YY_ShaderParser_ERROR ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YY_ShaderParser_ERROR_VERBOSE */
	YY_ShaderParser_ERROR("parse error");
    }

  YYGOTO(yyerrlab1);
YYLABEL(yyerrlab1)   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (YY_ShaderParser_CHAR == YYEOF)
	YYABORT;

#if YY_ShaderParser_DEBUG != 0
      if (YY_ShaderParser_DEBUG_FLAG)
	fprintf(stderr, "Discarding token %d (%s).\n", YY_ShaderParser_CHAR, yytname[yychar1]);
#endif

      YY_ShaderParser_CHAR = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;              /* Each real token shifted decrements this */

  YYGOTO(yyerrhandle);

YYLABEL(yyerrdefault)  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) YYGOTO(yydefault);
#endif

YYLABEL(yyerrpop)   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YY_ShaderParser_LSP_NEEDED
  yylsp--;
#endif

#if YY_ShaderParser_DEBUG != 0
  if (YY_ShaderParser_DEBUG_FLAG)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

YYLABEL(yyerrhandle)

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    YYGOTO(yyerrdefault);

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    YYGOTO(yyerrdefault);

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	YYGOTO(yyerrpop);
      yyn = -yyn;
      YYGOTO(yyreduce);
    }
  else if (yyn == 0)
    YYGOTO(yyerrpop);

  if (yyn == YYFINAL)
    YYACCEPT;

#if YY_ShaderParser_DEBUG != 0
  if (YY_ShaderParser_DEBUG_FLAG)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = YY_ShaderParser_LVAL;
#ifdef YY_ShaderParser_LSP_NEEDED
  *++yylsp = YY_ShaderParser_LLOC;
#endif

  yystate = yyn;
  YYGOTO(yynewstate);
/* end loop, in which YYGOTO may be used. */
  YYENDGOTO
}

/* END */
