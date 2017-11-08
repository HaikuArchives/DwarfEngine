#ifndef D_SHADER_PARSER_H
#define D_SHADER_PARSER_H

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "Defs.h"

#include "Exception.h"
#include "String.h"
#include "Shader/Lexer.h"
#include "list.h"

namespace dwarf
{

	class MyParser : public ShaderParser
	{
	public:
		class ParseError : public Exception
		{
		public:
			ParseError(const String &message, const String &source):
				Exception(message, source, TEXT("ShaderParser::ParseError")) {};
		};

		class ShaderNotFound : public Exception
		{
		public:
			ShaderNotFound(const String &message, const String &source):
			  Exception(message, source, TEXT("ShaderParser::ShaderNotFound")) {};
		};
		
		MyParser();
		virtual ~MyParser();
		void AddFile(FileSystem *dir, const String &filename);
		HShader operator()(const String &shader_name) throw (ParseError);

	private:
		virtual int yylex()
		{
			int token = m_scanner->yylex(&yylval);
			return token;
		}
		
		virtual void yyerror(char *m)
		{
			String text(m);
			String err;
			err.Format(TEXT("Error \"%s\" while reading shader file \"%s\""), *text, **m_current_filename);
			throw ParseError(err, TEXT("ShaderParser::operator()"));
		}

		struct ListItem
		{
			Buffer buffer;
			String filename;
		};
		
		std::list<ListItem> m_buffer_list;
		ShaderLexer *m_scanner;
		String *m_current_filename;
	};

} // dwarf

#endif // D_SHADER_PARSER_H
