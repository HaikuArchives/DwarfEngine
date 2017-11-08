/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Exception.h"

using namespace dwarf;



void dwarf::Error(const dchar *msg, ...)
{
	String buf;
	va_list vl;
	va_start(vl, msg);
	buf.vaFormat(msg, vl);
	va_end(vl);

	MsgBox(TEXT("Fatal error"), buf);
			
	::exit(1);
}



Exception::Exception(const Exception &exception):
	m_message(exception.m_message),
	m_source(exception.m_source),
	m_class(exception.m_class)
{
//		logger(TEXT("Exception thrown from %s (%s), message: \"%s\""),
//			*m_source, *m_class, *m_message);
}

/*!
	\param user_message the description of the exception, may be showed to the user
	\param source the name of the class::method that threw the exception
	\param class_name the name of the exception class
*/
Exception::Exception(const String &user_message, const String &source, const String &class_name):
	m_message(user_message),
	m_source(source),
	m_class(class_name)
{
//		logger(TEXT("Exception thrown from %s (%s), message: \"%s\""),
//			*m_source, *m_class, *m_message);
}

Exception::~Exception()
{
}
	

const Exception &Exception::operator=(const Exception &exception)
{
	m_message = exception.m_message;
	m_source = exception.m_source;
	m_class = exception.m_class;
	return *this;
}

const String &Exception::Message() const
{
	return m_message;
}

const String &Exception::Source() const
{
	return m_source;
}

const String &Exception::Class() const
{
	return m_class;
}

void Exception::OpenDialog() const
{
	MsgBox(TEXT("DwarfEngine 3D Error message"),
		TEXT("Exception thrown!\n\n\nType:  \t ") +
		m_class   + TEXT("\n")+
		TEXT("From:  \t ") + m_source  + TEXT("\n")+
		TEXT("Message:\t ") + m_message + TEXT("\n"));
}