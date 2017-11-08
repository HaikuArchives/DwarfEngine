/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include "nommgr.h"
#include <list>
#include "mmgr.h"

#include "ShaderParser.h"

using namespace dwarf;

MyParser::MyParser():
	m_scanner(0),
	m_current_filename(0)
{
}

MyParser::~MyParser()
{
}

void MyParser::AddFile(FileSystem *dir, const String &filename)
{
	// add an empty buffer
	m_buffer_list.push_back(ListItem());
	ListItem &item = m_buffer_list.back();
	File file(dir, filename);
	// read the file into the added buffer
	item.filename = filename;
	file.Read(item.buffer);
}


HShader MyParser::operator()(const String &shader_name) throw (ParseError)
{
	m_shader_found = FALSE;
	
	for(std::list<ListItem>::iterator i = m_buffer_list.begin(); i != m_buffer_list.end(); i++)
	{
		ListItem &item = *i;
		item.buffer.Rewind();
		ShaderLexer scanner(item.buffer);
		m_scanner = &scanner;
		m_current_filename = &item.filename;
		
		if(yyparse(shader_name))
			throw ParseError(TEXT("unknown parse error"), TEXT("ShaderParser::operator()"));
		if (m_shader_found) break;
	}
	if(!m_shader_found)
	{
		String message;
		message.Format(TEXT("Shader not found \"%s\""), shader_name);
		throw ShaderNotFound(message, TEXT("ShaderParser::operator()"));
	}
	
	m_scanner = 0;
	return m_shader;
}
