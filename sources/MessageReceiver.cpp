/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "MessageReceiver.h"
#include "Message.h"
#include "MessageFilter.h"
#include "Log.h"

using namespace dwarf;

/*!
	\brief default constructor
*/
MessageReceiver::MessageReceiver():
	m_detach_message(false),
	m_current_message(0)
{
}

/*!
	\brief virtual destructor
*/
MessageReceiver::~MessageReceiver()
{
}

/*!
	In every useful implementation of MessageReceiver (View and Window)
	The Tick() method is called every cycle (when the uView is added to
	another view or a window).
	\brief The Tick() method has to be called every now and then
	frame_time the time (in seconds) since the method was called last time
	\warning this implementation of Tick() is not empty, and should be called from
	from a new implementation.
*/
void MessageReceiver::Tick(float frame_time)
{
	while (m_messages.size())
	{
		m_detach_message = false; // reset this flag to default
		m_current_message = m_messages.front();
		m_messages.pop();

		D_ASSERT_MESSAGE(m_current_message != 0, TEXT("A null-pointer message has been received."));

		MessageReceived(m_current_message);
		
		if (!m_detach_message)
		{
			// if the message was not detached, delete it
			delete m_current_message;
			m_current_message = 0;
		}
	}
}

/*!
	\brief This method is called when you get a message
	\param message The message that was sent.
	\warning The message you get here does NOT belong to you,
	you may NOT delete it (unless you run DetachMessage())
*/
void MessageReceiver::MessageReceived(Message *message)
{
}

/*!
	\brief removes all messages from the message queue
*/
void MessageReceiver::EmptyMessageQueue()
{
	while (!m_messages.empty())
	{
		Message *message = m_messages.front();
		m_messages.pop();
		delete message;
	}
}

/*!
	posts a message to this message receiver. Once you have sent the
	message, the MessageReceiver will take over the responsibility to
	delete the message (and do whatever it wants with it). So, you should NOT
	don anything with the message after you've sent it, you don not own it anymore.
	\brief sends a message to the message receiver
	\param message the message you want to send
	\param reply_to if you want the message receiver to be able to reply on your
	message you can give it a pointer to a MessageReceiver (to reply to)
*/
void MessageReceiver::PostMessage(Message *message, MessageReceiver *reply_to)
{
	D_ASSERT_MESSAGE(message, TEXT("You are trying to send a null-message (a null-pointer instead of a message pointer)"));
	D_ASSERT_MESSAGE(message->sent == false, TEXT("Attempt to send a message twice without detaching it from it's message receiver (that's illegal)."));

	message->Rewind(); // make sure the receiver can read all the data.

	message->reply_to = reply_to;
	// ask the filter if we should receive the message or remove it
	if (m_filter.Filter(message))
	{
		#ifdef _DEBUG
		message->sent = true;
		#endif
		// enqueue the message
		m_messages.push(message);
	}
	else
	{
		// delete the message
		delete message;
	}
}

/*!
	\brief copy operator
*/
const MessageReceiver &MessageReceiver::operator=(const MessageReceiver &receiver)
{
	m_messages = receiver.m_messages;
	m_filter = receiver.m_filter;
	m_current_message = 0;
	m_detach_message = false;
	return *this;
}
