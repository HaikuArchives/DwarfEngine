#ifndef D_STRING_H_
#define D_STRING_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Array.h"
#include "Serializable.h"

namespace dwarf
{

		class Buffer;
		class String;

		/*!
			String is implemented with unicode characters (2 bytes/character) to support 
			international characters.
			To gain performance, every time it need to resize it's buffer it allocates 
			BLOCK_SIZE characters more than needed.
	
			\brief repesents a string of characters
		*/
		
		class String : public Serializable
		{
		public:
		
			String();

			String(const dchar *str, int32 length);
			String(const dchar *str);
		
			String(const String &str);
			String(const String &str1, const String &str2);

			virtual ~String();

			const String &operator=(const String &str);
			const String &operator=(const dchar *str);

#ifdef D_UNICODE
			explicit String(const char *str); // the explicit is there to prevent auto conversion
			String(const char *str, int32 length);
			const String &operator=(const char *str);
			const String &operator+=(const char* str);
#else
			explicit String(const uint16 *str); // the explicit is there to prevent auto conversion
			String(const uint16 *str, int32 length);
#endif
			String operator+(const String &str) const;
			String operator+(const dchar *str) const;
			friend String operator+(const dchar *str1, const String &str2);

			const String &operator+=(const String &str);
			const String &operator+=(const dchar* str);

			void AppendSpaces(uint32 num_spaces);
			void Insert(int32 offset, const String &str);
			void Insert(int32 offset, const dchar *str, int32 length);

			uint16 operator[](int32 index) const;
			const uint16 At(uint32 index) const;
		
			bool Replace(dchar exists, dchar with);
			int32 ReplaceAll(dchar exists, dchar with);
		
			String SubString(int32 index, int32 length) const;
			String LeftString(int32 length) const;
			String RightString(int32 length) const;

			const String& TrimLeft(int32 length); // trim chars off the beginning of the string
			const String& TrimRight(int32 length);
			const String& operator-=(int32 num); // trims num chars off the end of the string

			const String& ToLower();
			const String& ToUpper();

			bool operator==(const dchar *str) const;
			bool operator==(const String &str) const;
			bool operator!=(const dchar *str) const;
			bool operator!=(const String &str) const;
			bool operator>=(const String &str) const;
			bool operator<=(const String &str) const;
			bool operator>(const String &str) const;
			bool operator<(const String &str) const;

			void Format(const dchar *format, ...);
			void vaFormat(const dchar *format, va_list vl);
			void ScanFormat(const dchar *format, ...) const;

			void FromFloat(float value);
			void FromInt(int32 value);
			float ToFloat() const;
			int32 ToInt() const;

			String &operator<<(const String &str);
			String &operator<<(const char *str);
			String &operator<<(dchar character);
		//	String &operator<<(int8 val);
		//	String &operator<<(int16 val);
		//	String &operator<<(int32 val);
		//	String &operator<<(int64 val);
		//	String &operator<<(float val);
		//	String &operator<<(double val);
		
			int32 Find(String const& substr) const;
			int32 Find(const dchar chr) const;
		
//			int32 RevFind(String const& substr) const;
//			int32 RevFind(const char chr) const;

			void Empty();
			bool IsEmpty() const;
			int32 Length() const;
			
			bool StartsWith(const String &string);
			int32 Size() const;
			
			const dchar *ToArray() const;
			
			dchar* operator*();
			const dchar* operator*() const;
			
			char *ToChar(char* buffer, int32 max_size) const; // writes contained string into buffer

			operator dchar* ();
			operator const dchar* () const;

			virtual void Restore(InStream &stream)/* throw (Buffer::IllegalPop)*/;
			virtual void Serialize(OutStream &stream) const;

		private:

//			void wcscpy(dchar *dest, const dchar *src);
			int32 dstrlen(const dchar *string); // returns the number of characters in the given string
			int32 wstrlen(const uint16 *string); // returns the number of characters in the given wide character string
			
			// allocates at least BLOCK_SIZE characters at a time
			enum
			{
				BLOCK_SIZE = 16//128
			};
		
			int32 m_length;	// the number of characters of the array that defines the string (the array may be larger)
			Array<dchar> m_string; 	// unicode string
		};


		/*!
			\brief initializes the string to be empty
		*/
		inline String::String():
			m_length(0),
			m_string(BLOCK_SIZE)
		{
			m_string[0] = 0;
		}

		/*!
			\brief tells you if this string is empty
			\return true if the string is empty, false otherwise
		*/
		inline bool String::IsEmpty() const
		{
			return m_length == 0;
		}
		
		/*!
			\brief tells you the number of characters in this string
			\return the number of characters in this string
		*/
		inline int32 String::Length() const
		{
			return m_length;
		}
		
		/*!
			\brief tells you the size, in bytes, of this string
			\return the size, in bytes, of this string
		*/
		inline int32 String::Size() const
		{
			return m_length * sizeof(dchar);
		}
		
