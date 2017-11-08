#ifndef D_MESSAGERECEIVER_H_
#define D_MESSAGERECEIVER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "nommgr.h"
#include <queue>

#include "Defs.h"

#include "Message.h"
#include "MessageFilter.h"
#include "Array.h"
#include "List.h"

namespace dwarf
{


	class MessageReceiver;

	/*!
		The MessageReceiver is a class that can receive messages (Message).
		It has an internal message queue. Every time Tick() is called it traverses
		through it's queue. If the queue contains any messages it calls the hook function
		void MessageReceived(Message *) for every message it finds in the queue.
		The implementation of the message receiver is supposed to handle the messsage
		in it's message received. When MessageReceived() returns the message receiver
		deletes the message. If you want to send the message ton someone else or store
		the message (i.e. you don't want the message receiver to delete it) you must call
		DetachMessage() from within MessageReceived(). That call will make sure the currently
		handled message will not be deleted by the MessageReceiver (and hand over the
		responsibility to you).
		
		When you're sending a message to a MessageReceiver, you call PostMessage(). When
		you call PostMessage(), you also hands over the responsibility (ov the message) to
		the message system (you should never delete a message you've just posted, or post 
		the same message to more than one MessageReceiver. If you want to do that, copy
		the message and send the copy)
		\brief A baseclass that receives messages
	*/

	class MessageReceiver
	{
	public:


		MessageReceiver();
		virtual ~MessageReceiver();
	
		void PostMessage(Message *message, MessageReceiver *reply_to = 0);
		void PostMessage(uint32 message_id, MessageReceiver *reply_to = 0);
	
		void SetFilter(const MessageFilter &filter);
		void RemoveFilter();
	
		virtual void Tick(float frame_time);
		const MessageReceiver &operator=(const MessageReceiver &receiver);


	protected:

		void EmptyMessageQueue();
		
		/*!
			\brief tells you how many messages there are in the queue
			\return the number of messages currently in the message queue
		*/
		int32 MessageCount();
		virtual void MessageReceived(Message *message);
		void DetachMessage();
	
	private:



		bool m_detach_message;
		// points to the currently processed message (can be 0)
		Message *m_current_message;
	
		MessageFilter m_filter;
		std::queue<Message*> m_messages;
	};




	// inline definitions
	
	/*!
		\brief sets a message filter that will filter all incoming messages
		\param filter an instance of the filter to use
		\warning the message receiver will take over ownership of the filter.
		Once you've attached the filter you should not delete it.
	*/
	inline void MessageReceiver::SetFilter(const MessageFilter &message_filter)
	{
		m_filter = message_filter;
	}
	
	/*!
		\brief removes the filter and replaces it with a default filter, that
		let every message pass
	*/
	inline void MessageReceiver::RemoveFilter()
	{
		m_filter = MessageFilter();
	}
	
	/*!
		detaches the message currently being processed, i.e. this method
		can only be used from within MessageReceived(). If the message
		is detached the message is not deleted by the MessageReceiver
		and the responsibility to delete the message is left to you.
		\brief detatches the message currently being processed
	*/
	inline void MessageReceiver::DetachMessage()
	{
		if (m_current_message)
		{
			#ifdef _DEBUG
			m_current_message->sent = false;
			#endif
			m_detach_message = true;
		}
	}
	
	/*!
		\brief posts an empty message with the given id
		\param message_id the message id to send in the message
		\param reply_to if you want the message reveiver to be able to  reply to your
		message you can give it a pointer to a MessageReceiver (to reply to)
	*/
	inline void MessageReceiver::PostMessage(uint32 message_id, MessageReceiver *reply_to)
	{
		Message *msg = new Message(message_id);
//		PostMessage(Message(message_id), reply_to);
		PostMessage(msg, reply_to);
	}

} // dwarf

#endif // D_MESSAGERECEIVER_H_
