/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "MessageReceiver.h"

using namespace dwarf;

/*!
	\brief constructor.
	\param message_id the message id (for userdefined use U_USER_MESSAGE_ID or greater)
	\param sender if you want the receiver of the message be able to send a reply, you should
	set the sender to the MessageReceiver you want to receive the reply.
	\todo write a new message class that is really good.
*/
Message::Message(uint32 message_id, MessageReceiver *sender):
	Buffer(),
	id(message_id),
	reply_to(sender)
#ifdef _DEBUG
	,sent(false)
#endif
{
}

/*!
	\brief copy constructor.
	\param message the message to coty
*/
Message::Message(const Message &msg):
	Buffer(msg),
	id(msg.id),
	reply_to(msg.reply_to)
#ifdef _DEBUG
	,sent(false)
#endif
{
}

/*!
	\brief default constructor.
*/
Message::Message():
	Buffer(),
	id(0),
	reply_to(0)
#ifdef _DEBUG
	,sent(false)
#endif
{
}

/*!
	\brief copy operator.
	\param message the message to copy
	\return a reference to this message
*/
const Message &Message::operator=(const Message &msg)
{
	// call the baseclass' copy operator
	Buffer::operator=(msg);
	reply_to = msg.reply_to;
	id = msg.id;
	return *this;
}
