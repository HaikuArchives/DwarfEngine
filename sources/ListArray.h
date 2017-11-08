#ifndef D_LISTARRAY_H_
#define D_LISTARRAY_H_

/*
 *	    The Dwarf Engine 3D Library
 *          copyright (c) 2000
 *
 *  #include <license.txt>
 *
 */


#include "Defs.h"
#include "Exception.h"

namespace dwarf
{

		/*!
			\brief This is a list, implemented as an array.

			It is possible to index the items directly in the array,
			for speed.
		*/

		template <class Item> class ListArray
		{
		public:

			class ItemNotFound : public Exception
			{
			public:
				ItemNotFound(const String &message, const String &source):
					Exception(message, source, TEXT("ListArray<Item>::ItemNotFound()"))
				{}
			};

			class Iterator
			{
			friend class ListArray<Item>;
			public:

				Iterator& operator=(const Iterator &i)
				{
					m_current_index = i.m_current_index;
					return *this;
				}

				Iterator &operator++(int)
				{
					int32 i = m_current_index;
					if (m_current_index < m_list.m_length) i++;
					while(i < m_list.m_length && m_list.m_array[i].used == false) i++;
					m_current_index = i;
					return *this;
				}

				bool operator!=(const Iterator &iterator) const
				{
					return (m_current_index != iterator.m_current_index) || (m_list != iterator.m_list);
				}

				bool operator==(const Iterator &iterator) const
				{
					return (m_current_index == iterator.m_current_index) && (m_list == iterator.m_list);
				}
				
				bool IsValid() const
				{
					return (m_current_index < m_list.m_length)  &&  (m_current_index > -1) && m_list.m_array[m_current_index].used;
				}
				
				Item &operator*()
				{
					D_ASSERT(m_current_index < m_list.m_array.Length());
					D_ASSERT(m_current_index >= 0);
					D_ASSERT(m_list.m_array.Length() > 0)

					return m_list.m_array[m_current_index].data;
				}
				
				Iterator operator+(int32 num) const
				{
					Iterator j(*this);
					for (int32 i = 0; i < num; i++) j++;
					return j;
				}

				int32 Index() const
				{
					return m_current_index;
				}
				
			private:
				
				Iterator(ListArray<Item> &lst, int32 index):
					m_list(lst),
					m_current_index(index)
				{}

				ListArray<Item> &m_list;
				int32 m_current_index;
			};


			class ConstIterator
			{
			friend class ListArray<Item>;
			public:

				ConstIterator& operator=(const ConstIterator &i)
				{
					m_current_index = i.m_current_index;
					return *this;
				}

				ConstIterator &operator++(int)
				{
					int32 i = m_current_index;
					if (m_current_index < m_list.m_length) i++;
					while(i < m_list.m_length && m_list.m_array[i].used == false) i++;
					m_current_index = i;
					return *this;
				}

				bool operator!=(const ConstIterator &iterator) const
				{
					return (m_current_index != iterator.m_current_index) || (m_list != iterator.m_list);
				}

				bool operator==(const ConstIterator &iterator) const
				{
					return (m_current_index == iterator.m_current_index) && (m_list == iterator.m_list);
				}
				
				bool IsValid() const
				{
					return (m_current_index < m_list.m_length)  &&  (m_current_index > -1) && m_list.m_array[m_current_index].used;
				}
				
				const Item &operator*() const
				{
					D_ASSERT(m_current_index < m_list.m_array.Length());
					D_ASSERT(m_current_index >= 0);
					D_ASSERT(m_list.m_array.Length() > 0)

					return m_list.m_array[m_current_index].data;
				}
				
				ConstIterator operator+(int32 num) const
				{
					ConstIterator j(*this);
					for (int32 i = 0; i < num; i++) j++;
					return j;
				}

				int32 Index() const
				{
					return m_current_index;
				}
				
			private:
				
				ConstIterator(const ListArray<Item> &lst, int32 index):
					m_list(lst),
					m_current_index(index)
				{}

				const ListArray<Item> &m_list;
				int32 m_current_index;
			};


			ListArray(uint32 length = 1);
			ListArray(const ListArray<Item> &lst);
			virtual ~ListArray();
			ListArray<Item> &operator=(const ListArray<Item> &lst);
			bool operator==(const ListArray<Item> &lst) const { return m_array == lst.m_array; }
			bool operator!=(const ListArray<Item> &lst) const { return m_array != lst.m_array; }
			
			void AddList(const ListArray<Item> &lst);
			int32 AddItem(const Item &item);
			Item &RemoveItemAt(Iterator &i);
			Item &RemoveItemAt(int32 index);

			const ListArray<Item> &Union(const ListArray<Item> &lst);
			ListArray<Item>::Iterator Find(const Item &item) throw(ItemNotFound);
			ListArray<Item>::ConstIterator Find(const Item &item) const throw(ItemNotFound);

			Iterator Begin();
			ConstIterator Begin() const;

			Iterator Last();
			ConstIterator Last() const;

			void Empty();
			int32 ItemCount() const;
			Item &ItemAt(int32 index) const;
			Item &operator[](int32 index) const;

			friend class Iterator;
			friend class ConstIterator;

		private:

			class list_item
			{
			public:
				list_item() : data(), used(false) {}
				~list_item() {}
				list_item(const list_item& li): data(li.data), used(li.used) {}
				const list_item& operator=(const list_item& li) { data = li.data; used = li.used; return *this; }
				Item data;
				bool used;
			};

			Array<list_item> m_array;	// the array
			int32 m_length;	// the maximum index that is used in the lst plus one (it's the index after the last used one)
			int32 m_first_free; // the lowest index that is free in the array, this is used to speed up the insertion
			int32 m_num_items; // the current number of items in the lst
		};

