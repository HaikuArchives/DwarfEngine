#ifndef D_EXCEPTION_H_
#define D_EXCEPTION_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



#include "String.h"

namespace dwarf
{

		/*!
			\brief exception baseclass
		*/
		class Exception
		{
			public:
	
			Exception(const Exception &exception);
			Exception(const String &user_message = TEXT("no reason specified"), const String &source = TEXT("unknown thrower"), const String &class_name = TEXT("Exception"));
			virtual ~Exception();

			const Exception &operator=(const Exception &exception);

			void OpenDialog() const;

			const String &Message() const;
			const String &Source() const;
			const String &Class() const;

			private:

			String m_message; // the message that may be displayed to the user
			String m_source; // the name of the thrower
			String m_class; // the class name (of the derived class)
		};

} // dwarf

#endif // D_EXCEPTION_H_