		/*!
			Note that the byte-order is platform specific, unicode is
			defined to be big-endian, but if you use this string class on a
			little-endian machine you will get the unicode characters in little-
			endian format.
			\brief gives you read access to the unicode string
			\return a pointer to the null-terminated unicode string
		*/
		inline const dchar *String::ToArray() const
		{
			return *m_string;
		}
		
		/*!
			\brief returns the unicode character at the given position in the string
			\param index the index of the character you want
			\return the unicode character at the given position. Note that this method will return the unicode
			character no matter if D_UNICODE is definied or not.
			\warning there is no check to make sure the index is inside it's boundry, use Length()
			\sa Length()
		*/
		// this method cannot return a reference to the uint16, because
		// then it won't work in non-unicode mode.
		inline uint16 String::operator[](int32 index) const
		{
			return m_string[index];
		}
		
		/*!
			\brief returns the unicode character at the given position in the string
			\param index the index of the character you want
			\return the unicode character at the given position. Note that this method will return the unicode
			character no matter if D_UNICODE is definied or not.
			\warning there is no check to make sure the index is inside it's boundry, use Length()
			\sa Length()
		*/
		inline const uint16 String::At(uint32 index) const
		{
			#ifdef D_UNICODE
			return m_string.At(index);
			#else
			return m_string.At(index) >> 8;
			#endif
		}
		
		/*!
			That is, this string will be altered.
			\brief adds a string at the end
			\param str the string to add
			\returns a reference to this string (to enable you to add more strings for example)
		*/
		inline String &String::operator<<(const String &str)
		{
			*this += str;
			return *this;
		}
		
		/*!
			That is, this string will be altered. If compiled in D_UNICODE mode this string will be converted
			to unicode before it's added.
			\brief adds a string at the end
			\param str the string to add
			\returns a reference to this string (to enable you to add more strings for example)
		*/
		inline String &String::operator<<(const char *str)
		{
			*this += str;
			return *this;
		}

		/*!
			The string is null-terminated
			\brief returns the character pointer to the string
			\return a pointer to the string of characters
			\warning this pointer is only valid until next non-const method call on this string
			\sa Length
		*/
		inline dchar* String::operator*()
		{
			return *m_string;
		}

		/*!
			The string is null-terminated
			\brief returns the character pointer to the string
			\return a pointer to the string of characters
			\warning this pointer is only valid until next non-const method call on this string
			\sa Length
		*/
		inline const dchar* String::operator*() const
		{
			return *m_string;
		}

		/*!
			Note that this is a const method, nothis will be changed in this object.
			\brief adds this string with another string
			\param str the string to add this string with
			\return a new allocated string which contains this string and the given string
		*/
		inline String String::operator+(const String &str) const
		{
			return String(*this, str);
		}
		
		inline String String::operator+(const dchar *str) const
		{
			return String(*this, str);
		}
		
		inline String operator+(const dchar *str1, const String &str2)
		{
			return String(str1, str2);
		}
		
		/*!
			\brief returns a substring of this string
			\param offset the offset into this string to the first character of the substring. The offset
			must be less than the length of this string.
			\param length the length of the substring, if the offset + length is greater than this string's length
			the substring will be as long as possible to still fit into this string.
			\return the substring
			\sa Length() RightString() LeftString()
		*/
		inline String String::SubString(int32 offset, int32 length) const
		{
			D_ASSERT_MESSAGE(offset <= m_length, TEXT("You are trying to get a substring that is outside the string."));
			D_ASSERT_MESSAGE(length >= 0, TEXT("You are trying to get a substring with length shorter than 0."));
			return String(&m_string[offset], (m_length < offset + length)?m_length - offset: length);
		}
		
		/*!
			\brief returns a substring that starts in the beginning of this string
			\param length the length of the substring
			\return the subsring
		*/
		inline String String::LeftString(int32 length) const
		{
			return String(&m_string[0], (m_length < length)?m_length: length);
		}
		
		/*!
			\brief returns a substring that ends in the end of this string
			\param length the length of the substring
			\return the subsring
		*/
		inline String String::RightString(int32 length) const
		{
			int32 index = m_length - length;
			if (index < 0) index = 0;
			return String(&m_string[index], (m_length < index + length)?m_length - index: length);
		}

		inline bool String::operator!=(const dchar *str) const
		{
			return operator!=(String(str));
		}

		inline bool String::operator==(const dchar *str) const
		{
			return operator==(String(str));
		}

		/*!
			where dchar is either a uint8 or a uint16 (depending on the state of D_UNICODE)
			\brief automatically converts the string into a dchar pointer
		*/
		inline String::operator dchar* ()
		{
			return *m_string;
		}

		/*!
			where dchar is either a uint8 or a uint16 (depending on the state of D_UNICODE)
			\brief automatically converts the string into a const dchar pointer
		*/
		inline String::operator const dchar* () const
		{
			return *m_string;
		}

} // dwarf

#endif // D_STRING_H_
