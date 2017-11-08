/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "String.h"
#include "Buffer.h"
#include "Log.h"

using namespace dwarf;




#ifdef __BEOS__

/**********************************************************
 *
 *        BEOS PART
 *
 **********************************************************/


#ifdef D_UNICODE

	String::String(const char *str):
		m_length(0),
		m_string(strlen(str) * 2 + 1 + BLOCK_SIZE)
	{
		int32 state;
		int32 converted_length = m_string.Length() << 1;
		int32 length = strlen(str);
		convert_from_utf8(B_UNICODE_CONVERSION, str, &length, (char*)*m_string, &converted_length, &state);
		m_length = converted_length >> 1;
		m_string[m_length] = 0;
	
		#ifdef D_LITTLE_ENDIAN
	
		int32 i = 0;
		while(i < m_length)
		{
			m_string[i] = swap_byteorder(m_string[i]);
			i++;
		}
	
		#endif
	
	}


	/*!
		\brief Converts the string to a char*-array and writes it into the buffer.
		\param buffer the buffer to write the converted string into
		\param max_size the size of the buffer
		\return a pointer to the given buffer
	 */
	char *String::ToChar(char *buffer, int32 max_size) const
	{
		int32 dest_len = max_size - 1;
		int32 state;
		int32 length = Size();
	
		#ifdef D_LITTLE_ENDIAN
	
		dchar *str = (dchar*)buffer;
		for(int32 i = 0; i < min(m_length, max_size>>1); i++)
			str[i] = swap_byteorder(m_string[i]);
	
		convert_to_utf8(B_UNICODE_CONVERSION, buffer, &length, buffer, &dest_len, &state);
	
		#else
	
		convert_to_utf8(B_UNICODE_CONVERSION, m_string.Data(), &length, buffer, &dest_len, &state);
	
		#endif
	
		buffer[dest_len] = 0;
	
		return buffer;
	}

	/*!
		That means that the string's content will be replaced.
		\brief works just like vsprintf() but writes the output into this string
		\param format the format string
		\param vl the va_list
	*/
	void String::vaFormat(const dchar *format, va_list vl)
	{
		String string(format);
		char *buffer = new char[string.Size()];
		char buffer2[1024];
		
		string.ToChar(buffer, string.Size());

		// convert "%s" -> "%S", "%S" -> "%s"
		bool last_percent = false;
		for(char *a = buffer; *a != '\0'; a++)
		{
			if (*a == '%' && last_percent) last_percent = false;
			else if (*a == '%' && !last_percent) last_percent = true;
			else if (*a == 's' && last_percent) { *a = 'S'; last_percent = false; }
			else if (*a == 'S' && last_percent) { *a = 's'; last_percent = false; }
		}

		vsprintf(buffer2, buffer, vl);
		*this = buffer2;
		delete []buffer;
	}

	String::String(const char *str, int32 length):
		m_length(0),
		m_string(length + 1 + BLOCK_SIZE)
	{
		int32 state;
		int32 converted_length = m_string.Length() << 1;
		convert_from_utf8(B_UNICODE_CONVERSION, str, &length, (char *)*m_string, &converted_length, &state);
		m_length = converted_length >> 1;
		m_string[m_length] = 0;
		
		#ifdef D_LITTLE_ENDIAN
	
		int32 i = 0;
		while(i < m_length)
		{
			m_string[i] = swap_byteorder(m_string[i]);
			i++;
		}
	
		#endif
	}
	
	const String &String::operator=(const char *str)
	{
		m_length = 0;
		int32 new_length = strlen(str) * 2 + 1;
		if (m_string.Length() < new_length) m_string.SetLength(new_length + BLOCK_SIZE);
	
		int32 state;
		int32 length = strlen(str);
		convert_from_utf8(B_UNICODE_CONVERSION, str, &length, (char*)*m_string, &new_length, &state);
		m_length = new_length >> 1;
		m_string[m_length] = 0;
	
		#ifdef D_LITTLE_ENDIAN
	
		int32 i = 0;
		while(i < m_length)
		{
			m_string[i] = swap_byteorder(m_string[i]);
			i++;
		}
	
		#endif
	
		return *this;
	}
	
	const String &String::operator+=(const char* str)
	{
		int32 string_length = strlen(str) * 2;
		if (m_string.Length() - m_length < string_length) m_string.SetLength(m_length + string_length + BLOCK_SIZE);
	
		int32 state;
		int32 length = strlen(str);
		convert_from_utf8(B_UNICODE_CONVERSION, str, &length, (char*)(*m_string + m_length), &string_length, &state);
		m_length += string_length >> 1;
		m_string[m_length] = 0;
	
		#ifdef D_LITTLE_ENDIAN
	
		int32 i = m_length - (string_length >> 1);
		while(i < m_length)
		{
			m_string[i] = swap_byteorder(m_string[i]);
			i++;
		}
	
		#endif
	
		return *this;
	}