		/*!
			\brief constructor.
			\param length the number of items the array should initilize to.
			The list is of course automatically resized when needed.
		*/
		template <class Item>
		ListArray<Item>::ListArray(uint32 length):
			m_array(length+1),
			m_length(0),
			m_first_free(0),
			m_num_items(0)
		{
		}

		/*!
			\brief copy constructor.
			\param array an array to copy
		*/
		template <class Item>
		ListArray<Item>::ListArray(const ListArray<Item> &lst):
			m_array(lst.m_array),
			m_length(lst.m_length),
			m_first_free(lst.m_first_free),
			m_num_items(lst.m_num_items)
		{

		}
	
		/*!
			\brief destructor.
		*/
		template <class Item>
		ListArray<Item>::~ListArray()
		{
		}
	
		/*!
			\brief a copy operator.
			\warning all your data in the array will be replaced by the data in the given array
			\returns a reference to this object
			\param array an array to copy
		*/
		template <class Item>
		ListArray<Item> &ListArray<Item>::operator=(const ListArray<Item> &lst)
		{
			m_array = lst.m_array;
			m_length = lst.m_length;
			m_first_free = lst.m_first_free;
			m_num_items = lst.m_num_items;

			return *this;
		}

		template <class Item>
		void ListArray<Item>::AddList(const ListArray<Item> &lst)
		{
			for (ListArray<Item>::ConstIterator i = lst.Begin(); i.IsValid(); i++)
				AddItem(*i);
		}

		template <class Item>
		void ListArray<Item>::Empty()
		{
			for (int32 i = 0; i < m_array.Length(); i++)
				m_array[i].used = false;
			m_num_items = 0;
			m_first_free = 0;
			m_length = 0;
		}

		template <class Item>
		int32 ListArray<Item>::AddItem(const Item &item)
		{
			m_array[m_first_free].data = item;
			m_array[m_first_free].used = true;
			
			int32 ret = m_first_free;
			
			int32 i = m_first_free + 1;
			while(true)
			{
				// do we have to extend the lst?
				if (i >= m_length)
				{
					m_length++;
					// do we have to allocate more memory?
					if (m_length >= m_array.Length())
						m_array.SetLength(m_array.Length() + 30);
					
					m_first_free = i;
					break;
				}
				if (m_array[i].used == false)
				{
					m_first_free = i;
					break;
				}
				i++; // increase the counter
			}

			m_num_items++;
			return ret;
		}

