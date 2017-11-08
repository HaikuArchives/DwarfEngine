#ifndef D_TEXTBUFFER_H_
#define D_TEXTBUFFER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "Defs.h"

#include "Buffer.h"


namespace dwarf
{

		/*!
			It supplies some methods that treats the buffer as a
			plain ascii-file.
			\brief this class is used to parse (simple) text files
		*/

		class TextBuffer : public Buffer
		{
			public:
		
			TextBuffer(int32 size = 0) : Buffer(size) {};
			TextBuffer(const void *buffer, int32 size) : Buffer(buffer, size) {};
			TextBuffer(const TextBuffer &buf) : Buffer(buf) {};

			// used to parse simple files
			String ReadString();
			int32 ReadInteger();
			float ReadFloat();
			double ReadDouble();
			void SkipTokens(int32 num);

			String ReadLine();

			protected:

			void ReadString(Array<char> &string, bool ignore_spaces = true, const char *separators = " \t\r\n");

		};

} // dwarf


#endif //D_TEXTBUFFER_H_
