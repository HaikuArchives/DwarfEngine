#ifndef D_ASSERT_H
#define D_ASSERT_H

/*!	D_ASSERT
	If the project is compiled in release mode the macro won't do anything.
	\brief if the given statement is false the message will be showed and the application will terminate.
	\param x the statement
	\param msg the message to show
*/
#ifdef _DEBUG
#	define D_ASSERT(x) \
	if(!(x)) \
	{ \
		assert_message(__FILE__, __LINE__, TEXT(#x), TEXT("")); \
	}
#else
#	define D_ASSERT(x)
#endif

/*! D_ASSERT_MESSAGE
	\brief a macro that will check the given condition for validity
	\param x a condition that should be true, if it's false the mackro will assume that there's
	a bug somewhere and stop execution. The location (filename and line number) and the statement
	that failed will be presented to you and logged to the log stream.
	\param msg a message that will be shown to you, this can be used especially when someone else
	is trying to use your code and doesn't know why it goes wrong.
*/
#ifdef _DEBUG
#	define D_ASSERT_MESSAGE(x, msg) \
	if(!(x)) \
	{ \
		assert_message(__FILE__, __LINE__, TEXT(#x), msg); \
	}
#else
#	define D_ASSERT_MESSAGE(x, msg)
#endif

#endif // D_ASSERT_H