		template <class Item>
		inline Item &ListArray<Item>::RemoveItemAt(Iterator &i)
		{
			return RemoveItemAt(i.Index());
		}

		template <class Item>
		Item &ListArray<Item>::RemoveItemAt(int32 index)
		{
			D_ASSERT(index < m_array.Length());
			D_ASSERT(index >= 0);
			D_ASSERT_MESSAGE(m_array[index].used, TEXT("You tried to remove an item that didn't exist"));

			m_array[index].used = false;
			if (m_first_free > index) m_first_free = index;
			m_num_items--;
			return m_array[index].data;
		}

		template <class Item>
		ListArray<Item>::Iterator ListArray<Item>::Begin()
		{
			// find the first used element
			int32 i = 0;
			bool test;
			while(i < m_length && (test = m_array[i].used) == false)
				i++;

			if (i >= m_length) i = 0;

			return Iterator(*this, i);
		}

		template <class Item>
		ListArray<Item>::ConstIterator ListArray<Item>::Begin() const
		{
			// find the first used element
			int32 i = 0;
			bool test;
			while(i < m_length && (test = m_array[i].used) == false)
				i++;

			if (i >= m_length) i = 0;

			return ConstIterator(*this, i);
		}

		template <class Item>
		inline ListArray<Item>::Iterator ListArray<Item>::Last()
		{
			return Iterator(*this, m_length-1);
		}

		template <class Item>
		inline ListArray<Item>::ConstIterator ListArray<Item>::Last() const
		{
			return ConstIterator(*this, m_length-1);
		}
		
		/*!
			Adds all elements from the given lst to the current lst, that does not
			already exist in the current lst. That is, if there are no duplicates of
			any item in the lst, there will not be any after this call either.
			\brief performs a unary operation
		*/
		template <class Item>
		const ListArray<Item> &ListArray<Item>::Union(const ListArray<Item> &lst)
		{
			for (ListArray<Item>::ConstIterator i = lst.Begin(); i.IsValid(); i++)
			{
				try { Find(*i); }
				catch (ItemNotFound e) { AddItem(*i); }
			}
			return *this;
		}

		template <class Item>
		ListArray<Item>::Iterator ListArray<Item>::Find(const Item &item) throw(ItemNotFound)
		{
			for (ListArray<Item>::Iterator i = Begin(); i.IsValid(); i++)
				if (item == *i) return i;

			throw ItemNotFound(TEXT("The item could not be found"), TEXT("ListArray<Item>::Find()"));
		}
	
		template <class Item>
		ListArray<Item>::ConstIterator ListArray<Item>::Find(const Item &item) const throw(ItemNotFound)
		{
			for (ListArray<Item>::ConstIterator i = Begin(); i.IsValid(); i++)
				if (item == *i) return i;

			throw ItemNotFound(TEXT("The item could not be found"), TEXT("ListArray<Item>::Find()"));
		}


		// inline definitions

		/*!
			\brief Tells you the number of items currently in the lst.
			\return the number of items currently in the lst.
		*/
		template <class Item>
		inline int32 ListArray<Item>::ItemCount() const
		{
			return m_num_items;
		}

		/*!
			\brief Gives you acces to the elements in the lst
			\warning There's no check to make sure your index is within the arrays allocated data, so make
			sure you know what you give this method as parameter
			\param index The index you want to acces in the lst
			\return the Item at the given index in the lst
		*/
		template <class Item>
		inline Item &ListArray<Item>::ItemAt(int32 index) const
		{
			D_ASSERT(index >= 0 && index < m_num_items);
			return m_array[index].data;
		}

		/*!
			\brief Gives you acces to the elements in the lst
			\warning There's no check to make sure your index is within the arrays allocated data, so make
			sure you know what you give this method as parameter
			\param index The index you want to acces in the lst
			\return the Item at the given index in the lst
		*/
		template <class Item>
		inline Item &ListArray<Item>::operator[](int32 index) const
		{
			D_ASSERT(index >= 0 && index < m_num_items);
			return m_array[index].data;
		}


} // dwarf

#endif // D_LISTARRAY_H_

