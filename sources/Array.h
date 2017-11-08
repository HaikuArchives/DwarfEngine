#ifndef D_ARRAY_H_
#define D_ARRAY_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"
#include "Math.h"

namespace dwarf
{


	/*!
		\brief A dynamic array.
		The Array class is (just as it's name implies) an array. It can be reallocated by calling SetLength()
		which reallocates the array with the new length. This makes the array easy to use for dynamic
		memory structures. Example usage:
	*/

	template <class Item> class Array
	{
	public:

		Array(int32 length = 1);
		Array(const Array<Item> &array);
		Array(const Item *array,int32 length);
		virtual ~Array();

		void SetLength(int32 length, bool keep_data = true);
		int32 Length() const;

		Item & operator[](int32 index) const;
		Item At(int32 index) const;

		Array<Item> & operator=(const Array<Item> &array);
		bool operator==(const Array &array) const { return m_array == array.m_array; }
		bool operator!=(const Array &array) const { return m_array != array.m_array; }

		Item* operator*() const;

	private:

		Item*	m_array;	// the actual array
		int32	m_length;	// the number of items in the array

	};

	/*!
		\brief constructor
		\param length the length to initialize the array to.
		\warning note that the length is not equal to the number of bytes, it's the number of <class Item>
		(the size of the array depends on the template datatype and the length)
	*/
	template <class Item>
	Array<Item>::Array(int32 length):
		m_array(0),
		m_length(0)
	{
		D_ASSERT_MESSAGE(length >= 0, TEXT("You are trying to allocate an array with less than 0 elements"));
		if(length > 0)
		{
			m_array = new Item[length];
			m_length = length;
		}
		else
		{
			m_array = 0;
			m_length = 0;
		}
	}

	/*!
		\brief copy constructor
		\param array an array to copy
	*/
	template <class Item>
	Array<Item>::Array(const Array<Item> &array):
		m_array(0),
		m_length(0)
	{
		uint32 arrlen = array.Length();
		if(arrlen)
		{
			m_array = new Item[arrlen];
			m_length = arrlen;
			for (int32 i = 0; i < m_length; i++)
				m_array[i] = array.m_array[i];
		}
		else
		{
			m_array = 0;
			m_length = 0;
		}
	}

	/*!
		\brief pointer copy constructor
		\param array pointer to an array to copy
	*/

	template <class Item>
	Array<Item>::Array(const Item *array,int32 length):
		m_array(0),
		m_length(length)
	{
		if(length)
		{
			m_array = new Item[length];
			for (int32 i = 0; i < m_length; i++)
				m_array[i] = array[i];
		}
		else
		{
			m_array = 0;
			m_length = 0;
		}
	}
		
	/*!
		\brief destructor
	*/
	template <class Item>
	Array<Item>::~Array()
	{
		if (m_length) delete []m_array;
	}

	/*!
		\brief a copy operator
		\warning all your data in the array will be replaced by the data in the given array
		\returns a reference to this object
		\param array an array to copy
	*/
	template <class Item>
	Array<Item> & Array<Item>::operator=(Array<Item> const& array)
	{
		if (array.Length())
		{
			SetLength(array.Length());
			for (int32 i = 0; i < m_length; i++)
				m_array[i] = array.m_array[i];
		}
		else if (m_length)
		{
			delete [] m_array;
			m_array = 0;
			m_length = 0;
		}
		return *this;
	}

	/*!
		\brief Forces the array to the new size
		\warning all your data that doesn't fit in the new array are deleted (the data at the end of the former array)
		\param length the new length of the array
		\param keep_data if set to true (default) the current data in the array is kept, otherwise the data will be
		deleted.
	*/
	template <class Item>
	void Array<Item>::SetLength(int32 length, bool keep_data)
	{
		D_ASSERT_MESSAGE(length >= 0, TEXT("You are trying to set the length of an array to less than 0"));
		if (length > 0)
		{

			Item* new_array = 0;
			new_array = new Item[length];

			// copy the old array into the new only if we want to keep the data
			if (keep_data)
			{
				for (int32 i = 0; i < min(m_length, length); i++)
					new_array[i] = m_array[i];
			}

			D_ASSERT_MESSAGE((m_length > 0) == (m_array != 0), TEXT("the length member in Arary, is not set corerctly! This causes a memory leak, FIX IT!"));
			if (m_length > 0) delete [] m_array;

			m_length = length;
			m_array = new_array;
		}
		else if (m_length > 0)
		{
			delete [] m_array;
			m_array = 0;
			m_length = 0;
		}
	}

	// inline definitions

	/*!
		\brief Tells you the length of your array
		\return the length of the array (not in bytes, number of template Items)
	*/
	template <class Item>
	inline int32 Array<Item>::Length() const
	{
		return m_length;
	}

	/*!
		\brief Gives you access to the internal array pointer
		\return The pointer to the internal array
		\warning This pointer is invalid as soon as you call SetLength(),
		you should never store this pointer. The
		returned pointer may be 0 (if the array has the lenght 0)
	*/
	template <class Item>
	inline Item* Array<Item>::operator*() const
	{
		return m_array;
	}

	/*!
		\brief Gives you acces to the elements in the array
		\warning There's no check to make sure your index is within the arrays allocated data, so make
		sure you know what you give this method as parameter
		\param index The index you want to access in the array
		\return a reference to the Item at the given index in the array
	*/
	template <class Item>
	inline Item& Array<Item>::operator[](int32 index) const
	{
		D_ASSERT_MESSAGE(index >= 0 && index < m_length, TEXT("index out of bound on array indexing operation"))
		return m_array[index];
	}

	/*!
		\brief Gives you read acces to the elements in the array
		\warning There's no check to make sure your index is within the arrays allocated data, so make
		sure you know what you give this method as parameter
		\param index The index you want to access in the array
		\return the Item at the given index in the array
	*/
	template <class Item>
	inline Item Array<Item>::At(int32 index) const
	{
		D_ASSERT_MESSAGE(index >= 0 && index < m_length, TEXT("index out of bound on array indexing operation"))
		return m_array[index];
	}

} // dwarf

#endif // D_ARRAY_H_