#else

	String::String(const char *str):
		m_length(strlen(str)),
		m_string(m_length * 2 + 1 + BLOCK_SIZE)
	{
		strcpy(*m_string, str);
	}

	/*!
		\brief Converts the string to a char*-array and writes it into the buffer.
		\param buffer the buffer to write the converted string into
		\param max_size the size of the buffer
		\return a pointer to the given buffer
	 */
	char *String::ToChar(char *buffer, int32 max_size) const
	{
		strncpy(buffer, m_string.Data(), max_size);
		return buffer;
	}

	/*!
		That means that the string's content will be replaced.
		\brief works just like vsprintf() but writes the output into this string
		\param fotmat the format string
		\param vl the va_list
	*/
	void String::vaFormat(const dchar *format, va_list vl)
	{
		dchar buf[1024];
#ifdef D_UNICODE
		vswprintf(buf, format, vl);
#else
		vsprintf(buf, format, vl);
#endif
		*this = buf;
	}


#endif

#else

/**********************************************************
 *
 *        WIN32 PART
 *
 **********************************************************/

#ifdef D_UNICODE
#define STRCPY wcscpy
#define VSPRINTF vswprintf
#else
#define STRCPY strcpy
#define VSPRINTF vsprintf
#endif

/*!
	\brief Converts the string to a char*-array and writes it into the buffer.
	\param buffer the buffer to write the converted string into
	\param max_size the size of the buffer
	\return a pointer to the given buffer
 */
char *String::ToChar(char *buffer, int32 max_size) const
{
	for(int i = 0; i < min(max_size, m_length+1); i++)
		buffer[i] = m_string[i];
	return buffer;
}

/*!
	That means that the string's content will be replaced.
	\brief works just like vsprintf() but writes the output into this string
	\param fotmat the format string
	\param vl the va_list
*/
void String::vaFormat(const dchar *format, va_list vl)
{
	static dchar buf[10240];
	VSPRINTF(buf, format, vl);
	*this = buf;
}

#ifdef D_UNICODE


	String::String(const char *str):
		m_length(strlen(str)),
		m_string(m_length + 1 + BLOCK_SIZE)
	{
		for (int32 i = 0; i < m_length; i++)
			m_string[i] = str[i];
		m_string[m_length] = 0;
	}


	String::String(const char *str, int32 length):
		m_length(length),
		m_string(length + 1 + BLOCK_SIZE)
	{
		for (int32 i = 0; i < m_length; i++)
			m_string[i] = str[i];
		m_string[m_length] = 0;
	}
	
	const String &String::operator=(const char *str)
	{
		m_length = strlen(str);
		if (m_string.Length() < m_length) m_string.SetLength(m_length + 1 + BLOCK_SIZE);
	
		for (int32 i = 0; i < m_length; i++)
			m_string[i] = str[i];
		m_string[m_length] = 0;
	
		return *this;
	}
	
	const String &String::operator+=(const char* str)
	{
		int32 string_length = strlen(str);
		if (m_string.Length() - m_length < string_length) m_string.SetLength(m_length + string_length + BLOCK_SIZE);
	
		int32 j = m_length;
		for (int32 i = 0; i < string_length; i++)
			m_string[j++] = str[i];
		m_length += string_length;
	
		return *this;
	}
