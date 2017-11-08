#ifndef D_MESSAGEFILTER_H_
#define D_MESSAGEFILTER_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */



namespace dwarf
{

		class Message;
		
		/*!
			\brief Can be used to filter messages.
		*/
		class MessageFilter
		{
			public:
			MessageFilter();
			virtual ~MessageFilter();
			
			virtual bool Filter(Message *message);
		};

} // dwarf

#endif // D_MESSAGEFILTER_H_
