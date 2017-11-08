/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#ifndef SHADERLEXER_H_
#define SHADERLEXER_H_

#include "FlexLexer.h"
#include "InStream.h"
#include "Shader/Parser.h"

namespace dwarf
{
	class ShaderLexer : public yyFlexLexer
	{
	public:
		ShaderLexer(InStream &stream): yyFlexLexer(), m_stream(stream) {}

		virtual int ShaderLexer::yylex(YY_ShaderParser_STYPE *val);

	protected:

		virtual int LexerInput(char* buf, int max_size)
			{ return m_stream.Read((int8*)buf, max_size); }

	private:
		InStream &m_stream;
	};
}

#endif