#else

	String::String(const uint16 *str):
		m_length(wstrlen(str)),
		m_string(m_length + 1 + BLOCK_SIZE)
	{
		for (int32 i = 0; i < m_length; i++)
			m_string[i] = str[i] >> 8;
		m_string[m_length] = 0;
	}


	String::String(const uint16 *str, int32 length):
		m_length(length),
		m_string(length + 1 + BLOCK_SIZE)
	{
		for (int32 i = 0; i < m_length; i++)
			m_string[i] = str[i] >> 8;
		m_string[m_length] = 0;
	}

#endif

#endif

/**********************************************************
 *
 *        INDEPENDENT PART
 *
 **********************************************************/

/*!
	\brief destructs the string
*/
String::~String()
{
}

/*!
	The given string doesn't have to be null-terminated, because
	you also give it's length.
	\brief copies the given string into this string
	\param str A dchar pointer (a pointer to an array of dchars)
	\param length the number of characters in the given string
*/
String::String(const dchar *str, int32 length):
	m_length(length),
	m_string(length + 1 + BLOCK_SIZE)
{
	for (int32 i = 0; i < m_length; i++)
		m_string[i] = str[i];
	m_string[m_length] = 0;
}

String::String(const dchar *str):
	m_length(dstrlen(str)),
	m_string(m_length + 1 + BLOCK_SIZE)
{
	for (int32 i = 0; i < m_length; i++)
		m_string[i] = str[i];
	m_string[m_length] = 0;
}

/*!
	\brief copy constructor
	\param str The string to copy
*/
String::String(const String &str):
	m_length(str.m_length),
	m_string(str.m_string)
{
}

String::String(const String &str1, const String &str2):
	m_length(str1.m_length + str2.m_length),
	m_string(str1.m_length + str2.m_length + 1 + BLOCK_SIZE)
{
	if(m_length)
	{
		int32 i;
		int32 j = 0;
		for (i = 0; i < str1.m_length; i++)
			m_string[j++] = str1.m_string.At(i);

		for (i = 0; i < str2.m_length; i++)
			m_string[j++] = str2.m_string.At(i);
	}
	m_string[m_length] = 0;
}

bool String::StartsWith(const String &string)
{
	if (string.Length() > Length()) return false;
	for (int32 i = 0; i < string.Length(); i++)
		if (m_string[i] != string[i]) return false;
	return true;
}

/*!
	That means that the string's content will be replaced.
	\brief works just like sprintf() but writes the output into this string
	\param format the format string
*/
void String::Format(const dchar *format, ...)
{
	va_list vl;
	va_start(vl, format);
	vaFormat(format, vl);
	va_end(vl);
}

/*!
	The parameters are filled with the values the parser found. Just like
	sscanf().
	\brief Parses the string's content according to the format string
	\param format a format-string
	\todo This method is not implemented yet
*/
void String::ScanFormat(const dchar *format, ...) const
{
	//TODO: this isn't even a hack yet :P
}

void String::FromFloat(float value)
{
	//TODO: this is just a hack
	Format(TEXT("%f"), value);
}

void String::FromInt(int32 value)
{
	//TODO: this is just a hack
	Format(TEXT("%d"), value);
}

float String::ToFloat() const
{
	//TODO: this is just a hack
#ifdef D_UNICODE
	#ifdef WIN32
		uint16* end = *m_string;
		float ret = wcstod(*m_string, &end);
	#else
		char *buf = new char[m_string.Length()];
		ToChar(buf, m_string.Length());
		float ret = atof(buf);
		delete []buf;
	#endif
#else
	float ret = atof(*m_string);
#endif
	return ret;
}

int32 String::ToInt() const
{
	//TODO: this is just a hack
#ifdef D_UNICODE
	#ifdef WIN32
		int ret = _wtoi(*m_string);
	#else
		char *buf = new char[m_string.Length()];
		ToChar(buf, m_string.Length());
		int ret = atoi(buf);
		delete []buf;
	#endif
#else
	int ret = atoi(*m_string);
#endif
	return ret;
}

/*!
	\brief Empties the string of all it's content
*/
void String::Empty()
{
	if (m_string.Length() > 20 + BLOCK_SIZE ) m_string.SetLength(20 + BLOCK_SIZE);
	m_length = 0;
	m_string[0] = 0;
}

