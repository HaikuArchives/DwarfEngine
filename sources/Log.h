#ifndef D_LOG_H_
#define D_LOG_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Timer.h"

#include "nommgr.h"
#include <stack>
#include "mmgr.h"

namespace dwarf
{
	class TextOutStream;

	/*!
		\brief Through this class you can log what's going on in your appplication.

		It supplies two application-operators that logs to the current log stream.
		By default there's one global instance of this class that logs to "dwarf.log",
		it's simply called logger, you can redirect your log output with the PushOutput()'
		method. Remember that the logstream you give this method must be valid until
		you call PopOutput() (which will make the logger go back and use the previous
		outstream).
	*/
	class Log
	{
	public:
		enum LogLevel
		{
			LOW = 0,
			NORMAL,
			HIGH
		};

		Log(TextOutStream *output);
		Log();
		
		void operator()(const dchar *message, ...);
		void operator()(LogLevel level, const dchar *message, ...);
		
		void PushOutput(TextOutStream *stream);
		void PopOutput();
		TextOutStream *Output();

		void SetLogLevel(LogLevel level) { m_level = level; }
		LogLevel GetLogLevel() { return m_level; }

	private:

		Timer m_time;
		std::stack<TextOutStream*> m_stack;
		TextOutStream *m_output;
		LogLevel m_level;
	};
	
	// this is the main log instance
	extern Log logger;
	
} // dwarf

#endif // D_LOG_H_
