#include "TextBuffer.h"
#include "Array.h"
#include "String.h"

using namespace dwarf;

/*!
	\brief read a string from the text buffer
	\return a string read from the textfile
	(treats spaces, tabs, carrige-returns and
	newlines as separators between strings).
*/

String TextBuffer::ReadString()
{
	Array<char> string;
	ReadString(string);

	return String(*string);
}

int32 TextBuffer::ReadInteger()
{
	Array<char> string;
	ReadString(string);

	return atoi(*string);
}

float TextBuffer::ReadFloat()
{
	Array<char> string;
	ReadString(string);

	return atof(*string);
}

double TextBuffer::ReadDouble()
{
	Array<char> string;
	ReadString(string);

	return atof(*string);
}

String TextBuffer::ReadLine()
{
	Array<char> string;
	ReadString(string, false, "\n\r");

	return String();
}

void TextBuffer::SkipTokens(int32 num)
{
	Array<char> string;
	for (int32 i = 0; i < num; i++)
		ReadString(string);
}

void TextBuffer::ReadString(Array<char> &string, bool ignore_spaces, const char *separators)
{
	if (EndOfBuffer()) return;

	uint8 c;
	*this >> c;

// ignore any spaces
	if (ignore_spaces) while(strchr(separators, c) != 0 && !EndOfBuffer()) *this >> c;
	
	if (EndOfBuffer()) return;


	int32 counter = 0;
	while(strchr(separators, c) == 0 && !EndOfBuffer())
	{
		// if the string need to be reallocated, increase it's size by 128 characters
		if (string.Length() <= counter) string.SetLength(string.Length() + 128);

		string[counter] = c;
		counter++;
		*this >> c;
	}

	if (string.Length() <= counter) string.SetLength(string.Length() + 128);
	string[counter] = 0;
}