/*!
	\brief replaces first occurance of a character with another one
	\param exists the character to replace
	\param with the characte to replace the character with
	\return true if a character was replaced and false if the character wasn't found
*/
bool String::Replace(dchar exists, dchar with)
{
	if (exists == 0) return false;

	for (int32 i = 0; i < m_length; i++)
		if (m_string[i] == exists)
		{
			m_string[i] = with;
			return true;
		}

	return false;
}

int32 String::ReplaceAll(dchar exists, dchar with)
{
	if (exists == 0) return 0;

	int sum = 0;

	while(Replace(exists, with))
		sum++;
	return sum;
}

void String::AppendSpaces(uint32 num_spaces)
{
	int32 new_length = m_length + num_spaces;
	if (m_string.Length() < new_length + 1) m_string.SetLength(new_length + 1 + BLOCK_SIZE);

	for (int32 i = m_length; i < new_length + 1; i++)
		m_string[i] = ' ';

	m_length = new_length;
	m_string[m_length] = 0;
}

void String::Insert(int32 offset, const String &str)
{
	D_ASSERT_MESSAGE(offset >= 0, TEXT("Cannot insert a string at offset < 0"));
	int32 new_length = max(offset + str.Length(), m_length);

	if (m_string.Length() < new_length) m_string.SetLength(new_length + 1 + BLOCK_SIZE);

	if (m_length < offset)
		for (int32 i = m_length; i < offset; i++)
			m_string[i] = CHAR(' ');

	int32 j = 0;
	for (int32 i = offset; i < offset + str.Length(); i++)
		m_string[i] = str.m_string.At(j++);

	m_length = new_length;
	m_string[m_length] = 0;
}

void String::Insert(int32 offset, const dchar *str, int32 length)
{
	D_ASSERT_MESSAGE(offset >= 0, TEXT("Cannot insert a string at offset < 0"));
	int32 new_length = max(offset + length, m_length);

	logger(TEXT("%d = max(%d + %d, %d)"), new_length, offset, length, m_length);
	logger(TEXT("if(%d < %d) m_string.SetLength(%d + %d)"), m_string.Length(), new_length, new_length, BLOCK_SIZE);

	if (m_string.Length() < new_length) m_string.SetLength(new_length + 1 + BLOCK_SIZE);

	if (m_length < offset)
		for (int32 i = m_length; i < offset; i++)
			m_string[i] = L' ';

	int32 j = 0;
	for (int32 i = offset; i < offset + length; i++)
		m_string[i] = str[j++];

	m_length = new_length;
	m_string[m_length] = 0;
}

	/*!	Serialize and Restore always work on
	*	Unicode strings, so that saves will
	*	be compatible between builds.
	*/

void String::Restore(InStream &stream)// throw (Buffer::IllegalPop)
{
	int32 new_length;
	stream >> new_length;
	
	if (new_length > m_string.Length()) m_string.SetLength(new_length + 1 + BLOCK_SIZE);

	m_length = new_length;
	for (int32 i = 0; i < Length(); i++)
	{
#ifdef D_UNICODE
		stream >> m_string[i];
#else
		uint16 ch;
		stream >> ch;
		m_string[i] = ch;
#endif
	}
	
	m_string[m_length] = 0;
}

void String::Serialize(OutStream &stream) const
{
	stream << m_length;
	for (int32 i = 0; i < Length(); i++)
	{
#ifdef D_UNICODE
		stream << m_string[i];
#else
		uint16 ch = m_string[i];
		stream << ch;
#endif
	}
}

const String& String::TrimLeft(int32 num) // trim chars off the beginning of the string
{
	D_ASSERT_MESSAGE(num <= m_length, TEXT("You are trying to remove more characters from the string than the length of the string"))

	if (num == 0) return *this;
	if (num == m_length)
	{
		Empty();
		return *this;
	}
	return *this = SubString(num, m_length - num);
}

const String& String::TrimRight(int32 num)
{
	if (m_length)
	{
		if (num >= m_length) num = m_length;
		m_string[m_length - num] = 0;
		m_length -= num;
	}
	return *this;
}

const String &String::operator-=(int32 num) // trims num chars off the end of the string
{
	return TrimRight(num);
}

