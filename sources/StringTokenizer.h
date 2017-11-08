#ifndef D_STRINGTOKENIZER_H_
#define D_STRINGTOKENIZER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "String.h"
#include "ListArray.h"


namespace dwarf
{
	
		/*!
			\brief StringTokenizer is used to parse text strings.
			\todo Implement. Redesign interface until perfect.
		 */

		class StringTokenizer
		{
		public:
			StringTokenizer(const String& str, const String& delimiter = TEXT(" \t\n"), const String &strip = TEXT(""));
			StringTokenizer(const StringTokenizer& s2);
			~StringTokenizer()
			{
			}

			StringTokenizer& operator=(const StringTokenizer& s2);

			const String &operator[](uint32 index) const			{ return m_tokens[index]; }
			int32 NumTokens() const									{ return m_ntokens; }

		private:
			int32 m_ntokens;
			ListArray<String> m_tokens;
		};

}
#endif //D_STRINGTOKENIZER_H_
