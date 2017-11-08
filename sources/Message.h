#ifndef D_MESSAGE_H_
#define D_MESSAGE_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"

#include "Buffer.h"

namespace dwarf
{

		//! predefined system messages
		enum
		{
			/*!
			    This message is sent to the window when the user clicks the close button on the window
			*/
			D_QUIT,
			D_SHOW,
			D_HIDE,
			D_REMOVE,
			D_REMOVE_AND_DELETE,

			/*!
			    this id and greater can be used for user defined message
			*/
			D_USER_MESSAGE_ID = 100
		};

		class MessageReceiver;

		/*!
			\brief Used for internal communication.
		
			The Message class represents a message. The message contains
			one member that tells you what kind of message it is. It derives from Buffer
			which allows it to contain attached data. This data can be strings, points, rectangles anything.
			The trick is that you need to know the content of the message just by looking at the id member
			(so you can interpret the data in the buffer).
			\warning You have to pop the data from the message in the right order to get it right (see uBuffer)
		*/
		class Message : public Buffer
		{
			friend class MessageReceiver;
			public:

		
			Message(uint32 message_id, MessageReceiver *sender = 0);
			Message(const Message &msg);
			Message();

	
			/*!
				\brief the message id.
			*/
			uint32 id;
			
			/*!
				\brief reply_to points to the MessageReceiver that you should send a reply to, or it's zero.
				\warning replt_to is most likely zero, be sure to check that every time you use it
			*/
			MessageReceiver *reply_to;	// who sent this? can be 0!
			
			const Message &operator=(const Message &msg);

			private:

			#ifdef _DEBUG
			bool sent; // has the message passed through PostMessage?
			#endif
		};

} // dwarf

#endif // D_MESSAGE_H_
