/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */

#include <stack>


#include "Defs.h"

#include "Log.h"
#include "String.h"
#include "TextOutStream.h"
#include "Timer.h"

using namespace std;
using namespace dwarf;

namespace dwarf
{
	Log logger;
}

/*!
	\brief constructs a default logger, without any output.
	Sets the loglevel to normal
*/
Log::Log():
	m_time(),
	m_stack(),
	m_output(0),
	m_level(Log::NORMAL)
{
	m_time.Start();
}

/*!
	\brief constructs the logger, and sets the output to the given stream.
	Sets the loglevel to normal
*/
Log::Log(TextOutStream *outstream):
	m_time(),
	m_stack(),
	m_output(outstream),
	m_level(Log::NORMAL)
{
	m_time.Start();
}

/*!
	\brief Puts the current output stream on top of the stack and sets the current output to the given stream.
	\param stream the new TextOutSteam to be used (all logging is redirected to this stream)
	\warning You will still be responsible to delete the given stream, and you are also
	responsible to make sure that it is not deleted until this instance stops using it (when PopOutput() is called)
	\sa PopOutput Output
*/
void Log::PushOutput(TextOutStream *stream)
{
	D_ASSERT_MESSAGE(stream != 0, TEXT("You called PushStream() with a null-pointer"));
	if (m_output != 0) m_stack.push(m_output);
	m_output = stream;
}

/*!
	\brief makes the log instance use the previous TextOutStream, and trashes the pointer to the current stream.
	\sa PushOutput Output
*/
void Log::PopOutput()
{
	if (m_stack.size() > 0)
	{
		m_output = m_stack.top();
		m_stack.pop();
	}
	else m_output = 0;
}

/*!
	\brief returns a pointer to the current TextOutStream in use.
	\return The currently used stream, this may be 0 if no stream is assigned
*/
TextOutStream *Log::Output()
{
	return m_output;
}

void Log::operator()(const dchar *message, ...)
{
	if(Log::NORMAL <= m_level && m_output != 0)
	{
		String string;
		
		// timestamp this output
		string.Format(TEXT("%03.3f: "), m_time.Current());
		*m_output << string;
		
		
		va_list vl;
		va_start(vl, message);
		string.vaFormat(message, vl);
		va_end(vl);
		
		string += TEXT("\n");
		
		*m_output << string;
		
#if defined( _DEBUG ) && defined( WIN32 )
		OutputDebugString(*string);
#endif
	}
}

void Log::operator()(LogLevel level, const dchar *message, ...)
{
	if(level <= m_level && m_output != 0)
	{
		String string;

		// timestamp this output
		string.Format(TEXT("%03.3f: "), m_time.Current());
		*m_output << string;

	
		va_list vl;
		va_start(vl, message);
		string.vaFormat(message, vl);
		va_end(vl);
		
		string += TEXT("\n");

		*m_output << string;
		
#if defined( _DEBUG ) && defined( WIN32 )
		OutputDebugString(*string);
#endif
	}
}

/*
namespace
{
	TextOutStream *str = 0;
	Timer logtime;
	int32 loglevel = Logl::NORMAL;
};

void dwarf::SetLogStream(TextOutStream *stream)
{
	if(!stream)
	{
		str = 0;
		return;
	}

	str = stream;

	logtime.Start(0);

#if defined( _DEBUG ) && defined( WIN32 )
		OutputDebugString(TEXT("\r\n"));
#endif
}

TextOutStream *dwarf::LogOutStream()
{
	return str;
}

void dwarf::Log(int32 level, const dchar* msg, ...)
{
	if(str && loglevel >= level)
	{
		String string;

		va_list vl;
		va_start(vl, msg);
		string.vaFormat(msg, vl);
		va_end(vl);
	
		string += TEXT("\n");
		
#if defined( _DEBUG ) && defined( WIN32 )
		OutputDebugString(*string);
#endif

		String time;

		time.Format(TEXT("(%03.3f)"), logtime.Current());


		String out = time + string;

		*str << out;
	}
}


void dwarf::Log(const dchar *msg, ...)
{
	if(str)
	{
		String string;
	
		va_list vl;
		va_start(vl, msg);
		string.vaFormat(msg, vl);
		va_end(vl);

		Log(LogLevel::NORMAL, *string);
	}
}

void dwarf::SetLogLevel(int32 level)
{
	if(loglevel <= LogLevel::HIGH)
		loglevel = level;
}

int32	dwarf::GetLogLevel()
{
	return loglevel;
}

*/