const String &String::operator+=(const String &str)
{
	int32 new_length = m_length + str.m_length;
	if (m_string.Length() < new_length + 1) m_string.SetLength(new_length + 1 + BLOCK_SIZE);
	
	int32 j = 0;
	for (int32 i = m_length; i < new_length + 1; i++)
		m_string[i] = str.m_string.At(j++);

	m_length = new_length;
	m_string[m_length] = 0;
	
	return *this;
}

const String &String::operator+=(const dchar* str)
{
	*this += String(str);
	return *this;
}

String &String::operator<<(dchar character)
{
	if (m_string.Length() < m_length + 2) m_string.SetLength(m_length + 2 + BLOCK_SIZE);

	m_string[m_length] = character;
	m_length++;
	m_string[m_length] = 0;
	return *this;
}

const String &String::operator=(const String &str)
{
	m_length = str.m_length;
	if (m_string.Length() < m_length + 1) m_string.SetLength(m_length + 1 + BLOCK_SIZE);
	for (int32 i = 0; i < m_length + 1; i++)
		m_string[i] = str.m_string[i];
	return *this;
}

const String &String::operator=(const dchar *str)
{
	m_length = dstrlen(str);
	if(m_string.Length() < m_length + 1) m_string.SetLength(m_length + 1 + BLOCK_SIZE);
	for(int32 i = 0; i < m_length + 1; i++)
		m_string[i] = str[i];
	return *this;
}

bool String::operator==(const String &str) const
{
	return (m_length == str.m_length) && (memcmp(*m_string, *str.m_string, Size()) == 0);
}

bool String::operator!=(const String &str) const
{
	return (m_length != str.m_length) || (memcmp(*m_string, *str.m_string, Size()) != 0);
}

bool String::operator>=(const String &str) const
{
	for (int32 i = 0; i < Length(); i++)
		if (m_string[i] < str[i]) return false;
	return true;
}

bool String::operator<=(const String &str) const
{
	for (int32 i = 0; i < Length(); i++)
		if (m_string[i] > str[i]) return false;
	return true;
}

bool String::operator> (const String &str) const
{
	for (int32 i = 0; i < Length(); i++)
		if (m_string[i] > str[i]) return true;
	return false;
}

bool String::operator< (const String &str) const
{
	for (int32 i = 0; i < Length(); i++)
		if (m_string[i] < str[i]) return true;
	return false;
}


const String& String::ToLower()
{
	for (int i = 0; i < Length(); i++)
	{
		m_string[i] = tolower(m_string[i]);
	}
	return *this;
}

const String& String::ToUpper()
{
	for (int i = 0; i < Length(); i++)
	{
		m_string[i] = toupper(m_string[i]);
	}
	return *this;
}

int32 String::dstrlen(const dchar *string)
{
	int32 i = 0;
	while(string[i]) i++;
	return i;
}

int32 String::wstrlen(const uint16 *string)
{
	int32 i = 0;
	while(string[i]) i++;
	return i;
}

/*!
	A return value of -1 means that the substring wasn't found within the string.
*/

int32 String::Find(const dchar chr) const
{
	for (int32 i = 0; i < m_length; i++)
		if (m_string[i] == chr)
			return i;
	return -1;
}


int32 String::Find(const String &substr) const
{
	if (substr.m_length > m_length)
		return -1;

	int32 start = 0;
	int32 i = 0;

	while (start > 0 && start < m_length)
	{
		for (i=0; i<substr.m_length; i++)
			if (m_string[start+i] != substr[i])
				break;

		if (i == substr.m_length-1)
			return start;

		do
		{
			start++;
		}
		while (start != substr[0] && start != 0);
	}
	return -1;
}

/*
inline int32 String::rfind(const String &substr) const
{
	uint32 r = (std::string(m_str)).rfind(substr.CString());
	if(r < 0 || r >= len)
		return -1;
	return r;
}

inline int32 String::rfind(const char *substr) const
{
	uint32 r = (std::string(m_str)).rfind(substr);
	if(r < 0 || r >= len)
		return -1;
	return r;
}

inline int32 String::rfind(const char chr) const
{
	uint32 r = (std::string(m_str)).rfind(chr);
	if(r < 0 || r >= len)
		return -1;
	return r;
}

*/
