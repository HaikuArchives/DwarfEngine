/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"


#include "StringTokenizer.h"

using namespace dwarf;

StringTokenizer::StringTokenizer(const String& str, const String& delimiter, const String &strip):
	m_ntokens(0)
{

	String token;
	
	for(int32 i = 0; i < str.Length(); i++)
	{
		bool stripit = false;

		for(int32 d = 0; d < delimiter.Length(); d++)
		{
			if(str[i] == delimiter[d])
			{
				m_tokens.AddItem(token);
				token.Empty();
				m_ntokens++;
				i++;
			}
		}

		for(int32 s = 0; s < strip.Length(); s++)
		{
			if(str[i] == strip[s])
			{
				stripit = true;
				break;
			}
		}
		if(!stripit)
			token << str[i];
	}
}